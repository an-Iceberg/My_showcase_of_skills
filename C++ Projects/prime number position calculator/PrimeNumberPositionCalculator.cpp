#include <iostream>
#include <string>
#include <math.h>

void findCorrectSuffix(std::string& suffix, int index) {
	if (((index / 10) % 10) == 1) { suffix = "th"; return; } // For numbers ending in 11, 12, 13
	int a = index % 10;
	switch (a) {
	case 1: suffix = "st"; break;
	case 2: suffix = "nd"; break;
	case 3: suffix = "rd"; break;
	default: suffix = "th"; break;
	}
}

bool isPrime(int number) {
	if (number == 2) { return true; }
	if (number % 2 == 0) { return false; }

	int counter = 3;
	while (counter <= sqrt(number)) {
		if (number % counter == 0) { return false; }
		counter += 2;
	}
	return true;
}

int isValidInput(int input) {
	if (input < 0) { return 1; }
	if (!isPrime(input)) { return 2; }
	return 0;
}

void validateInput(int& input) {
	int test;
	while (true) {
		test = isValidInput(input);
		switch (test) {
		case 0: return;
		case 1: std::cout << "Your input is negative, try again:"; break;
		case 2: std::cout << "Your input is not prime, try again:"; break;
		}
		std::cin >> input;
	}
}

int findPrimeIndex(int input) {
	if (input == 2) { return 1; }
	int position = 2;
	int i = 3;
	while (i < input) {
		if (isPrime(i)) { position++; }
		i += 2;
	}
	return position;
}

int main() {
	int input = 0;
	std::cout << "Enter your number here:";
	std::cin >> input;

	validateInput(input);

	int index = findPrimeIndex(input);

	std::string suffix;
	findCorrectSuffix(suffix, index);

	std::cout << "Your number " << input << " is the " << index << suffix << " prime number.";

	return 0;
}
