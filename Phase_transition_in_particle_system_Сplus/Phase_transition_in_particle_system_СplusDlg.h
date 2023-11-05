#include "Drawer.h"
// Phase_transition_in_particle_system_СplusDlg.h: файл заголовка
//

#pragma once


// Диалоговое окно CPhasetransitioninparticlesystemСplusDlg
class CPhasetransitioninparticlesystemСplusDlg : public CDialogEx
{
// Создание
public:
	CPhasetransitioninparticlesystemСplusDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHASE_TRANSITION_IN_PARTICLE_SYSTEM_PLUS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Drawer drawer_particle;
	Drawer drawer_energy;
	Drawer drawer_enthalpy;
	Drawer bias;
	void startAnimationFlow();
	void finishAnimationFlow();
	afx_msg void OnBnClickedButton1();
	CStatic display_picture;
	afx_msg void OnBnClickedButton2();
	CStatic display_picture_2;
	CStatic display_picture_3;
	CStatic display_picture_4;
};
