#pragma once

#include <mshtml.h>

class CHTMLWriter
{
protected:
	IUnknown * m_pUnk;
	IWebBrowser2 * m_pWB;
	IDispatch * m_pDoc;
	bool m_bReleaseIWebBrowserPtr;

public:
	CHTMLWriter( IUnknown * pUnk );
	CHTMLWriter( IWebBrowser2 * pWB );
	~CHTMLWriter( void );
	bool Write( LPCTSTR pszHTMLContent );
	bool Add( LPCTSTR pszHTMLContent );
	bool RunScript(LPCTSTR pszHTMLContent);
	bool SetMapBrowser(BSTR szHtmlPath);

private:
	CHTMLWriter( void ) {};
	bool GetDocumentPtr( void );
};