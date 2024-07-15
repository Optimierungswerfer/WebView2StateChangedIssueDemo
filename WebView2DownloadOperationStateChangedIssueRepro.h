
// WebView2DownloadOperationStateChangedIssueRepro.h : main header file for the WebView2DownloadOperationStateChangedIssueRepro application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CWebView2DownloadOperationStateChangedIssueReproApp:
// See WebView2DownloadOperationStateChangedIssueRepro.cpp for the implementation of this class
//

class CWebView2DownloadOperationStateChangedIssueReproApp : public CWinAppEx
{
public:
	CWebView2DownloadOperationStateChangedIssueReproApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWebView2DownloadOperationStateChangedIssueReproApp theApp;
