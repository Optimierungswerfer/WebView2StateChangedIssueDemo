
// WebView2DownloadOperationStateChangedIssueReproView.cpp : implementation of the CWebView2DownloadOperationStateChangedIssueReproView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WebView2DownloadOperationStateChangedIssueRepro.h"
#endif

#include "WebView2DownloadOperationStateChangedIssueReproDoc.h"
#include "WebView2DownloadOperationStateChangedIssueReproView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWebView2DownloadOperationStateChangedIssueReproView

IMPLEMENT_DYNCREATE(CWebView2DownloadOperationStateChangedIssueReproView, CView)

BEGIN_MESSAGE_MAP(CWebView2DownloadOperationStateChangedIssueReproView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CWebView2DownloadOperationStateChangedIssueReproView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CWebView2DownloadOperationStateChangedIssueReproView construction/destruction

CWebView2DownloadOperationStateChangedIssueReproView::CWebView2DownloadOperationStateChangedIssueReproView() noexcept
{
	// TODO: add construction code here

}

CWebView2DownloadOperationStateChangedIssueReproView::~CWebView2DownloadOperationStateChangedIssueReproView()
{
}

BOOL CWebView2DownloadOperationStateChangedIssueReproView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CWebView2DownloadOperationStateChangedIssueReproView drawing

void CWebView2DownloadOperationStateChangedIssueReproView::OnDraw(CDC* /*pDC*/)
{
	CWebView2DownloadOperationStateChangedIssueReproDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CWebView2DownloadOperationStateChangedIssueReproView printing


void CWebView2DownloadOperationStateChangedIssueReproView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CWebView2DownloadOperationStateChangedIssueReproView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CWebView2DownloadOperationStateChangedIssueReproView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CWebView2DownloadOperationStateChangedIssueReproView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CWebView2DownloadOperationStateChangedIssueReproView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CWebView2DownloadOperationStateChangedIssueReproView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CWebView2DownloadOperationStateChangedIssueReproView diagnostics

#ifdef _DEBUG
void CWebView2DownloadOperationStateChangedIssueReproView::AssertValid() const
{
	CView::AssertValid();
}

void CWebView2DownloadOperationStateChangedIssueReproView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWebView2DownloadOperationStateChangedIssueReproDoc* CWebView2DownloadOperationStateChangedIssueReproView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWebView2DownloadOperationStateChangedIssueReproDoc)));
	return (CWebView2DownloadOperationStateChangedIssueReproDoc*)m_pDocument;
}
#endif //_DEBUG


// CWebView2DownloadOperationStateChangedIssueReproView message handlers
