
// MfcCImageDlg.h: 헤더 파일
//

#pragma once
#include "afxwin.h"

// CMfcCImageDlg 대화 상자
class CMfcCImageDlg : public CDialogEx
{
private:
	CImage m_image;
	BOOL validImgPos(int x, int y);
	int nTimes;
// 생성입니다.
public:
	CMfcCImageDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCCIMAGE_DIALOG };
	enum { IDD = IDD_YOUR_DIALOG_ID };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	bool m_bDrawOverlay;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void UpdateDisplay();
	void MoveRect();
	afx_msg void OnBnClickedBtnAction();
	void drawCircle(unsigned char* fm, int i, int j, int nRadius, int nGray);
	bool isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	afx_msg void OnBnClickedBtnMakeSave();
	afx_msg void OnEnChangeEditTimes();
	void AddRandomDot(unsigned char* fm, int nWidth, int nHeight, int nPitch);
	bool CalculateCircle(unsigned char* fm, int nWidth, int nHeight, int nPitch, int& centerX, int& centerY, int& radius);
	afx_msg void OnBnClickedButtonLoadcenter();
	int m_radius;
	afx_msg void OnEnChangeEditCoordinate();
	BOOL m_showGraphics;
};
