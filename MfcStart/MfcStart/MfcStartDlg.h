
// MfcStartDlg.h: 헤더 파일
//

#pragma once


// CMfcStartDlg 대화 상자
class CMfcStartDlg : public CDialogEx
{
// 생성입니다.
public:
	CMfcStartDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCSTART_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnDoubleclickedBtnTest();
	afx_msg void OnBnClickedBtnTest();
	int m_nNum;
	int m_nNum2;
	CString m_strResult;
};
