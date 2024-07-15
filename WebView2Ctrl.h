#pragma once

#include <functional>

struct CWebView2CtrlImpl;
namespace winrt
{
  struct fire_and_forget;
}

class CWebView2Ctrl : public CWnd
{
public:
  using CallbackFunc = std::function<void()>;

  CWebView2Ctrl() = default;
  CWebView2Ctrl(const CWebView2Ctrl&) = delete;
  CWebView2Ctrl& operator=(const CWebView2Ctrl&) = delete;
  CWebView2Ctrl(CWebView2Ctrl&&) = delete;
  CWebView2Ctrl& operator=(CWebView2Ctrl&&) = delete;
  virtual ~CWebView2Ctrl() = default;

  BOOL Create(
    LPCTSTR lpszClassName,
    LPCTSTR lpszWindowName,
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    UINT nID,
    CCreateContext* = nullptr) override;

  BOOL CreateAsync(
    DWORD dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    UINT nID,
    CallbackFunc cbOnCreated);

  // WebView control methods
  void Navigate(LPCTSTR lpszUrl);

  // Window methods
  void Resize(const LONG width, const LONG height);

protected:
  DECLARE_MESSAGE_MAP()

protected:
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnDestroy();

private:
  winrt::fire_and_forget CreateWebView2ControllerAsync();

  void RegisterDownloadOperationStateChangedEventHandler_BROKEN();
  void RegisterDownloadOperationStateChangedEventHandler_WORKING();

  void ResizeToClientArea();

  // Window methods
  BOOL CreateHostWindow(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);
  static PCTSTR GetWindowClass();

  CWebView2CtrlImpl* m_pWebViewImpl{ nullptr };
  CallbackFunc m_cbOnCreated{};
};
