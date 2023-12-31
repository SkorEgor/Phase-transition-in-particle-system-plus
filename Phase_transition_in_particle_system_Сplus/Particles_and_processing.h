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
	// ���. ���������
	const double a = 0.382e-9;		// [ � ] ����������� ��������
	const double k = 1.38e-23;		// [ ��/� ] ���������� ��������� 
	const double m = 6.63e-26;		// [ �� ] ����� ������
	const double e = 1.6e-19;		// [ �� ] ����� ���������
	const double D = 0.0103 * e;	// [ �� ] ������ ������������� �������
	// ��������� ��� ���������
	const double a_6_power = pow(a, 6);
	const double force_multiplier = 12 * D * a_6_power;
	// - ��� ������� ������������� �������
	const double sigma = a / pow(2, 1. / 6.); 
	const double sigma_6_power = pow(sigma, 6);
	const double r_1 = 1.2 * a;
	const double r_2 = 1.8 * a;

	// ������� ��������� ������
	Border border;	

	// ����� ����� ������������
	double true_temperature;
	int iteration = 1;
	int normalization_step = 1000;
	double offset_spped = 0;
	int count_sum = 1;

	// �������������� �������
	vector<double> energy_kinetic, energy_potential, energy_total; // �������
	vector<double> temperature;	// ���������� 
	vector<double> average_square_displacement = {0}; // ������� ������� �������� R^2
	
	// ��������� A
	vector<double> enthalpy_a;	
	double all_time = 0;
	double flux_x = 0;
	double flux_y = 0;
	// ��������� B
	vector<double> enthalpy_b;

	// ��������� �������: ���������, ��������, ����
	Particle_State state;

private:
	// ���������� ��������� ������
	vector<double> generate_coordinates_x(
		int& all_particles, int& number_elements_on_side,
		double step, double coordinate_start);
	vector<double> generate_coordinates_y(
		int& all_particles, int& number_elements_on_side,
		double step, double coordinate_start);

	// ���������� ��������
	void zero_impuls(vector<double>& speed);
	void starting_speed(
		double& temperature, int& all_particles,
		vector<double>& speed_x, vector<double>& speed_y);

	// ������ ���
	double force_calculation(Particle_State& in_state,
		double& cell_width, double& cell_height);
	// V^2 �������
	double square_average_speed(vector<double>& speed_x, vector<double>& speed_y);
	// �������
	double kinetic_energy_of_system(
		vector<double>& speed_x, vector<double>& speed_y);
	double kinetic_energy_of_system(double& square_average_speed);
	// PV ����� �������
	double calculation_PV(double& temperature);
	double calculation_PV2(double& temperature,
		Particle_State& old_state, Particle_State& new_state);
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
	// ���������� ����� ������, �� ����� ������ � ������
	int number_of_particles(int number_elements_on_side);

	// ��������� ���������
	void simple_initial_state(
		int number_elements_on_side,
		double temperature,
		int in_normalization_step
	);
	
	// ��������� ��������� �������
	void next_state(Particle_State& old_state, double& time);
	void next_state_2(double& time);

	// ����������
	void all_speed_normalization();

	// ������� �������� �������
	static double vector_average(vector<double>& mass);

	// ��������� ������ ��� ���������
	Vector2D get_border_line();
	Vector2D get_center_particles();

	// ��� ����� �������� ���������� ������� �� ���������� ��������
	// ������������ ����� ������ �� ����������� ���������
	void leave_last_state();
};

