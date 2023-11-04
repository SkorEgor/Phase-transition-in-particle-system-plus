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
	// Физ. константы
	const double a = 0.382e-9;		// [ м ] Равновесное растоние
	const double k = 1.38e-23;		// [ Дж/К ] Постоянная Больцмана 
	const double m = 6.63e-26;		// [ кг ] Масса Аргона
	const double e = 1.6e-19;		// [ Кл ] Заряд электрона
	const double D = 0.0103 * e;	// [ Дж ] Модуль потенциальной энергии
	// Константы для вычислний
	const double a_6_power = pow(a, 6);
	const double force_multiplier = 12 * D * a_6_power;
	// - Для расчета потенциальной энергии
	const double sigma = a / pow(2, 1. / 6.); 
	const double sigma_6_power = pow(sigma, 6);
	const double r_1 = 1.2 * a;
	const double r_2 = 1.8 * a;

	// Граница расчетной ячейки
	Border border;	

	// Характеристики системы
	vector<double> energy_kinetic, energy_potential, energy_total; // Энергии
	vector<double> enthalpy;	// Энтальпия
	vector<double> temperature;	// Темпераура 
	vector<double> average_square_displacement = {0}; // Средний квадрат смещения R^2


	// Состояние системы: координты, скорости, силы
	Particle_State state;

private:
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
	double force_calculation(Particle_State& in_state,
		double& cell_width, double& cell_height);
	// V^2 средняя
	double square_average_speed(vector<double>& speed_x, vector<double>& speed_y);
	// Энегрии
	double kinetic_energy_of_system(
		vector<double>& speed_x, vector<double>& speed_y);
	double kinetic_energy_of_system(double& square_average_speed);
	// PV
	double calculation_PV(double& temperature);
	// T
	double temperature_calculation(
		vector<double>& speed_x, vector<double>& speed_y);
	double temperature_calculation(double& square_average_speed);
	// R^2
	double average_square_displacement_calculation(
		vector<double>& x_old, vector<double>& y_old,
		vector<double>& x_new, vector<double>& y_new);
	double average_square_displacement_calculation(
		Particle_State state_old,
		Particle_State state_new);

public:
	// Начальное состояние
	void simple_initial_state(
		int number_elements_on_side,
		double temperature
	);
	
	// Следующее положение системы
	void next_state(Particle_State& old_state, double& time);

	// Нормировка
	void all_speed_normalization();

	// Получение данных для отрисовки
	Vector2D get_border_line();
	Vector2D get_center_particles();
};

