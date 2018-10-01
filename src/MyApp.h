
// MyApp.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル

#include <string>


// CMyApp:
// このクラスの実装については、MyApp.cpp を参照してください
//

class CMyApp : public CWinApp
{
public:
	CMyApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()

private:
	std::wstring app_directory_;	//<! アプリケーションのディレクトリ
};

extern CMyApp theApp;
