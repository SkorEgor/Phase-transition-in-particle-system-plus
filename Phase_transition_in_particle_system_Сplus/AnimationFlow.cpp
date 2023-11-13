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
	
	//Зададим начальную вероятность
	Particles_and_processing start_particles;
	start_particles.simple_initial_state(
		thisWind->input_model_particles_width,
		thisWind->input_model_temperature,
		thisWind->normalization_step);

	// Отображаем кол-во частиц
	CString str;
	str.Format(L"%d", start_particles.number_of_particles(
		thisWind->input_model_particles_width));
	thisWind->out_all_particles.SetWindowText(str);

	// Очищаем поля вывода
	thisWind->output_kinetic.SetWindowText(L"");
	thisWind->output_potential.SetWindowText(L"");
	thisWind->output_total_energy.SetWindowText(L"");
	thisWind->output_enthalpy.SetWindowText(L"");
	thisWind->output_offset_square.SetWindowText(L"");
	thisWind->output_temperature.SetWindowText(L"");


	int iteration = 0;
	double step_time = thisWind->input_time_step;

	int initial_iterations = thisWind->initial_iterations;
	bool starting_iterations = true;

	int iteration_end = thisWind->iteration_end;
	//int normalization_step = thisWind->normalization_step;

	//Запуск потока
	while (status_th) {
		//---------Работа в потоке---------//
		// Следующее состояние системы
		// start_particles.next_state(start_particles.state, step_time);
		//break;
		start_particles.next_state_2(step_time);
		
		// Анимаиця движения
		if (thisWind->input_animation_movement) {
			thisWind->drawer_particle.drawing_particles(start_particles.get_border_line(),
				start_particles.get_center_particles(), 1e9);
		}

		// Анимация графиков
		if (thisWind->animation_graphics) {
			thisWind->drawer_energy.drawing_energy(
				start_particles.energy_kinetic,
				start_particles.energy_potential,
				start_particles.energy_total
			);
			thisWind->drawer_enthalpy.drawing_enthalpy(
				start_particles.enthalpy_a
			);
			thisWind->bias.drawing_bias(
				start_particles.temperature,
				start_particles.average_square_displacement
			);
		}

		// Конец расчета
		if (iteration >= iteration_end) {
			status_th = false;
		}

		// Выход на равновесное расстояние
		if (iteration >= initial_iterations && starting_iterations) {
			start_particles.leave_last_state();
			starting_iterations = false;
		}


		// Наращивание и вывод итерации
		CString str;
		str.Format(L"%d", iteration);
		thisWind->number_iterations.SetWindowText(str);
		iteration++;
	}

	// ВЫВОД ГРАФИКОВ
	thisWind->drawer_particle.drawing_particles(start_particles.get_border_line(),
		start_particles.get_center_particles(), 1e9);
	thisWind->drawer_energy.drawing_energy(
		start_particles.energy_kinetic,
		start_particles.energy_potential,
		start_particles.energy_total
	);
	thisWind->drawer_enthalpy.drawing_enthalpy(
		start_particles.enthalpy_a
	);
	thisWind->bias.drawing_bias(
		start_particles.temperature,
		start_particles.average_square_displacement
	);

	// ВЫВОД СРЕДНИХ ЗНАЧЕНИЙ
	CString str_out;

	str_out.Format(L"%g", Particles_and_processing::vector_average(
		start_particles.energy_kinetic));
	thisWind->output_kinetic.SetWindowText(str_out);

	str_out.Format(L"%g", Particles_and_processing::vector_average(
		start_particles.energy_potential));
	thisWind->output_potential.SetWindowText(str_out);

	str_out.Format(L"%g", Particles_and_processing::vector_average(
		start_particles.energy_total));
	thisWind->output_total_energy.SetWindowText(str_out);

	str_out.Format(L"%g", Particles_and_processing::vector_average(
		start_particles.enthalpy_a));
	thisWind->output_enthalpy.SetWindowText(str_out);

	str_out.Format(L"%g", Particles_and_processing::vector_average(
		start_particles.enthalpy_a));
	thisWind->output_enthalpy.SetWindowText(str_out);
	
	str_out.Format(L"%g", Particles_and_processing::vector_average(
		start_particles.average_square_displacement));
	thisWind->output_offset_square.SetWindowText(str_out);

	str_out.Format(L"%g", Particles_and_processing::vector_average(
		start_particles.temperature));
	thisWind->output_temperature.SetWindowText(str_out);

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
