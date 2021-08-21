#include <iostream>
#include <vector>

//// Factorising part ////
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

void incrementOrPushBack(std::vector<int>& factors, int& divisor, int& number, int incrementValue) {
	if (!isPrime(divisor) || number % divisor != 0) { // If 'counter' is not a prime or if 'number' cannot be divided evenly
		divisor += incrementValue;
		return;
	}
	factors.push_back(divisor);
	number /= divisor;
}

void factoriser(int number, std::vector<int>& factors) {
	int divisor = 2;

	while (divisor == 2) {
		incrementOrPushBack(factors, divisor, number, 1);
	}
	while (divisor <= number) {
		incrementOrPushBack(factors, divisor, number, 2);
	}
}
//// Factorising part ////

//// Printing part ////
void printPowerOf(int memory, int counter) {
	std::cout << memory;
	if (counter != 1) {
		std::cout << '^' << counter;
	}
}

void endOfPrintSegment(int factor, int& memory, int& counter) {
	std::cout << ", ";
	memory = factor;
	counter = 1;
}

void incrementOrPrint(int factor, int& memory, int& counter) {
	if (factor == memory) {
		counter++;
		return;
	}
	printPowerOf(memory, counter);
	endOfPrintSegment(factor, memory, counter);
}

void printer(std::vector<int> factors, int number) {
	std::cout << "The prime factors of " << number << " are: ";

	int counter = 0;
	int memory = factors[0];

	for (unsigned int i = 0; i < factors.size(); i++) {
		incrementOrPrint(factors[i], memory, counter);
	}
	printPowerOf(memory, counter);
}
//// Printing part ////

int main() {

	int number = 0;
	std::cout << "Please enter the number you want to factorise here:";
	std::cin >> number;

	std::vector<int> factors;
	factoriser(number, factors); // *This is where the magic happens* (つ◕౪◕)つ━☆ﾟ.*･｡ﾟ

	printer(factors, number);

	return 0;
}
