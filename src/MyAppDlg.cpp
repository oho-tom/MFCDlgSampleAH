
// MyAppDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "winapi_utility.h"
#include "MyApp.h"
#include "MyAppDlg.h"
#include "afxdialogex.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

// CMyAppDlg ダイアログ



CMyAppDlg::CMyAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INPUT_PATH, edit_input_path);
	DDX_Control(pDX, IDC_EDIT_OUTPUT_PATH, edit_output_path);
}

BEGIN_MESSAGE_MAP(CMyAppDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SELECT_INPUT_PATH, &CMyAppDlg::OnBnClickedButtonSelectInputPath)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_OUTPUT_PATH, &CMyAppDlg::OnBnClickedButtonSelectOutputPath)
	ON_BN_CLICKED(IDOK, &CMyAppDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMyAppDlg メッセージ ハンドラー

BOOL CMyAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CMyAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMyAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyAppDlg::OnBnClickedButtonSelectInputPath()
{
	CFolderPickerDialog dialog;
	dialog.m_ofn.lpstrTitle = L"入力パス";
	if (dialog.DoModal() == IDOK)
	{
		edit_input_path.SetWindowTextW(dialog.GetPathName());
	}
}


void CMyAppDlg::OnBnClickedButtonSelectOutputPath()
{
	CFolderPickerDialog dialog;
	dialog.m_ofn.lpstrTitle = L"出力パス";
	if (dialog.DoModal() == IDOK)
	{
		edit_output_path.SetWindowTextW(dialog.GetPathName());
	}
}


void CMyAppDlg::OnBnClickedOk()
{
	if (edit_input_path.GetWindowTextLengthW() > 0)
	{
		CString input_path;
		edit_input_path.GetWindowText(input_path);
		vector<wstring> folder_list;
		winapi_utility::CreateFolderList((wstring)input_path, true, folder_list);
	}

	CDialogEx::OnOK();
}
