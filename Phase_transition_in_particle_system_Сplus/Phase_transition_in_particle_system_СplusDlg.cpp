
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

//#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
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
	, input_model_temperature(100)
	, input_model_particles_width(20)
	, input_time_step(1e-14)
	, input_animation_movement(TRUE)
	, animation_graphics(TRUE)
	, initial_iterations(1000)
	, iteration_end(2000)
	, normalization_step(10)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPhasetransitioninparticlesystemСplusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPH, display_picture);
	DDX_Control(pDX, IDC_GRAPH2, display_picture_2);
	DDX_Control(pDX, IDC_GRAPH3, display_picture_3);
	DDX_Control(pDX, IDC_GRAPH4, display_picture_4);
	DDX_Text(pDX, IDC_EDIT1, input_model_temperature);
	DDX_Text(pDX, IDC_EDIT2, input_model_particles_width);
	DDX_Control(pDX, IDC_EDIT3, out_all_particles);
	DDX_Text(pDX, IDC_EDIT5, input_time_step);
	DDX_Check(pDX, IDC_CHECK1, input_animation_movement);
	DDX_Check(pDX, IDC_CHECK2, animation_graphics);
	DDX_Control(pDX, IDC_EDIT7, number_iterations);
	DDX_Text(pDX, IDC_EDIT6, initial_iterations);
	DDX_Text(pDX, IDC_EDIT8, iteration_end);
	DDX_Text(pDX, IDC_EDIT4, normalization_step);
	DDX_Control(pDX, IDC_EDIT9, output_kinetic);
	DDX_Control(pDX, IDC_EDIT10, output_potential);
	DDX_Control(pDX, IDC_EDIT11, output_total_energy);
	DDX_Control(pDX, IDC_EDIT12, output_enthalpy);
	DDX_Control(pDX, IDC_EDIT13, output_offset_square);
	DDX_Control(pDX, IDC_EDIT14, output_temperature);
}

BEGIN_MESSAGE_MAP(CPhasetransitioninparticlesystemСplusDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPhasetransitioninparticlesystemСplusDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPhasetransitioninparticlesystemСplusDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &CPhasetransitioninparticlesystemСplusDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CPhasetransitioninparticlesystemСplusDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_CHECK1, &CPhasetransitioninparticlesystemСplusDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CPhasetransitioninparticlesystemСplusDlg::OnBnClickedCheck2)
	ON_EN_CHANGE(IDC_EDIT6, &CPhasetransitioninparticlesystemСplusDlg::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT8, &CPhasetransitioninparticlesystemСplusDlg::OnEnChangeEdit8)
	ON_EN_CHANGE(IDC_EDIT4, &CPhasetransitioninparticlesystemСplusDlg::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, &CPhasetransitioninparticlesystemСplusDlg::OnEnChangeEdit5)
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
	drawer_enthalpy.Create(GetDlgItem(IDC_GRAPH4)->GetSafeHwnd());
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
	finishAnimationFlow();
	Particles_and_processing start_particles;

	start_particles.simple_initial_state(
		input_model_particles_width, input_model_temperature,10);

	drawer_particle.drawing_particles(
		start_particles.get_border_line(), start_particles.get_center_particles(), 1e9);
	
	CString str;
	str.Format(L"%d", start_particles.number_of_particles(
		input_model_particles_width));
	out_all_particles.SetWindowText(str);
}


void CPhasetransitioninparticlesystemСplusDlg::OnBnClickedButton2()
{
	startAnimationFlow();
}


void CPhasetransitioninparticlesystemСplusDlg::OnEnChangeEdit1() { UpdateData(TRUE); }
void CPhasetransitioninparticlesystemСplusDlg::OnEnChangeEdit2() { UpdateData(TRUE); }

void CPhasetransitioninparticlesystemСplusDlg::OnBnClickedCheck1() { UpdateData(TRUE); }
void CPhasetransitioninparticlesystemСplusDlg::OnBnClickedCheck4() { UpdateData(TRUE); }
void CPhasetransitioninparticlesystemСplusDlg::OnBnClickedCheck2() { UpdateData(TRUE); }
void CPhasetransitioninparticlesystemСplusDlg::OnBnClickedCheck3() { UpdateData(TRUE); }

void CPhasetransitioninparticlesystemСplusDlg::OnEnChangeEdit6() { UpdateData(TRUE); }
void CPhasetransitioninparticlesystemСplusDlg::OnEnChangeEdit8() { UpdateData(TRUE); }
void CPhasetransitioninparticlesystemСplusDlg::OnEnChangeEdit4() { UpdateData(TRUE); }
void CPhasetransitioninparticlesystemСplusDlg::OnEnChangeEdit5() { UpdateData(TRUE); }