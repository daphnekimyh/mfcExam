#include "pch.h"
#include "framework.h"
#include "MfcCImage.h"
#include "MfcCImageDlg.h"
#include "afxdialogex.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);

protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CMfcCImageDlg::CMfcCImageDlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MFCCIMAGE_DIALOG, pParent), m_bDrawOverlay(false)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMfcCImageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMfcCImageDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_ACTION, &CMfcCImageDlg::OnBnClickedBtnAction)
    ON_BN_CLICKED(IDC_BTN_MAKE_SAVE, &CMfcCImageDlg::OnBnClickedBtnMakeSave)
    ON_BN_CLICKED(IDC_BUTTON_LOADCENTER, &CMfcCImageDlg::OnBnClickedButtonLoadcenter)
    ON_EN_CHANGE(IDC_EDIT_TIMES, &CMfcCImageDlg::OnEnChangeEditTimes)
    ON_EN_CHANGE(IDC_EDIT_COORDINATE, &CMfcCImageDlg::OnEnChangeEditCoordinate)
END_MESSAGE_MAP()

BOOL CMfcCImageDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != nullptr)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);
    SetIcon(m_hIcon, FALSE);

    return TRUE;
}

void CMfcCImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

void CMfcCImageDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this);
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

HCURSOR CMfcCImageDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CMfcCImageDlg::OnBnClickedBtnAction()
{
    MoveRect();
}

void CMfcCImageDlg::OnEnChangeEditTimes()
{
    UpdateData(TRUE);
}

void CMfcCImageDlg::OnBnClickedBtnMakeSave()
{
    CString strTimes;
    GetDlgItemText(IDC_EDIT_TIMES, strTimes);
    int nTimes = _ttoi(strTimes);
    if (nTimes <= 0) return;

    srand((unsigned int)time(NULL));

    CRect rect;
    GetClientRect(&rect);
    int nWidth = rect.Width();
    int nHeight = rect.Height();

    int nBpp = 8;

    for (int imageCounter = 1; imageCounter <= nTimes; ++imageCounter) {
        CImage newImage;
        newImage.Create(nWidth, -nHeight, nBpp);
        if (nBpp == 8) {
            static RGBQUAD rgb[256];
            for (int i = 0; i < 256; i++) {
                rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
            }
            newImage.SetColorTable(0, 256, rgb);
        }

        int nPitch = newImage.GetPitch();
        unsigned char* fm = (unsigned char*)newImage.GetBits();
        memset(fm, 0x00, nWidth * nHeight);

        AddRandomDot(fm, nWidth, nHeight, nPitch);

        CString strFile;
        strFile.Format(_T("C:\\Users\\home\\Documents\\GitHub\\Image\\circleImage_%d.bmp"), imageCounter);
        newImage.Save(strFile);

        if (m_image) {
            m_image.Destroy();
        }
        m_image.Attach(newImage.Detach());

        m_bDrawOverlay = false;
        UpdateDisplay();
        Sleep(100);
    }
}

void CMfcCImageDlg::OnBnClickedButtonLoadcenter()
{
    CFileDialog filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ENABLESIZING, _T("Bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*||"), this);
    filedlg.m_ofn.lpstrInitialDir = _T("C:\\Users\\home\\Documents\\GitHub\\Image\\");

    if (filedlg.DoModal() == IDOK) {
        CString strFilePath = filedlg.GetPathName();

        if (!m_image.IsNull()) {
            m_image.Destroy();
        }

        if (SUCCEEDED(m_image.Load(strFilePath))) {
            int nWidth = m_image.GetWidth();
            int nHeight = m_image.GetHeight();
            int nPitch = m_image.GetPitch();
            unsigned char* fm = (unsigned char*)m_image.GetBits();

            int centerX = 0, centerY = 0, radius = 0;
            if (CalculateCircle(fm, nWidth, nHeight, nPitch, centerX, centerY, radius)) {
                CString strMsg;
                strMsg.Format(_T("%d, %d"), centerX, centerY, radius);
                SetDlgItemText(IDC_EDIT_COORDINATE, strMsg);
            }
            else {
                SetDlgItemText(IDC_EDIT_COORDINATE, _T("No circle found"));
            }

            m_bDrawOverlay = true;
            UpdateDisplay();
        }
        else {
            AfxMessageBox(_T("Failed to load image"));
        }
    }
}

void CMfcCImageDlg::UpdateDisplay()
{
    CClientDC dc(this);
    if (!m_image.IsNull()) {
        m_image.Draw(dc, 0, 50);

        if (m_bDrawOverlay) {
            int nWidth = m_image.GetWidth();
            int nHeight = m_image.GetHeight();
            int nPitch = m_image.GetPitch();
            unsigned char* fm = (unsigned char*)m_image.GetBits();

            int centerX = 0, centerY = 0, radius = 0;
            if (CalculateCircle(fm, nWidth, nHeight, nPitch, centerX, centerY, radius)) {
                int start_X = centerX;
                int start_Y = centerY + 50;

                CPen penSolid(PS_SOLID, 1, RGB(255, 0, 0));
                CPen* pOldPen = dc.SelectObject(&penSolid);

                double angleRadians = 45 * (3.14159265358979323846 / 180);
                int end_X = start_X + int(cos(angleRadians) * radius);
                int end_Y = start_Y + int(sin(angleRadians) * radius);

                dc.MoveTo(start_X, start_Y);
                dc.LineTo(end_X, end_Y);

                CPen penDot(PS_DOT, 1, RGB(255, 255, 255));
                dc.SelectObject(&penDot);
                dc.MoveTo(start_X, start_Y);
                dc.LineTo(0, start_Y);

                dc.MoveTo(start_X, start_Y);
                dc.LineTo(start_X, 50);

                COLORREF oldTextColor = dc.SetTextColor(RGB(0, 255, 0));
                int oldBkMode = dc.SetBkMode(TRANSPARENT);
                CString strRadius;
                strRadius.Format(_T("Radius == %d"), radius);
                int text_X = end_X + 50;
                int text_Y = end_Y;

                CPen penToText(PS_SOLID, 1, RGB(0, 255, 0));
                dc.SelectObject(&penToText);
                dc.MoveTo(end_X, end_Y);
                dc.LineTo(text_X, text_Y);

                dc.TextOutW(text_X, text_Y, strRadius);

                dc.SelectObject(pOldPen);
                dc.SetTextColor(oldTextColor);
                dc.SetBkMode(oldBkMode);
            }
        }
    }
}

void CMfcCImageDlg::MoveRect()
{
    if (m_image.IsNull()) {
        return;
    }

    int nGray = rand() % 256;
    int nWidth = m_image.GetWidth();
    int nHeight = m_image.GetHeight();
    int nPitch = m_image.GetPitch();
    unsigned char* fm = (unsigned char*)m_image.GetBits();
    int nRadius = 18;

    int x = rand() % (m_image.GetWidth() - nRadius * 2);
    int y = rand() % (m_image.GetHeight() - nRadius * 2);
    drawCircle(fm, x, y, nRadius, nGray);

    UpdateDisplay();
}

BOOL CMfcCImageDlg::validImgPos(int x, int y)
{
    int nWidth = m_image.GetWidth();
    int nHeight = m_image.GetHeight();

    CRect rect(0, 0, nWidth, nHeight);
    return rect.PtInRect(CPoint(x, y));
}

void CMfcCImageDlg::drawCircle(unsigned char* fm, int x, int y, int nRadius, int nGray)
{
    int nPitch = m_image.GetPitch();
    int nCenterX = x + nRadius;
    int nCenterY = y + nRadius;

    for (int j = y; j < y + nRadius * 2; j++) {
        for (int i = x; i < x + nRadius * 2; i++) {
            if (isInCircle(i, j, nCenterX, nCenterY, nRadius)) {
                if (validImgPos(i, j)) {
                    fm[j * nPitch + i] = nGray;
                }
            }
        }
    }
}

bool CMfcCImageDlg::isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
    bool bRet = false;

    double dx = i - nCenterX;
    double dy = j - nCenterY;
    double dDist = dx * dx + dy * dy;

    if (dDist <= nRadius * nRadius) {
        bRet = true;
    }

    return bRet;
}

void CMfcCImageDlg::AddRandomDot(unsigned char* fm, int nWidth, int nHeight, int nPitch)
{
    int x = rand() % nWidth;
    int y = rand() % nHeight;

    int radius = (rand() % (min(nWidth, nHeight) / 10)) + 30;

    int grayScale = 190 + rand() % 66;

    for (int j = y - radius; j <= y + radius; j++) {
        for (int i = x - radius; i <= x + radius; i++) {
            if (i >= 0 && i < nWidth && j >= 0 && j < nHeight) {
                int dx = i - x;
                int dy = j - y;
                if (dx * dx + dy * dy <= radius * radius) {
                    fm[j * nPitch + i] = grayScale;
                }
            }
        }
    }
}

bool CMfcCImageDlg::CalculateCircle(unsigned char* fm, int nWidth, int nHeight, int nPitch, int& centerX, int& centerY, int& radius)
{
    int sumX = 0, sumY = 0, count = 0;
    int threshold = 200;

    std::vector<std::pair<int, int>> edgePixels;

    for (int y = 1; y < nHeight - 1; ++y) {
        for (int x = 1; x < nWidth - 1; ++x) {
            unsigned char pixel = fm[y * nPitch + x];
            if (pixel < threshold) {
                if (fm[(y - 1) * nPitch + x] >= threshold || fm[(y + 1) * nPitch + x] >= threshold ||
                    fm[y * nPitch + (x - 1)] >= threshold || fm[y * nPitch + (x + 1)] >= threshold) {
                    edgePixels.push_back({ x, y });
                    sumX += x;
                    sumY += y;
                    count++;
                }
            }
        }
    }

    if (count == 0) return false;

    centerX = sumX / count;
    centerY = sumY / count;

    long long sumRadius = 0;
    for (auto& p : edgePixels) {
        int dx = p.first - centerX;
        int dy = p.second - centerY;
        sumRadius += std::sqrt(dx * dx + dy * dy);
    }
    radius = static_cast<int>(sumRadius / count);

    return true;
}

void CMfcCImageDlg::OnEnChangeEditCoordinate()
{
}