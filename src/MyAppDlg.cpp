
// MyAppDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "string_utility.h"
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

CMyAppDlg::~CMyAppDlg()
{
}


void CMyAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INPUT_PATH, edit_input_path);
	DDX_Control(pDX, IDC_EDIT_OUTPUT_PATH, edit_output_path);
	DDX_Control(pDX, IDC_CHECK_SUBDIRECTORY, check_subdirectory);
	DDX_Control(pDX, IDC_EDIT_APP_STATUS, edit_app_status);
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

	edit_app_status.SetWindowTextW(L"待機中");

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
	if (!CheckPath())
		return;

	edit_app_status.SetWindowTextW(L"実行中");

	// スレッド生成
	AfxBeginThread(Execute, this);
}

bool CMyAppDlg::CheckPath()
{
	if (edit_input_path.GetWindowTextLengthW() == 0)
	{
		AfxMessageBox(_T("入力パスが入力されていません"));
		return false;
	}
	if (edit_output_path.GetWindowTextLengthW() == 0)
	{
		AfxMessageBox(_T("出力パスが入力されていません"));
		return false;
	}

	CString path;
	edit_input_path.GetWindowTextW(path);
	int accesibility = _taccess(path, 00);

	if (accesibility != 0)
	{
		AfxMessageBox(_T("入力パスが存在しません"));
		return false;
	}

	edit_output_path.GetWindowTextW(path);
	accesibility = _taccess(path, 00);

	if (accesibility != 0)
	{
		AfxMessageBox(_T("出力パスが存在しません"));
		return false;
	}

	// 出力先に書き込み権限があるかチェック

	// 出力先が空でなければ警告

	// 入出力パスが被っていれば警告

	// 入力パス配下にフォルダがなければ警告

	// ダイアログにステータス表示

	return true;
}

UINT CMyAppDlg::Execute(LPVOID pParam)
{
	CMyAppDlg* pDlg = (CMyAppDlg*)pParam;

	// ダイアログアイテムの無効化
	pDlg->EnableDialogItem(false);

	// ★メンバ変数の排他制御が必要

	// フォルダリスト作成
	CString input_path;
	pDlg->edit_input_path.GetWindowText(input_path);
	vector<wstring> folder_list;
	bool isIncludingSubdirectory = (pDlg->check_subdirectory.GetCheck() == BST_CHECKED) ? true : false;
	winapi_utility::CreateFolderList((wstring)input_path, isIncludingSubdirectory, folder_list);

	// 出力先のパス取得
	CString output_path;
	pDlg->edit_output_path.GetWindowText(output_path);
	wstring output_path_wstr = output_path;

	// フォルダ作成
	for (auto& folder_path : folder_list)
	{
		folder_path.replace(0, _tcsclen(input_path), output_path_wstr.c_str());
		int result = ::SHCreateDirectory(NULL, folder_path.c_str());

		switch (result)
		{
		case ERROR_SUCCESS:
			pDlg->success_count++;
			break;
		case ERROR_FILE_EXISTS:
		case ERROR_ALREADY_EXISTS:
			pDlg->exists_count++;
			break;
		default:
			pDlg->error_count++;
			break;
		}
	}

	wstring finish_message;
	finish_message = string_utility::Format(L"処理を終了しました。\n - コピー成功：%d件\n - 既に存在：%d件\n - コピー失敗：%d件", pDlg->success_count, pDlg->exists_count, pDlg->error_count);
	AfxMessageBox(finish_message.c_str());

	// 変数のリセット
	pDlg->success_count = 0;
	pDlg->exists_count = 0;
	pDlg->error_count = 0;

	// ダイアログアイテムの有効化
	pDlg->EnableDialogItem(true);

	pDlg->edit_app_status.SetWindowTextW(L"待機中");
	
	return 0;
}

void CMyAppDlg::EnableDialogItem(bool isEnable)
{
	GetDlgItem(IDC_EDIT_INPUT_PATH)->EnableWindow(isEnable);
	GetDlgItem(IDC_BUTTON_SELECT_INPUT_PATH)->EnableWindow(isEnable);
	GetDlgItem(IDC_EDIT_OUTPUT_PATH)->EnableWindow(isEnable);
	GetDlgItem(IDC_BUTTON_SELECT_OUTPUT_PATH)->EnableWindow(isEnable);
	GetDlgItem(IDC_CHECK_SUBDIRECTORY)->EnableWindow(isEnable);
}
