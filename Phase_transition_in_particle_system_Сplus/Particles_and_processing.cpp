#include "pch.h"
#include "Particles_and_processing.h"

// Расчет колличества частиц
int Particles_and_processing::number_of_particles(
	int number_elements_on_side)
{
	// Колличество нечетных строк
	int number_of_odd_lines = int(number_elements_on_side / 2);
	// Колличество четных строк
	int number_even_lines = number_elements_on_side - number_of_odd_lines;

	return (
		(number_of_odd_lines * (number_elements_on_side - 1)) +
		(number_even_lines * number_elements_on_side)
		);
}

/*   КООРДИНАТЫ   */
// Генерирование по x
vector<double> Particles_and_processing::generate_coordinates_x(
	int& all_particles, int& number_elements_on_side,
	double step, double coordinate_start = 0)
{
	// Вектор координат
	vector<double> x(all_particles, 0);

	// Колличество нечетных строк
	int number_of_odd_lines = int(number_elements_on_side / 2);
	// Колличество четных строк
	int number_even_lines = number_elements_on_side - number_of_odd_lines;


	// Заполнение четных строка
	for (int line_number = 0;
		line_number < all_particles;
		line_number += 2 * number_elements_on_side - 1) {

		for (int x_i = 0; x_i < number_elements_on_side; x_i++) {
			x[line_number + x_i] = x_i * step + coordinate_start;
		}

	}

	// Заполнение нечетных строка
	for (int line_number = number_elements_on_side;
		line_number < all_particles;
		line_number += 2 * number_elements_on_side - 1) {

		for (int x_i = 0; x_i < number_elements_on_side - 1; x_i++) {
			x[line_number + x_i] = x_i * step + coordinate_start + step / 2;
		}

	}

	return x;
}

// Генерирование по y
vector<double> Particles_and_processing::generate_coordinates_y(
	int& all_particles, int& number_elements_on_side,
	double step, double coordinate_start = 0)
{
	// Вектор координат
	vector<double> y(all_particles, 0);

	// Колличество нечетных строк
	int number_of_odd_lines = int(number_elements_on_side / 2);
	// Колличество четных строк
	int number_even_lines = number_elements_on_side - number_of_odd_lines;


	// Заполнение
	int line_number = 0;
	int start_line = 0;
	bool odd = false; // не четная
	while (start_line < all_particles)
	{
		// Размер строки зависиь от четности. Не четные на 1 меньше
		for (int x_i = 0; x_i < number_elements_on_side - ((int)odd); x_i++) {
			y[start_line + x_i] = line_number * step + coordinate_start;
		}

		// Следующая строка начинается с индекса окончания этой строки
		start_line += number_elements_on_side - ((int)odd);
		// Меняем четность для следующей строки
		odd = !odd;
		// Меняем номер строки
		line_number += 1;
	}

	return y;
}

/*   СКОРОСТИ   */
// Равномерное распределение [0;1)
double uniform_random_distribution() {
	int n = 12;
	double val = 0.;
	for (int i = 0; i < n; i++) {
		val += rand() % 2;
	}
	return val / n;
}

// Нормировка
void Particles_and_processing::speed_normalization(vector<double>& speed) {
	int all_particles = speed.size();
	// Сумма скоростей
	double sum_speed = 0;
	for (int i = 0; i < all_particles; i++) {
		sum_speed += speed[i];
	}
	// Средняя 
	double average_speed = sum_speed / all_particles;
	// Нормировка
	for (int i = 0; i < all_particles; i++) {
		speed[i] -= average_speed;
	}
}

void Particles_and_processing::all_speed_normalization() {
	speed_normalization(state.speed_x);
	speed_normalization(state.speed_y);
}

// Задание скоростей x,y
void Particles_and_processing::starting_speed(
	double& temperature, int& all_particles,
	vector<double>& speed_x, vector<double>& speed_y) {
	/*

	mV ^ 2   d
	------ = - * k * Temperature; d - степень свободы
	   2     2

	V = sqrt(d * k * T / m) d = 1
	V = sqrt(k * T / m)

	*/

	// Находим модуль скорости
	double speed_module = sqrt((k * temperature) / m);

	// Задаем вектора скоростей
	// Очищаем от старых координат
	speed_x.clear();
	speed_y.clear();
	// Задаем размерности
	speed_x.resize(all_particles);
	speed_y.resize(all_particles);

	// Задаем скорости
	int angle;
	for (int i = 0; i < all_particles; i++) {
		// Случайный угол [ рад ], [0;2pi)
		angle = uniform_random_distribution() * 2 * M_PI;
		speed_x[i] = speed_module * cos(angle);
		speed_y[i] = speed_module * sin(angle);
	}

	// Нормировка
	speed_normalization(speed_x);
	speed_normalization(speed_y);
}

/*   СИЛЫ   */
// Возвращает потенциальную энергию
double Particles_and_processing::force_calculation(Particle_State& in_state,
	double& cell_width, double& cell_height) {
	if (in_state.x.empty() || in_state.y.empty())
		return 0.0;

	int particle_number = in_state.x.size();

	// Задаем вектора сил
	// Очищаем от старых
	in_state.force_x.clear();
	in_state.force_y.clear();
	// Задаем размерности
	in_state.force_x.resize(particle_number);
	in_state.force_y.resize(particle_number);


	// Потенциальная энергия системы
	double potential = 0;
	// Проходим по строчно
	for (int particle_i = 0; particle_i < particle_number; particle_i++) {

		// Суммируем
		double sum_x = 0;
		double sum_y = 0;
		for (int j = 0; j < particle_number; j++) {
			// Все кроме самих себя
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

			// Расчет потенциальной энергии
			double K_r = 0;
			if (K_r <= r_1) {
				K_r = 1;
			}
			else if (K_r <= r_2) {
				double radius_ratio = (sqrt(radius_2_power) - r_1) / (r_1 - r_2);
				K_r = 1 - radius_ratio * radius_ratio;
				K_r *= K_r;
			}

			double val = (sigma_6_power / radius_6_power);
			potential += (a * a - a) * K_r;
		}
		in_state.force_x[particle_i] = force_multiplier * sum_x;
		in_state.force_y[particle_i] = force_multiplier * sum_y;
	}
	potential /= 2; // Взаимодействие между 2 частицами считалось дважды
	potential *= 4 * D;
}


/*   НАЧАЛЬНОЕ СОСТОЯНИЕ   */
void Particles_and_processing::simple_initial_state(
	int number_elements_on_side,
	double in_temperature
) {
	// Шаги по сетки
	double step_x = a;
	double step_y = a * sqrt(3) / 2.;

	// Кол-во всего частиц
	int all_particles = number_of_particles(number_elements_on_side);

	// Задание границ расчетной ячейки
	border.x_start = -step_x / 2;
	border.x_end = (number_elements_on_side - 1) * step_x + (step_x / 2);
	border.y_start = -step_y / 2;
	border.y_end = (number_elements_on_side - 1) * step_y + (step_y / 2);
	border.initialized = true;

	// Заполнение координат частиц
	state.x = generate_coordinates_x(
		all_particles, number_elements_on_side, step_x);
	state.y = generate_coordinates_y(
		all_particles, number_elements_on_side, step_y);

	// Задание скоростей
	temperature.push_back(in_temperature);
	starting_speed(
		in_temperature, all_particles,
		state.speed_x, state.speed_y);

	// Расчет сил
	double cell_width = border.x_end - border.x_start;
	double cell_height = border.y_end - border.y_start;
	double potential = force_calculation(state, cell_width, cell_height);

	// V^2 средняя
	double average_speed_power2 = square_average_speed(
		state.speed_x, state.speed_y);

	// Заполнение энргии
	double kinetic = kinetic_energy_of_system(average_speed_power2);
	double total = kinetic + potential;

	energy_kinetic.push_back(kinetic);
	energy_potential.push_back(potential);
	energy_total.push_back(total);

	// Заполнение энтальпии
	enthalpy.push_back(total + calculation_PV(in_temperature));
}

void position_in_cell(double& coordinate, double& cell_min, double& cell_max, double& size) {
	if (coordinate < cell_min)
		coordinate += size;
	else if (coordinate > cell_max)
		coordinate -= size;
}

/*   Расчет следующего положения    */
void Particles_and_processing::next_state(Particle_State& old_state, double& time) {
	// Создаем новое состояние
	Particle_State new_state;

	if (old_state.x.empty() || old_state.y.empty() ||
		old_state.speed_x.empty() || old_state.speed_y.empty() ||
		old_state.force_x.empty() || old_state.force_y.empty())
		return ;

	int particle_number = old_state.x.size(); // Кол-в частиц

	new_state.resize(particle_number);

	// Задаем координаты
	double cell_width = border.x_end - border.x_start;
	double cell_height = border.y_end - border.y_start;
	for (int i = 0; i < particle_number; i++) {
		new_state.x[i] = old_state.x[i] +
			old_state.speed_x[i] * time +
			(old_state.force_x[i] / (2 * m)) * time * time;

		new_state.y[i] = old_state.y[i] +
			old_state.speed_y[i] * time +
			(old_state.force_y[i] / (2 * m)) * time * time;

		// Проверка на положение частицы в ячейке
		position_in_cell(new_state.x[i], border.x_start, border.x_end, cell_width);
		position_in_cell(new_state.y[i], border.y_start, border.y_end, cell_height);
	}

	// Расчитываем силы
	double potential = force_calculation(new_state, cell_width, cell_height);

	// Расчитываем скорости
	for (int i = 0; i < particle_number; i++) {
		new_state.speed_x[i] = old_state.speed_x[i] +
			((new_state.force_x[i] - old_state.force_x[i] ) / (2 * m)) * time;

		new_state.speed_y[i] = old_state.speed_y[i] +
			((new_state.force_y[i] - old_state.force_y[i]) / (2 * m)) * time;
	}


	// V^2 средняя
	double average_speed_power2 = square_average_speed(
		state.speed_x, state.speed_y);

	// Заполнение энргии
	double kinetic = kinetic_energy_of_system(average_speed_power2);
	double total = kinetic + potential;

	energy_kinetic.push_back(kinetic);
	energy_potential.push_back(potential);
	energy_total.push_back(total);

	// Заполнение Среднего квадрата смещения R^2
	double temperature_now = temperature_calculation(average_speed_power2);
	double radius_displacement = average_square_displacement_calculation(old_state, new_state);
	temperature.push_back(temperature_now);
	average_square_displacement.push_back(radius_displacement);

	// Заполнение энтальпии
	enthalpy.push_back(total + calculation_PV(temperature_now));

	state = new_state;
}

/* ДЛЯ ОТРИСОВКИ */
// Получение линии границы
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

// Получение вектора координат центров частиц
Vector2D Particles_and_processing::get_center_particles() {
	return Vector2D(state.x, state.y);
}


// V^2 средняя
double Particles_and_processing::square_average_speed(
	vector<double>& speed_x, vector<double>& speed_y) {
	if (speed_x.empty() || speed_y.empty())
		return 0.0;

	int particle_number = speed_x.size(); // Кол-в частиц

	// Суммируем квадрат скоростей
	double sum_speeds = 0;
	for (int i = 0; i < particle_number; i++) {
		sum_speeds += speed_x[i] * speed_x[i] + speed_y[i] * speed_y[i];
	}
	sum_speeds /= particle_number; // Средний квадрат скорости
	return sum_speeds;
}

// Энергия
double Particles_and_processing::kinetic_energy_of_system(
	vector<double>& speed_x, vector<double>& speed_y) {

	if (speed_x.empty() || speed_y.empty())
		return 0.0;

	int particle_number = speed_x.size(); // Кол-в частиц

	// Суммируем квадрат скоростей
	double sum_speeds = 0;
	for (int i = 0; i < particle_number; i++) {
		sum_speeds += speed_x[i] * speed_x[i] + speed_y[i] * speed_y[i];
	}
	sum_speeds /= particle_number; // Средний квадрат скорости

	return (m * sum_speeds) / 2;
}
double Particles_and_processing::kinetic_energy_of_system(double& square_average_speed) {
	return (m * square_average_speed) / 2;
}

// PV
double scalar_product(double& x1, double& y1, double& x2, double& y2) {
	return x1 * x2 + y1 * y2;
}

double Particles_and_processing::calculation_PV(double& temperature) {
	if (state.x.empty() || state.y.empty() ||
		state.force_x.empty() || state.force_y.empty())
		return 0.0;

	int particle_number = state.x.size(); // Кол-в частиц

	double val = 0;
	for (int i = 0; i < particle_number; i++) {
		// скалярное произведение
		val += state.x[i] * state.force_x[i] + state.y[i] * state.force_y[i];
	}

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	return particle_number * k * temperature + 0.5 * val;
}

// T
double Particles_and_processing::temperature_calculation(
	vector<double>& speed_x, vector<double>& speed_y) {

	if (speed_x.empty() || speed_y.empty())
		return 0.0;

	/*

	mV ^ 2   d
	------ = - * k * Temperature; d - степень свободы
	   2     2

	Temperature = (m * V ^ 2) / ( d * k ) ; d = 1
	Temperature = (m * V * V) / k

	*/
	int particle_number = speed_x.size(); // Кол-в частиц

	// Суммируем квадрат скоростей
	double sum_speeds = 0;
	for (int i = 0; i < particle_number; i++) {
		sum_speeds += speed_x[i] * speed_x[i] + speed_y[i] * speed_y[i];
	}
	sum_speeds /= particle_number; // Средний квадрат скорости

	return (m * sum_speeds) / k;
}
double Particles_and_processing::temperature_calculation(double& square_average_speed) {
	return (m * square_average_speed) / k;
}

// R^2
double Particles_and_processing::average_square_displacement_calculation(
	vector<double>& x_old, vector<double>& y_old,
	vector<double>& x_new, vector<double>& y_new) {

	if (x_old.empty() || y_old.empty() ||
		x_new.empty() || y_new.empty())
		return 0.0;

	int particle_number = x_old.size(); // Кол-в частиц

	double displacement = 0;

	for (int i = 0; i < particle_number; i++) {
		double difference_x = x_old[i] - x_new[i];
		double difference_y = y_old[i] - y_new[i];
		displacement = difference_x * difference_x + difference_y * difference_y;
	}
	return displacement / particle_number;
}

double Particles_and_processing::average_square_displacement_calculation(
	Particle_State state_old,
	Particle_State state_new) {
	return average_square_displacement_calculation(
		state_old.x, state_old.y,
		state_new.x, state_new.y);
}