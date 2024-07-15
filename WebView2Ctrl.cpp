#include "pch.h"

#include "WebView2Ctrl.h"

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.collections.h> // IVector
#include <winrt/Microsoft.Web.WebView2.Core.h>

#include <list>

using namespace ::winrt;
using namespace ::winrt::Windows::Foundation;
using namespace ::winrt::Microsoft::Web::WebView2::Core;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

struct CWebView2CtrlImpl
{
  explicit CWebView2CtrlImpl(CoreWebView2Controller&& controller)
    : m_webView2Controller{ controller }
    , m_webView2{ m_webView2Controller.CoreWebView2() }
  {}
  CWebView2CtrlImpl(const CWebView2CtrlImpl&) = delete;
  CWebView2CtrlImpl& operator=(const CWebView2CtrlImpl&) = delete;
  CWebView2CtrlImpl(CWebView2CtrlImpl&&) = delete;
  CWebView2CtrlImpl& operator=(CWebView2CtrlImpl&&) = delete;
  ~CWebView2CtrlImpl()
  {
    m_webView2Controller.Close();
  }

  CoreWebView2Controller m_webView2Controller;
  CoreWebView2 m_webView2;
};

namespace Internal
{
  static void ShowFailure(const winrt::hresult_error& hr)
  {
    CString strErrorMessage{};
    strErrorMessage.Format(L"Internal Error!\n\nAn unexpected error occurred in the WebView2 control (error code 0x%08X).\n\n%s", hr.code().operator INT32(), hr.message().c_str());
    AfxMessageBox(strErrorMessage.operator LPCWSTR(), MB_OK | MB_ICONERROR);
  }

  std::list<CoreWebView2DownloadOperation> g_DownloadOperationHandles{}; // Needed to keep alive at least one handle per download operation in order for the StateChanged event to work.
}

/////////////////////////////////////////////////////////////////////////////
// CWebView2Ctrl properties
BEGIN_MESSAGE_MAP(CWebView2Ctrl, CWnd)
  ON_WM_DESTROY()
  ON_WM_SIZE()
  ON_WM_SETFOCUS()
END_MESSAGE_MAP()

BOOL CWebView2Ctrl::Create(LPCTSTR /*lpszClassName*/, LPCTSTR /*lpszWindowName*/, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext*)
{
  return CreateAsync(dwStyle, rect, pParentWnd, nID, nullptr);
}

BOOL CWebView2Ctrl::CreateAsync(
  DWORD dwStyle,
  const RECT& rect,
  CWnd* pParentWnd,
  UINT nID,
  CallbackFunc cbOnCreated
)
{
  if (!CreateHostWindow(nullptr, nullptr, dwStyle, rect, pParentWnd, nID))
  {
    Internal::ShowFailure({});
    return FALSE;
  }

  m_cbOnCreated = cbOnCreated;
  CreateWebView2ControllerAsync();

  return TRUE;
}

void CWebView2Ctrl::Navigate(LPCTSTR lpszUrl)
{
  if (!m_pWebViewImpl)
  {
    ASSERT(false);
    return;
  }

  // Ensure that lpszUrl is at least a valid Uri.
  try
  {
    if (Uri{ lpszUrl }.Host().empty())
    {
      throw hresult_error{};
    }
    m_pWebViewImpl->m_webView2.Navigate(lpszUrl);
  }
  catch (const winrt::hresult_error& hr)
  {
    Internal::ShowFailure(hr);
  }
}

void CWebView2Ctrl::Resize(const LONG width, const LONG height)
{
  SetWindowPos(nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOREPOSITION);
}

void CWebView2Ctrl::OnSize(UINT nType, int cx, int cy)
{
  if (cx || cy)
  {
    ResizeToClientArea();
    return;
  }
  __super::OnSize(nType, cx, cy);
}

void CWebView2Ctrl::OnSetFocus(CWnd* pOldWnd)
{
  if (m_pWebViewImpl)
  {
    m_pWebViewImpl->m_webView2Controller.MoveFocus(CoreWebView2MoveFocusReason::Programmatic);
    return;
  }
  __super::OnSetFocus(pOldWnd);
}

void CWebView2Ctrl::OnDestroy()
{
  delete m_pWebViewImpl;
  m_pWebViewImpl = nullptr;
}

winrt::fire_and_forget CWebView2Ctrl::CreateWebView2ControllerAsync()
{
  CoreWebView2EnvironmentOptions environmentOptions{};
  // We enable experimental feature BrowserHitTransparent because it seems to solve a lot of our problems
  // The feature is still undocumented, but we found it in some issue discussions where it is stated that "Focus and Inputs will be on native apps in this mode" (https://github.com/MicrosoftEdge/WebView2Feedback/issues/1703)
  // https://github.com/MicrosoftEdge/WebView2Feedback/issues/468 offers an immense discussion on this topic and Microsoft still wants to implement further support on this topic.
  // As Microsoft WebView2 developers say that this experimental feature is quite a big change in their code base, we do not imagine that we can solve our issues on our own with reasonable effort
  // The following known issues are fixed with that experimental feature in our context:
  //   - An opened menu is not closed when clicking into WebView2 control
  //   - Implemented Keyboard Accelerators do not work (e.g. Ctrl+N , Ctrl+F6)
  //   - 'Alt' for Menu access via keyboard
  environmentOptions.AdditionalBrowserArguments(L"--enable-features=msWebView2BrowserHitTransparent");
  try
  {
    CoreWebView2Environment environment = co_await CoreWebView2Environment::CreateWithOptionsAsync({}, {}, environmentOptions);
    const auto wnd{ CoreWebView2ControllerWindowReference::CreateFromWindowHandle(reinterpret_cast<uint64_t>(m_hWnd)) };
    environment.CreateCoreWebView2ControllerAsync(wnd).Completed([this](IAsyncOperation<CoreWebView2Controller> webControllerTask, AsyncStatus status) {
      if (status != AsyncStatus::Completed) // Safeguard against error-state and cancellation
      {
        if (status == AsyncStatus::Error)
        {
          const hresult_error hr{ webControllerTask.ErrorCode() };
          Internal::ShowFailure(hr);
        }
        return;
      }
      ASSERT(m_pWebViewImpl == nullptr); // Must be nullptr, otherwise someone called Create multiple times, which must never happen, because that would overwrite the existing WebView2 with a new instance without cleaning up the old one.
      m_pWebViewImpl = new CWebView2CtrlImpl{ webControllerTask.GetResults() };

      RegisterDownloadOperationStateChangedEventHandler_BROKEN();
      RegisterDownloadOperationStateChangedEventHandler_WORKING();

      ResizeToClientArea();

      if (m_cbOnCreated)
      {
        m_cbOnCreated();
      }
    });
  }
  catch (const winrt::hresult_error& hr)
  {
    Internal::ShowFailure(hr);
  }
}

void CWebView2Ctrl::RegisterDownloadOperationStateChangedEventHandler_BROKEN()
{
  if (!m_pWebViewImpl)
  {
    ASSERT(false);
    return;
  }

  m_pWebViewImpl->m_webView2.DownloadStarting(
    [](auto&&, const CoreWebView2DownloadStartingEventArgs& args)
    {
      TRACE(_T("\n[Info] DownloadStarting! Registering broken StateChanged event handler...\n"));
      const CoreWebView2DownloadOperation& downloadOperation{ args.DownloadOperation() };
      downloadOperation.StateChanged([](auto&&, auto&&) {
        TRACE(_T("\n[Info] Broken StateChanged event handler was called!\n")); // Does not get executed!
      });
    });
}

void CWebView2Ctrl::RegisterDownloadOperationStateChangedEventHandler_WORKING()
{
  if (!m_pWebViewImpl)
  {
    ASSERT(false);
    return;
  }

  m_pWebViewImpl->m_webView2.DownloadStarting(
    [](auto&&, const CoreWebView2DownloadStartingEventArgs& args)
    {
      TRACE(_T("\n[Info] DownloadStarting! Registering working StateChanged event handler...\n"));
      const CoreWebView2DownloadOperation& downloadOperation{ args.DownloadOperation() };
      Internal::g_DownloadOperationHandles.push_back(downloadOperation); // Storing a reference to the download operation fixes the issue.
      downloadOperation.StateChanged([](auto&&, auto&&) {
        TRACE(_T("\n[Info] Working StateChanged event handler was called!\n")); // Does get executed!
      });
    });
}

void CWebView2Ctrl::ResizeToClientArea()
{
  if (!m_pWebViewImpl)
  {
    return;
  }

  RECT bounds;
  GetClientRect(&bounds);
  // These casts are necessary to convert from Win32 RECT type to WinRT Rect type:
  m_pWebViewImpl->m_webView2Controller.Bounds(
    Rect{
      static_cast<float>(bounds.left),
      static_cast<float>(bounds.top),
      static_cast<float>(bounds.right),
      static_cast<float>(bounds.bottom)
    });
}

BOOL CWebView2Ctrl::CreateHostWindow(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
  if (lpszClassName == nullptr)
    lpszClassName = GetWindowClass();

  if (!CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID))
    return FALSE;

  return TRUE;
}

PCTSTR CWebView2Ctrl::GetWindowClass()
{
  static PCTSTR windowClass = []
  {
    static TCHAR const* className = L"EdgeBrowserHost";

    WNDCLASSEX wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = ::DefWindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = AfxGetInstanceHandle();
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = className;
    wcex.hIconSm = nullptr;

    ATOM result = RegisterClassEx(&wcex);
    if (result == 0)
    {
      [[maybe_unused]] DWORD lastError = ::GetLastError();
    }

    return className;
  }();

  return windowClass;
}
