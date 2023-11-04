//1) Подключение файла диалога
#pragma once
#include "pch.h"
#include "framework.h"
#include "Phase_transition_in_particle_system_Сplus.h"
#include "Phase_transition_in_particle_system_СplusDlg.h"
#include "afxdialogex.h"

#include <vector>

using namespace std;


//Определяет работать или нет потоку
bool status_th = false;

//4)Задаем действия в потоке
DWORD WINAPI animationThread(LPVOID t)
{
	//4.1)Создаем объект нашего окна
	CPhasetransitioninparticlesystemСplusDlg* thisWind =
		(CPhasetransitioninparticlesystemСplusDlg*)t;
	
	/*
	//Получаем кол-во отсчетов и ширину ямы
	int N = thisWind->input_number_of_counts;
	double d = thisWind->input_pit_width;
	double step_x = d / N;

	//Задаем барьер
	double a_barrier = thisWind->input_amplituda_barrier;
	double sigma_barrier = thisWind->input_sigma_barrier;

	//Задаем начальное распределение
	double fi_packet=thisWind->input_amplituda_distributions;
	double sigma_packet = thisWind->input_sigma_distributions;

	//Получаем шаг по времени
	double time_step = thisWind->input_time_step;
	*/

	//Зададим начальную вероятность
	Particles_and_processing start_particles;

	start_particles.simple_initial_state(20, 10);

	//Задержка между кадрами
	int delTime = 0;

	int iteration = 1;
	double step_time = 1e-14;
	//Запуск потока
	while (status_th) {
		//---------Работа в потоке---------//
		//4.2)Прописываем необходимые команды в потоке
		for (int i = 0; i < 10; i++) {
			start_particles.next_state(start_particles.state, step_time);
			iteration++;
		}


		if (iteration % 10==0) {
			// Нормировка
			start_particles.all_speed_normalization();
		}
		thisWind->drawer_particle.drawing_particles(start_particles.get_border_line(),
			start_particles.get_center_particles(), 1e9);
		thisWind->drawer_energy.drawing_energy_and_enthalpy(
			start_particles.energy_kinetic,
			start_particles.energy_potential,
			start_particles.energy_total,
			start_particles.enthalpy
		);
		thisWind->bias.drawing_bias(
			start_particles.temperature,
			start_particles.average_square_displacement
		);
		//------Конец работы в потоке------//
		//Sleep(delTime);		//4.3) Указываем задержку между кадрами
		iteration++;
	}

	return 0;
}


//2) Функция создания потока и указываем нужно простарнство
void CPhasetransitioninparticlesystemСplusDlg::startAnimationFlow()
{
	if (status_th) {
		finishAnimationFlow();
		/*
		if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck())choiceParameterResearched('l');
		else if (((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck())choiceParameterResearched('k');
		else if (((CButton*)GetDlgItem(IDC_RADIO3))->GetCheck())choiceParameterResearched('c');
		else if (((CButton*)GetDlgItem(IDC_RADIO4))->GetCheck())choiceParameterResearched('v');
		else if (((CButton*)GetDlgItem(IDC_RADIO5))->GetCheck())choiceParameterResearched('f');
		*/
		return;
	}

	//запуск потока
	status_th = true;
	LPVOID lpParameter = (LPVOID)this;
	HANDLE thread = CreateThread(
		NULL,				//LPSECURITY_ATTRIBUTES lpThreadAttributes, // дескриптор защиты
		0,					//SIZE_T dwStackSize,                       // начальный размер стека
		animationThread,	//LPTHREAD_START_ROUTINE lpStartAddress,    // функция потока
		lpParameter,		//LPVOID lpParameter,                       // параметр потока
		0,					//DWORD dwCreationFlags,                    // опции создания
		NULL				//LPDWORD lpThreadId						// идентификатор потока
	);
	//UpdateData(FALSE);
}

//3) Создаем функцию endAnimationFlow и указываем нужно простарнство
void CPhasetransitioninparticlesystemСplusDlg::finishAnimationFlow()
{
	status_th = false;
	//UpdateData(FALSE);
}
