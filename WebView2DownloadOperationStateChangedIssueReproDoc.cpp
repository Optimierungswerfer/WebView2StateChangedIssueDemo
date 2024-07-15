
// WebView2DownloadOperationStateChangedIssueReproDoc.cpp : implementation of the CWebView2DownloadOperationStateChangedIssueReproDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WebView2DownloadOperationStateChangedIssueRepro.h"
#endif

#include "WebView2DownloadOperationStateChangedIssueReproDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWebView2DownloadOperationStateChangedIssueReproDoc

IMPLEMENT_DYNCREATE(CWebView2DownloadOperationStateChangedIssueReproDoc, CDocument)

BEGIN_MESSAGE_MAP(CWebView2DownloadOperationStateChangedIssueReproDoc, CDocument)
END_MESSAGE_MAP()


// CWebView2DownloadOperationStateChangedIssueReproDoc construction/destruction

CWebView2DownloadOperationStateChangedIssueReproDoc::CWebView2DownloadOperationStateChangedIssueReproDoc() noexcept
{
	// TODO: add one-time construction code here

}

CWebView2DownloadOperationStateChangedIssueReproDoc::~CWebView2DownloadOperationStateChangedIssueReproDoc()
{
}

BOOL CWebView2DownloadOperationStateChangedIssueReproDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CWebView2DownloadOperationStateChangedIssueReproDoc serialization

void CWebView2DownloadOperationStateChangedIssueReproDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CWebView2DownloadOperationStateChangedIssueReproDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CWebView2DownloadOperationStateChangedIssueReproDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWebView2DownloadOperationStateChangedIssueReproDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWebView2DownloadOperationStateChangedIssueReproDoc diagnostics

#ifdef _DEBUG
void CWebView2DownloadOperationStateChangedIssueReproDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWebView2DownloadOperationStateChangedIssueReproDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWebView2DownloadOperationStateChangedIssueReproDoc commands
