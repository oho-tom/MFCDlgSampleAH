
// MyAppDlg.h : ヘッダー ファイル
//

#pragma once

#include <thread>


// CMyAppDlg ダイアログ
class CMyAppDlg : public CDialogEx
{
// コンストラクション
public:
	CMyAppDlg(CWnd* pParent = nullptr);	// 標準コンストラクター
	~CMyAppDlg();	// デストラクタ

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	// 入力パス
	CEdit edit_input_path;
	// 出力パス
	CEdit edit_output_path;
	// サブディレクトリを含めるかどうかのチェックボックス
	CButton check_subdirectory;
	// アプリケーションの状態(待機中、実行中)
	CEdit edit_app_status;
	// 結果の件数集計用
	int success_count = 0;
	int exists_count = 0;
	int error_count = 0;

public:
	afx_msg void OnBnClickedButtonSelectInputPath();
	afx_msg void OnBnClickedButtonSelectOutputPath();
	afx_msg void OnBnClickedOk();

private:
	bool CheckPath();
	static UINT Execute(LPVOID pParam);
	void EnableDialogItem(bool isEnable);
};
