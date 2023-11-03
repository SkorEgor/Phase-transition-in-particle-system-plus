#include "pch.h"
#include "Particles_and_processing.h"

// ������ ����������� ������
int Particles_and_processing::number_of_particles(
	int number_elements_on_side)
{
	// ����������� �������� �����
	int number_of_odd_lines = int(number_elements_on_side / 2);
	// ����������� ������ �����
	int number_even_lines = number_elements_on_side - number_of_odd_lines;

	return (
		(number_of_odd_lines * (number_elements_on_side - 1)) +
		(number_even_lines * number_elements_on_side)
		);
}

/*   ����������   */
// ������������� �� x
vector<double> Particles_and_processing::generate_coordinates_x(
	int& all_particles, int& number_elements_on_side,
	double step, double coordinate_start = 0)
{
	// ������ ���������
	vector<double> x(all_particles, 0);

	// ����������� �������� �����
	int number_of_odd_lines = int(number_elements_on_side / 2);
	// ����������� ������ �����
	int number_even_lines = number_elements_on_side - number_of_odd_lines;


	// ���������� ������ ������
	for (int line_number = 0;
		line_number < all_particles;
		line_number += 2 * number_elements_on_side - 1) {

		for (int x_i = 0; x_i < number_elements_on_side; x_i++) {
			x[line_number + x_i] = x_i * step + coordinate_start;
		}

	}

	// ���������� �������� ������
	for (int line_number = number_elements_on_side;
		line_number < all_particles;
		line_number += 2 * number_elements_on_side - 1) {

		for (int x_i = 0; x_i < number_elements_on_side - 1; x_i++) {
			x[line_number + x_i] = x_i * step + coordinate_start + step / 2;
		}

	}

	return x;
}

// ������������� �� y
vector<double> Particles_and_processing::generate_coordinates_y(
	int& all_particles, int& number_elements_on_side,
	double step, double coordinate_start = 0)
{
	// ������ ���������
	vector<double> y(all_particles, 0);

	// ����������� �������� �����
	int number_of_odd_lines = int(number_elements_on_side / 2);
	// ����������� ������ �����
	int number_even_lines = number_elements_on_side - number_of_odd_lines;


	// ����������
	int line_number = 0;
	int start_line = 0;
	bool odd = false; // �� ������
	while (start_line < all_particles)
	{
		// ������ ������ ������� �� ��������. �� ������ �� 1 ������
		for (int x_i = 0; x_i < number_elements_on_side - ((int)odd); x_i++) {
			y[start_line + x_i] = line_number * step + coordinate_start;
		}

		// ��������� ������ ���������� � ������� ��������� ���� ������
		start_line += number_elements_on_side - ((int)odd);
		// ������ �������� ��� ��������� ������
		odd = !odd;
		// ������ ����� ������
		line_number += 1;
	}

	return y;
}

/*   ��������   */
// ����������� ������������� [0;1)
double uniform_random_distribution() {
	int n = 12;
	double val = 0.;
	for (int i = 0; i < n; i++) {
		val += rand() % 2;
	}
	return val / n;
}

// ����������
void Particles_and_processing::speed_normalization(vector<double>& speed) {
	int all_particles = speed.size();
	// ����� ���������
	double sum_speed = 0;
	for (int i = 0; i < all_particles; i++) {
		sum_speed += speed[i];
	}
	// ������� 
	double average_speed = sum_speed / all_particles;
	// ����������
	for (int i = 0; i < all_particles; i++) {
		speed[i] -= average_speed;
	}
}

// ������� ��������� x,y
void Particles_and_processing::starting_speed(
	double& temperature, int& all_particles,
	vector<double>& speed_x, vector<double>& speed_y) {
	/*

	mV ^ 2   d
	---- = -*k * Temperature; d - ������� �������
	2    2

	V = sqrt(d * k * T / m) d = 1
	V = sqrt(k * T / m)

	*/

	// ������� ������ ��������
	double speed_module = sqrt((k * temperature) / m);

	// ������ ������� ���������
	// ������� �� ������ ���������
	speed_x.clear();
	speed_y.clear();
	// ������ �����������
	speed_x.resize(all_particles);
	speed_y.resize(all_particles);

	// ������ ��������
	int angle;
	for (int i = 0; i < all_particles; i++) {
		// ��������� ���� [ ��� ], [0;2pi)
		angle = uniform_random_distribution() * 2 * M_PI;
		speed_x[i] = speed_module * cos(angle);
		speed_y[i] = speed_module * sin(angle);
	}

	// ����������
	speed_normalization(speed_x);
	speed_normalization(speed_y);
}

/*   ����   */
void Particles_and_processing::force_calculation(Particle_State& in_state,
	double& cell_width, double& cell_height) {
	if (in_state.x.empty() || in_state.y.empty())
		return;

	int particle_number = in_state.x.size();

	// ������ ������� ���
	// ������� �� ������
	in_state.force_x.clear();
	in_state.force_y.clear();
	// ������ �����������
	in_state.force_x.resize(particle_number);
	in_state.force_y.resize(particle_number);

	// �������� �� �������
	for (int particle_i = 0; particle_i < particle_number; particle_i++) {

		// ���������
		double sum_x = 0;
		double sum_y = 0;
		for (int j = 0; j < particle_number; j++) {
			// ��� ����� ����� ����
			if (particle_i == j)
				continue;

			double difference_x = in_state.x[particle_i] - in_state.x[j];
			if (abs(difference_x) > cell_width / 2)
				difference_x -= cell_width * (difference_x > 0 ? 1 : -1);

			double difference_y = in_state.y[particle_i] - in_state.y[j];
			if (abs(difference_y) > cell_height / 2)
				difference_y -= cell_height * (difference_y > 0 ? 1 : -1);

			double radius_2_power = difference_x * difference_x + difference_y * difference_y;
			double radius_6_power = radius_2_power * radius_2_power * radius_2_power;

			sum_x += (a_6_power / (radius_6_power)-1) *
				(difference_x / (radius_6_power * radius_2_power));
			sum_y += (a_6_power / (radius_6_power)-1) *
				(difference_y / (radius_6_power * radius_2_power));
		}
		in_state.force_x[particle_i] = force_multiplier * sum_x;
		in_state.force_y[particle_i] = force_multiplier * sum_y;
	}
}


/*   ��������� ���������   */
void Particles_and_processing::simple_initial_state(
	int number_elements_on_side,
	double in_temperature
) {
	// ���� �� �����
	double step_x = a;
	double step_y = a * sqrt(3) / 2.;

	// ���-�� ����� ������
	int all_particles = number_of_particles(number_elements_on_side);

	// ������� ������ ��������� ������
	border.x_start = -step_x / 2;
	border.x_end = (number_elements_on_side - 1) * step_x + (step_x / 2);
	border.y_start = -step_y / 2;
	border.y_end = (number_elements_on_side - 1) * step_y + (step_y / 2);
	border.initialized = true;

	// ���������� ��������� ������
	state.x = generate_coordinates_x(
		all_particles, number_elements_on_side, step_x);
	state.y = generate_coordinates_y(
		all_particles, number_elements_on_side, step_y);

	// ������� ���������
	temperature.push_back(in_temperature);
	starting_speed(
		in_temperature, all_particles,
		state.speed_x, state.speed_y);

	// ������ ���
	double cell_width = border.x_end - border.x_start;
	double cell_height = border.y_end - border.y_start;
	force_calculation(state, cell_width, cell_height);

}

void position_in_cell(double& coordinate, double& cell_min, double& cell_max, double& size) {
	if (coordinate < cell_min)
		coordinate += size;
	else if (coordinate > cell_max)
		coordinate -= size;
}

/*   ������ ���������� ���������    */
Particle_State Particles_and_processing::next_state(Particle_State& old_state, double& time) {
	// ������� ����� ���������
	Particle_State new_state;

	if (old_state.x.empty() || old_state.y.empty() ||
		old_state.speed_x.empty() || old_state.speed_y.empty() ||
		old_state.force_x.empty() || old_state.force_y.empty())
		return new_state;

	int particle_number = old_state.x.size(); // ���-� ������

	new_state.resize(particle_number);

	// ������ ����������
	double cell_width = border.x_end - border.x_start;
	double cell_height = border.y_end - border.y_start;
	for (int i = 0; i < particle_number; i++) {
		new_state.x[i] = old_state.x[i] +
			old_state.speed_x[i] * time +
			(old_state.force_x[i] / (2 * m)) * time * time;

		new_state.y[i] = old_state.y[i] +
			old_state.speed_y[i] * time +
			(old_state.force_y[i] / (2 * m)) * time * time;

		// �������� �� ��������� ������� � ������
		position_in_cell(new_state.x[i], border.x_start, border.x_end, cell_width);
		position_in_cell(new_state.y[i], border.y_start, border.y_end, cell_height);
	}

	// ����������� ����
	force_calculation(new_state, cell_width, cell_height);

	// ����������� ��������
	for (int i = 0; i < particle_number; i++) {
		new_state.speed_x[i] = old_state.speed_x[i] +
			((new_state.force_x[i] - old_state.force_x[i] ) / (2 * m)) * time;

		new_state.speed_y[i] = old_state.speed_y[i] +
			((new_state.force_y[i] - old_state.force_y[i]) / (2 * m)) * time;
	}

	state = new_state;
	return new_state;
}

/* ��� ��������� */
// ��������� ����� �������
Vector2D Particles_and_processing::get_border_line() {
	/*
	if (!border.initialized)
		return;
	*/

	vector<double> x = {
		border.x_start, border.x_start,
		border.x_end, border.x_end,
		border.x_start
	};
	vector<double> y = {
		border.y_start, border.y_end,
		border.y_end, border.y_start,
		border.y_start
	};
	return Vector2D(x, y);
}

// ��������� ������� ��������� ������� ������
Vector2D Particles_and_processing::get_center_particles() {
	return Vector2D(state.x, state.y);
}