#include <iostream>

float diff(float minutes, float hours) {
	if (minutes > hours) {
		return minutes - hours;
	}
	return hours - minutes;
}

int main() {
	float hours;
	float minutes;
	std::cout << "Enter the time below." << std::endl;
	std::cout << "Hours:";
	std::cin >> hours;
	std::cout << "Minutes:";
	std::cin >> minutes;

	float degreeMinutes = minutes * 6.0f;
	float degreeHours = hours * 30.0f + (degreeMinutes / 72.0f * 6.0f);
	std::cout << "The degree between the long arm and the short arm is " << diff(degreeMinutes , degreeHours) << " degrees Celcius.";
}
