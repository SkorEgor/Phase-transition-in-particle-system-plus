#pragma once

#include <afxwin.h>
#include <vector>
#include "Particles_and_processing.h"
#include <iostream>
#include <algorithm>

using namespace std;

class GraphicsParameters
{
public:
	int padding;
	int left_keys_padding;
	int bottom_keys_padding;

	int actual_width;
	int actual_height;

	int actual_top;
	int actual_bottom;
	int actual_left;
	int actual_right;

	double data_x_min;
	double data_x_max;
	double data_y_min;
	double data_y_max;

	GraphicsParameters(
		int frame_height, int frame_width,
		int in_padding = 20, int in_left_keys_padding = 20, int in_bottom_keys_padding = 10) {

		padding = in_padding;
		left_keys_padding = in_left_keys_padding;
		bottom_keys_padding = in_bottom_keys_padding;

		actual_width = frame_width - 2 * padding - left_keys_padding;
		actual_height = frame_height - 2 * padding - bottom_keys_padding;

		actual_top = padding;
		actual_bottom = actual_top + actual_height;
		actual_left = padding + left_keys_padding;
		actual_right = actual_left + actual_width;
	}
};


class Drawer
{
	// Прямоугольник области рисования.
	CRect frame;
	// Указатель на объект окна, области рисования.
	CWnd* wnd;
	// Контекст рисования, привязанный к области рисования.
	CDC* dc;
	// Контекст рисования, привязанный к памяти.
	CDC memDC;
	// Память для контекста рисования memDC.
	CBitmap bmp;

	// Инструменты отрисовки
	CPen subgrid_pen;
	CPen grid_pen;
	CPen data_pen;
	CPen data_pen2;
	CPen pen_red;
	CPen pen_green;
	CFont font;

	// Флаг для отслеживания состояния инициализации класса.
	bool init;

public:
	// Проинициализировать объект класса на основе HWND.
	void Create(HWND hWnd)
	{
		// Получаем указатель на окно.
		wnd = CWnd::FromHandle(hWnd);
		// Получаем прямоугольник окна.
		wnd->GetClientRect(frame);
		// Получаем контекст для рисования в этом окне.
		dc = wnd->GetDC();

		// Создаем буфер-контекст.
		memDC.CreateCompatibleDC(dc);
		// Создаем растр для контекста рисования.
		bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
		// Выбираем растр для использования буфер-контекстом.
		memDC.SelectObject(&bmp);
		init = true;

		// Задание инструментов отрисовки
		subgrid_pen.CreatePen(PS_DOT, 1, RGB(200, 200, 200));
		grid_pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		data_pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		data_pen2.CreatePen(PS_SOLID, 2, RGB(38, 0, 255));
		pen_red.CreatePen(PS_SOLID, 2, RGB(178, 34, 34));
		pen_green.CreatePen(PS_SOLID, 2, RGB(0, 128, 0));

		font.CreateFontW(18, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// Курсив
			FALSE,				// Подчеркнутый
			FALSE,				// Перечеркнутый
			DEFAULT_CHARSET,	// Набор символов
			OUT_OUTLINE_PRECIS,	// Точность соответствия.	
			CLIP_DEFAULT_PRECIS,//  
			CLEARTYPE_QUALITY,	// Качество
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
		);
	}

	void drawing_particles(Vector2D& border_line, Vector2D& center_particles, double unit_multiplier = 1)
	{
		if (!init) return;

		// Параметры области отрисовки
		GraphicsParameters graph_param(frame.Height(), frame.Width());

		// Белый фон.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));

		// Рисуем сетку и подсетку.
		grid(memDC, graph_param);


		/*  Отрисовка Границы рассчетной ячейки  */
		if (border_line.x.empty()) return;

		// НАЙДЕМ МАКС / МИН ДАННЫХ
		double data_y_max(max_vector(border_line.y)), data_y_min(min_vector(border_line.y));
		double data_x_max(max_vector(border_line.x)), data_x_min(min_vector(border_line.x));

		// Конвектируем величины в величины элемента отрисовки
		vector<double> y = convert_range(border_line.y, graph_param.actual_top, graph_param.actual_bottom, data_y_max, data_y_min);
		vector<double> x = convert_range(border_line.x, graph_param.actual_right, graph_param.actual_left, data_x_max, data_x_min);

		// Рисуем линию границы
		CPen data_pen_red(PS_SOLID, 2, RGB(255, 0, 0));
		draw_line(memDC, x, y, data_pen_red);

		/*   Отрисовка кружочков  */
		CPen pen_green(PS_SOLID, 2, RGB(0, 128, 0));
		memDC.SelectObject(&pen_green);
		CBrush brush;
		brush.Attach(GetStockObject(NULL_BRUSH));
		memDC.SelectObject(brush);

		vector<double> y_particles = convert_range(center_particles.y, graph_param.actual_top, graph_param.actual_bottom, data_y_max, data_y_min);
		vector<double> x_particles = convert_range(center_particles.x, graph_param.actual_right, graph_param.actual_left, data_x_max, data_x_min);

		vector<double> data_radius = { ((double)0.382e-9)/2 };

		vector<double> y_radius = convert_range(data_radius, graph_param.actual_height, 0, data_y_max - data_y_min, 0);
		vector<double> x_radius = convert_range(data_radius, graph_param.actual_width, 0, data_x_max - data_x_min, 0);

		for (unsigned int i = 0; i < y_particles.size(); i++)
		{
			draw_ellipse(memDC,
				x_particles[i], y_particles[i],
				x_radius[0], y_radius[0]);
		}

		// Подписываем оси
		axis_signature(
			memDC, graph_param,
			data_x_min, data_x_max,
			data_y_min, data_y_max,
			unit_multiplier);

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}

	void drawing_energy(vector<double>& energy_kinetic, vector<double>& energy_potential, vector<double>& energy_total)
	{
		if (!init) return;

		// Параметры области отрисовки
		GraphicsParameters graph_param(frame.Height(), frame.Width());

		// Белый фон.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));

		// Рисуем сетку и подсетку.
		grid(memDC, graph_param);


		/*  Отрисовка Границы рассчетной ячейки  */
		if (energy_kinetic.empty()) return;

		// ПО ОСИ x - индексы, создадим данные
		vector<double> data_x(energy_kinetic.size());
		for (int i = 0; i < data_x.size(); i++)
			data_x[i] = i;

		// НАЙДЕМ МАКС / МИН ДАННЫХ

		vector<double> y_max_vector = { max_vector(energy_kinetic), max_vector(energy_potential) , max_vector(energy_total) };
		vector<double> y_min_vector = { min_vector(energy_kinetic), min_vector(energy_potential) , min_vector(energy_total) };


		double data_y_max = max_vector(y_max_vector);
		double data_y_min = min_vector(y_min_vector);
		double data_x_max(data_x.size()-1), data_x_min(0.0);

		// Конвектируем величины в величины элемента отрисовки
		vector<double> kinetic = convert_range(energy_kinetic, graph_param.actual_top, graph_param.actual_bottom, data_y_max, data_y_min);
		vector<double> potential = convert_range(energy_potential, graph_param.actual_top, graph_param.actual_bottom, data_y_max, data_y_min);
		vector<double> total = convert_range(energy_total, graph_param.actual_top, graph_param.actual_bottom, data_y_max, data_y_min);
		vector<double> x = convert_range(data_x, graph_param.actual_right, graph_param.actual_left, data_x_max, data_x_min);

		// Выбираем ручки
		CPen data_pen_green(PS_SOLID, 4, RGB(0, 128, 0));
		CPen data_pen_blue(PS_SOLID, 2, RGB(38, 0, 255));
		CPen data_pen_red(PS_SOLID, 2, RGB(255, 0, 0));


		// Рисуем графики
		draw_line(memDC, x, total, data_pen_green);
		draw_line(memDC, x, potential, data_pen_blue);
		draw_line(memDC, x, kinetic, data_pen_red);
		

		// Подписываем оси
		double unit_multiplier = 1e23;
		axis_signature(
			memDC, graph_param,
			data_x_min, data_x_max,
			data_y_min, data_y_max,
			unit_multiplier, true);

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}

	void drawing_bias(vector<double>& data_x, vector<double>& data_y)
	{
		if (!init) return;

		// Параметры области отрисовки
		GraphicsParameters graph_param(frame.Height(), frame.Width());

		// Белый фон.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));

		// Рисуем сетку и подсетку.
		grid(memDC, graph_param);


		/*  Отрисовка Границы рассчетной ячейки  */
		if (data_x.empty()) return;


		// НАЙДЕМ МАКС / МИН ДАННЫХ

		// НАЙДЕМ МАКС / МИН ДАННЫХ
		double data_y_max(max_vector(data_y)), data_y_min(min_vector(data_y));
		double data_x_max(max_vector(data_x)), data_x_min(min_vector(data_x));

		// Конвектируем величины в величины элемента отрисовки
		vector<double> y = convert_range(data_y, graph_param.actual_top, graph_param.actual_bottom, data_y_max, data_y_min);
		vector<double> x = convert_range(data_x, graph_param.actual_right, graph_param.actual_left, data_x_max, data_x_min);

		// Выбираем ручки
		CPen data_pen_red(PS_SOLID, 2, RGB(255, 0, 0));

		// Рисуем графики
		draw_line(memDC, x, y, data_pen_red);

		// Подписываем оси
		double unit_multiplier = 1e28;
		axis_signature(
			memDC, graph_param,
			data_x_min, data_x_max,
			data_y_min, data_y_max,
			unit_multiplier, true);

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}

	void drawing_energy_and_enthalpy(
		vector<double>& energy_kinetic, vector<double>& energy_potential,
		vector<double>& energy_total, vector<double>& data_enthalpy)
	{
		if (!init) return;

		// Параметры области отрисовки
		GraphicsParameters graph_param(frame.Height(), frame.Width());

		// Белый фон.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));

		// Рисуем сетку и подсетку.
		grid(memDC, graph_param);


		/*  Отрисовка Границы рассчетной ячейки  */
		if (energy_kinetic.empty()) return;

		// ПО ОСИ x - индексы, создадим данные
		vector<double> data_x(energy_kinetic.size());
		for (int i = 0; i < data_x.size(); i++)
			data_x[i] = i;

		// НАЙДЕМ МАКС / МИН ДАННЫХ

		vector<double> y_max_vector = { max_vector(energy_kinetic), max_vector(energy_potential) , max_vector(energy_total) , max_vector(data_enthalpy) };
		vector<double> y_min_vector = { min_vector(energy_kinetic), min_vector(energy_potential) , min_vector(energy_total) , min_vector(data_enthalpy) };


		double data_y_max = max_vector(y_max_vector);
		double data_y_min = min_vector(y_min_vector);
		double data_x_max(data_x.size() - 1), data_x_min(0.0);

		// Конвектируем величины в величины элемента отрисовки
		vector<double> kinetic = convert_range(energy_kinetic, graph_param.actual_top, graph_param.actual_bottom, data_y_max, data_y_min);
		vector<double> potential = convert_range(energy_potential, graph_param.actual_top, graph_param.actual_bottom, data_y_max, data_y_min);
		vector<double> total = convert_range(energy_total, graph_param.actual_top, graph_param.actual_bottom, data_y_max, data_y_min);

		vector<double> enthalpy = convert_range(data_enthalpy, graph_param.actual_top, graph_param.actual_bottom, data_y_max, data_y_min);

		vector<double> x = convert_range(data_x, graph_param.actual_right, graph_param.actual_left, data_x_max, data_x_min);

		// Выбираем ручки
		CPen data_pen_red(PS_SOLID, 2, RGB(255, 0, 0));
		CPen data_pen_blue(PS_SOLID, 2, RGB(38, 0, 255));
		CPen data_pen_green(PS_SOLID, 2, RGB(0, 128, 0));

		CPen data_pen_orange(PS_SOLID, 4, RGB(255, 165, 0));

		// Рисуем графики
		draw_line(memDC, x, kinetic, data_pen_red);
		draw_line(memDC, x, potential, data_pen_blue);
		draw_line(memDC, x, total, data_pen_green);

		draw_line(memDC, x, enthalpy, data_pen_orange);

		// Подписываем оси
		double unit_multiplier = 1e23;
		axis_signature(
			memDC, graph_param,
			data_x_min, data_x_max,
			data_y_min, data_y_max,
			unit_multiplier, true);

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}

	void drawing_enthalpy(
		vector<double>& data_enthalpy_a)
	{
		if (!init) return;

		// Параметры области отрисовки
		GraphicsParameters graph_param(frame.Height(), frame.Width());

		// Белый фон.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));

		// Рисуем сетку и подсетку.
		grid(memDC, graph_param);


		/*  Отрисовка Границы рассчетной ячейки  */
		if (data_enthalpy_a.empty()) return;

		// ПО ОСИ x - индексы, создадим данные
		vector<double> data_x(data_enthalpy_a.size());
		for (int i = 0; i < data_x.size(); i++)
			data_x[i] = i;

		// НАЙДЕМ МАКС / МИН ДАННЫХ

		vector<double> y_max_vector = { max_vector(data_enthalpy_a)};
		vector<double> y_min_vector = { min_vector(data_enthalpy_a) };


		double data_y_max = max_vector(y_max_vector);
		double data_y_min = min_vector(y_min_vector);
		double data_x_max(data_x.size() - 1), data_x_min(0.0);

		// Конвектируем величины в величины элемента отрисовки
		vector<double> enthalpy_a = convert_range(data_enthalpy_a, graph_param.actual_top, graph_param.actual_bottom, data_y_max, data_y_min);
		//vector<double> enthalpy_b = (!data_enthalpy_a.empty()) vector<double> enthalpy_b = convert_range(data_enthalpy_b, graph_param.actual_top, graph_param.actual_bottom, data_y_max, data_y_min);

		vector<double> x = convert_range(data_x, graph_param.actual_right, graph_param.actual_left, data_x_max, data_x_min);

		// Выбираем ручки
		CPen data_pen_red(PS_SOLID, 2, RGB(255, 0, 0));
		CPen data_pen_blue(PS_SOLID, 2, RGB(38, 0, 255));
		CPen data_pen_green(PS_SOLID, 2, RGB(0, 128, 0));

		CPen data_pen_orange(PS_SOLID, 4, RGB(255, 165, 0));

		// Рисуем графики
		draw_line(memDC, x, enthalpy_a, data_pen_red);
		//if (!data_enthalpy_a.empty()) draw_line(memDC, x, enthalpy_b, data_pen_blue);

		// Подписываем оси
		double unit_multiplier = 1;
		axis_signature(
			memDC, graph_param,
			data_x_min, data_x_max,
			data_y_min, data_y_max,
			unit_multiplier, true);

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}

	// Рисует сетку координат
	void grid(CDC& memDC, GraphicsParameters& param, unsigned int grid_size = 10) {
		// Подсетка - пунктирная
		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
		memDC.SelectObject(&subgrid_pen);

		for (double i = 0.5; i < grid_size; i += 1.0)
		{
			memDC.MoveTo(param.actual_left + i * param.actual_width / grid_size, param.actual_top);
			memDC.LineTo(param.actual_left + i * param.actual_width / grid_size, param.actual_bottom);
			memDC.MoveTo(param.actual_left, param.actual_top + i * param.actual_height / grid_size);
			memDC.LineTo(param.actual_right, param.actual_top + i * param.actual_height / grid_size);
		}
		// Сетка - сплошная
		CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
		memDC.SelectObject(&grid_pen);

		for (double i = 0.0; i < grid_size + 1; i += 1.0)
		{
			memDC.MoveTo(param.actual_left + i * param.actual_width / grid_size, param.actual_top);
			memDC.LineTo(param.actual_left + i * param.actual_width / grid_size, param.actual_bottom);
			memDC.MoveTo(param.actual_left, param.actual_top + i * param.actual_height / grid_size);
			memDC.LineTo(param.actual_right, param.actual_top + i * param.actual_height / grid_size);
		}
	}

	// Подпись осей
	void axis_signature(
		CDC& memDC, GraphicsParameters& param,
		double data_x_min, double data_x_max,
		double data_y_min, double data_y_max,
		double& unit_multiplier,
		bool ignore_multiplier_x = false,
		unsigned int grid_size = 10) {

		CFont font;
		font.CreateFontW(18, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// Курсив
			FALSE,				// Подчеркнутый
			FALSE,				// Перечеркнутый
			DEFAULT_CHARSET,	// Набор символов
			OUT_OUTLINE_PRECIS,	// Точность соответствия.	
			CLIP_DEFAULT_PRECIS,//  
			CLEARTYPE_QUALITY,	// Качество
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
		);
		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));
		for (int i = 0; i < grid_size / 2 + 1; i++)
		{
			CString str;
			str.Format((ignore_multiplier_x ? L"%.0f" : L"%.3f"), (ignore_multiplier_x?1: unit_multiplier)* (data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2)));
			memDC.TextOutW(
				param.actual_left + (double)i * param.actual_width / (grid_size / 2) - param.bottom_keys_padding,
				param.actual_bottom + param.bottom_keys_padding / 2, str);

			str.Format(L"%.3f", unit_multiplier * (data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2)));
			memDC.TextOutW(
				param.actual_left - 1.5 * param.left_keys_padding,
				param.actual_bottom - (double)i * param.actual_height / (grid_size / 2) - param.bottom_keys_padding, str);
		}
	}

	double max_vector(vector<double>& data) {
		return *max_element(data.begin(), data.end());
	}
	double min_vector(vector<double>& data) {
		return *min_element(data.begin(), data.end());
	}

	void draw_ellipse(CDC& memDC,
		double& x_0, double& y_0,
		double x_radius, double y_radius) {
		memDC.Ellipse(x_0 - x_radius, y_0 - y_radius, x_0 + x_radius, y_0 + y_radius);
	}

	void draw_line(CDC& memDC,
		vector<double>& x, vector<double>& y, 
		CPen& data_pen) {

		memDC.SelectObject(&data_pen);
		memDC.MoveTo(x[0], y[0]);
		for (unsigned int i = 0; i < y.size(); i++)
		{
			memDC.LineTo(x[i], y[i]);
		}
	}


	void Draw2(vector<double>& data, double min_data, double max_data,
		vector<double>& keys, double min_keys, double max_keys,
		char color,
		vector<double>& data2,
		vector<double>& keys2,
		char color2
	)
	{
		if (!init) return;

		CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
		CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
		CPen data_pen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen data_pen2(PS_SOLID, 2, RGB(38, 0, 255));
		CPen pen_red(PS_SOLID, 2, RGB(178, 34, 34));
		CPen pen_green(PS_SOLID, 2, RGB(0, 128, 0));
		CFont font;
		font.CreateFontW(18, 0, 0, 0,
			FW_DONTCARE,
			FALSE,				// Êóðñèâ
			FALSE,				// Ïîä÷åðêíóòûé
			FALSE,				// Ïåðå÷åðêíóòûé
			DEFAULT_CHARSET,	// Íàáîð ñèìâîëîâ
			OUT_OUTLINE_PRECIS,	// Òî÷íîñòü ñîîòâåòñòâèÿ.	
			CLIP_DEFAULT_PRECIS,//  
			CLEARTYPE_QUALITY,	// Êà÷åñòâî
			VARIABLE_PITCH,		//
			TEXT("Times New Roman")		//
		);

		int padding = 20;
		int left_keys_padding = 20;
		int bottom_keys_padding = 10;

		int actual_width = frame.Width() - 2 * padding - left_keys_padding;
		int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

		int actual_top = padding;
		int actual_bottom = actual_top + actual_height;
		int actual_left = padding + left_keys_padding;
		int actual_right = actual_left + actual_width;

		// Áåëûé ôîí.
		memDC.FillSolidRect(frame, RGB(255, 255, 255));

		// Ðèñóåì ñåòêó è ïîäñåòêó.
		unsigned int grid_size = 10;

		memDC.SelectObject(&subgrid_pen);

		for (double i = 0.5; i < grid_size; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		memDC.SelectObject(&grid_pen);

		for (double i = 0.0; i < grid_size + 1; i += 1.0)
		{
			memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
			memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
			memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
			memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
		}

		// Ðèñóåì ãðàôèê.
		if (data.empty()) return;

		if (keys.size() != data.size())
		{
			keys.resize(data.size());
			for (int i = 0; i < keys.size(); i++)
			{
				keys[i] = i;
			}
		}

		/*ÐÈÑÓÅÌ ÏÅÐÂÛÉ ÃÐÀÔÈÊ*/
		if (color == 'R')memDC.SelectObject(&pen_red);
		else if (color == 'G')memDC.SelectObject(&pen_green);
		else memDC.SelectObject(&data_pen);

		double data_y_max(max_data), data_y_min(min_data);
		double data_x_max(max_keys), data_x_min(min_keys);

		vector<double> y = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);
		vector<double> x = convert_range(keys, actual_right, actual_left, data_x_max, data_x_min);

		memDC.MoveTo(x[0], y[0]);
		for (unsigned int i = 0; i < y.size(); i++)
		{
			memDC.LineTo(x[i], y[i]);
		}
		/*ÐÈÑÓÅÌ ÂÒÎÐÎÉ ÃÐÀÔÈÊ*/
		if (color2 == 'R')memDC.SelectObject(&pen_red);
		else if (color2 == 'G')memDC.SelectObject(&pen_green);
		else memDC.SelectObject(&data_pen);

		y = convert_range(data2, actual_top, actual_bottom, data_y_max, data_y_min);
		x = convert_range(keys2, actual_right, actual_left, data_x_max, data_x_min);

		memDC.MoveTo(x[0], y[0]);
		for (unsigned int i = 0; i < y.size(); i++)
		{
			memDC.LineTo(x[i], y[i]);
		}
		/**/

		memDC.SelectObject(&font);
		memDC.SetTextColor(RGB(0, 0, 0));
		for (int i = 0; i < grid_size / 2 + 1; i++)
		{
			CString str;
			str.Format(L"%.1f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
			memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

			str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
			memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
		}

		dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
	}

	vector<double> convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin)
	{
		vector<double> output = data;
		double k = (outmax - outmin) / (inmax - inmin);
		for (auto& item : output)
		{
			item = (item - inmin) * k + outmin;
		}

		return output;
	}
};