
// WebView2DownloadOperationStateChangedIssueReproView.h : interface of the CWebView2DownloadOperationStateChangedIssueReproView class
//

#pragma once

#include "WebView2Ctrl.h"

#include <memory>

class CWebView2DownloadOperationStateChangedIssueReproView : public CView
{
protected: // create from serialization only
	CWebView2DownloadOperationStateChangedIssueReproView() noexcept;
	DECLARE_DYNCREATE(CWebView2DownloadOperationStateChangedIssueReproView)

// Attributes
public:
	CWebView2DownloadOperationStateChangedIssueReproDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CWebView2DownloadOperationStateChangedIssueReproView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

protected:
  void OnInitialUpdate() override;
  
  // Generated message map functions
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnSetFocus(CWnd* pOldWnd);

  // DATA
  std::unique_ptr<CWebView2Ctrl> m_pWebView2Ctrl{ nullptr };

private:
  // Implementation
  void CreateWebView2Ctrl();
  void OnWebView2CtrlCreated();
};

#ifndef _DEBUG  // debug version in WebView2DownloadOperationStateChangedIssueReproView.cpp
inline CWebView2DownloadOperationStateChangedIssueReproDoc* CWebView2DownloadOperationStateChangedIssueReproView::GetDocument() const
   { return reinterpret_cast<CWebView2DownloadOperationStateChangedIssueReproDoc*>(m_pDocument); }
#endif

