
// Phase_transition_in_particle_system_Сplus.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CPhasetransitionsApp:
// Сведения о реализации этого класса: Phase_transition_in_particle_system_Сplus.cpp
//

class CPhasetransitionsApp : public CWinApp
{
public:
	CPhasetransitionsApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CPhasetransitionsApp theApp;
