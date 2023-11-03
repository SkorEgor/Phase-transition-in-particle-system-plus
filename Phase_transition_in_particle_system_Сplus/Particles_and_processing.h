#pragma once
#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

struct Border
{
	double x_start;
	double x_end;
	double y_start;
	double y_end;
	bool initialized = false;
};

class Vector2D {
public:
	vector<double> x;
	vector<double> y;

	Vector2D(vector<double> in_x, vector<double> in_y) {
		x = in_x;
		y = in_y;
	}
};

class Particle_State {
public:
	vector<double> x, y;
	vector<double> speed_x, speed_y;
	vector<double> force_x, force_y;

	void resize(int& size) {
		x.resize(size);
		y.resize(size);

		speed_x.resize(size);
		speed_y.resize(size);

		force_x.resize(size);
		force_y.resize(size);
	}
};

class Particles_and_processing
{
public:
	const double a = 0.382e-9;		// [ м ] Равновесное растоние
	const double k = 1.38e-23;		// [ Дж/К ] Постоянная Больцмана 
	const double m = 6.63e-26;		// [ кг ] Масса Аргона
	const double e = -1.6e-19;		// [ Кл ] Заряд электрона
	const double D = 0.0103 * e;	// [ Дж ] Модуль потенциальной энергии
	// Константы для вычислний
	const double a_6_power = pow(a, 6);
	const double force_multiplier = 12 * D * a_6_power;

	Border border;	// Граница расчетной ячейки

	vector<double> temperature;	// Темпераура 

	Particle_State state;

	// Возвращает число частиц, по числу частиц в ширину
	int number_of_particles(int number_elements_on_side);

	// Заполнение координат частиц
	vector<double> generate_coordinates_x(
		int& all_particles, int& number_elements_on_side,
		double step, double coordinate_start);
	vector<double> generate_coordinates_y(
		int& all_particles, int& number_elements_on_side,
		double step, double coordinate_start);

	// Заполнение скорости
	void speed_normalization(vector<double>& speed);
	void starting_speed(
		double& temperature, int& all_particles,
		vector<double>& speed_x, vector<double>& speed_y);

	// Расчет Сил
	void force_calculation(Particle_State& in_state,
		double& cell_width, double& cell_height);

	/*   НАЧАЛЬНОЕ СОСТОЯНИЕ   */
	void simple_initial_state(
		int number_elements_on_side,
		double temperature
	);
	
	// Расчет следующего положения шага
	Particle_State next_state(Particle_State& old_state, double& time);

	// Получение данных для отрисовки
	Vector2D get_border_line();
	Vector2D get_center_particles();
};

