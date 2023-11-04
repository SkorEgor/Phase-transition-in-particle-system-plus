
// Phase_transition_in_particle_system_СplusDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Phase_transition_in_particle_system_Сplus.h"
#include "Phase_transition_in_particle_system_СplusDlg.h"
#include "afxdialogex.h"
#include <iostream>
using namespace std;

#include "Particles_and_processing.h"

// Для отладки


#ifdef _DEBUG
#define new DEBUG_NEW

#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
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


// Диалоговое окно CPhasetransitioninparticlesystemСplusDlg



CPhasetransitioninparticlesystemСplusDlg::CPhasetransitioninparticlesystemСplusDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PHASE_TRANSITION_IN_PARTICLE_SYSTEM_PLUS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPhasetransitioninparticlesystemСplusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPH, display_picture);
	DDX_Control(pDX, IDC_GRAPH2, display_picture_2);
	DDX_Control(pDX, IDC_GRAPH3, display_picture_3);
}

BEGIN_MESSAGE_MAP(CPhasetransitioninparticlesystemСplusDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPhasetransitioninparticlesystemСplusDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPhasetransitioninparticlesystemСplusDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Обработчики сообщений CPhasetransitioninparticlesystemСplusDlg

BOOL CPhasetransitioninparticlesystemСplusDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
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

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	drawer_particle.Create(GetDlgItem(IDC_GRAPH)->GetSafeHwnd());
	drawer_energy.Create(GetDlgItem(IDC_GRAPH2)->GetSafeHwnd());
	bias.Create(GetDlgItem(IDC_GRAPH3)->GetSafeHwnd());

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CPhasetransitioninparticlesystemСplusDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CPhasetransitioninparticlesystemСplusDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CPhasetransitioninparticlesystemСplusDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void vector_output(vector<double>& val) {
	for (int i = 0; i < val.size(); i++) {
		cout << val[i] << endl;
	}
}

void CPhasetransitioninparticlesystemСplusDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	Particles_and_processing start_particles;

	start_particles.simple_initial_state(3, 100);
	/*
	cout << "1111111111111111111111111" << endl;
	for (int i = 0; i < start_particles.state.x.size(); i++) {
		cout << start_particles.state.x[i] << "\t\t"
			<< start_particles.state.y[i] << "\t\t"
			<< start_particles.state.speed_x[i] << "\t\t"
			<< start_particles.state.speed_y[i] << "\t\t"
			<< start_particles.state.force_x[i] << "\t\t"
			<< start_particles.state.force_y[i] << endl;
	}
	cout << "222222222222222222222222" << endl;
	/*
	vector <double> x, y;

	int numberFunctionPoints = 20;
	for (int i = -5; i < numberFunctionPoints; i++) {
		x.push_back(i);
		y.push_back(i * 2);
	}

	//drv.Draw(Vector2D(x, y));
	vector_output(start_particles.get_center_particles().x);
	*/
	//drv.Draw(start_particles.get_border_line(), start_particles.get_center_particles(), 1e9);


	// Следующее состояние
	double step_time = 1e-14;
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 10; i++)
			start_particles.next_state(start_particles.state, step_time);
		// Нормировка
		start_particles.all_speed_normalization();
	}


	/*
	for (int i = 0; i < start_particles.state.x.size(); i++) {
		cout << start_particles.state.x[i] << "\t\t"
			<< start_particles.state.y[i] << "\t\t"
			<< start_particles.state.speed_x[i] << "\t\t"
			<< start_particles.state.speed_y[i] << "\t\t"
			<< start_particles.state.force_x[i] << "\t\t"
			<< start_particles.state.force_y[i] << endl;
	}*/
	drawer_particle.drawing_particles(start_particles.get_border_line(), start_particles.get_center_particles(), 1e9);


}


void CPhasetransitioninparticlesystemСplusDlg::OnBnClickedButton2()
{
	startAnimationFlow();
}
