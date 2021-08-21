#include <iostream>
#include <fstream>
#include <string>

const std::string upperCase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // !! WARNING !! This is a huge safety hazard and is highly advised against by the council of safety
const std::string lowerCase = "abcdefghijklmnopqrstuvwxyz"; // !! WARNING !! This is a huge safety hazard and is highly advised against by the council of safety

void transformIntoLookupAlphabet( std::string& abc) {
	std::string newAbc = abc;
	newAbc.append(abc);
	newAbc.append(abc);
	abc = newAbc;
}

char shift(int index, int shiftAmount, std::string abc) {
	transformIntoLookupAlphabet(abc);
	std::string::iterator shifter = abc.begin();

	shifter += (26 + index);
	shifter += shiftAmount;
	return *shifter;
}

void modifyInput(char& inputBuffer, int index, int shiftAmount) {
	if (index < 26) {
		inputBuffer = shift(index, shiftAmount, lowerCase);
		return;
	}
	inputBuffer = shift(index - 26, shiftAmount, upperCase);
}

int findIndex(int inputBuffer) {
	std::string abc = lowerCase; abc.append(upperCase);
	std::string::iterator indexFinder = abc.begin();
	int index = 0;

	while (indexFinder != abc.end()) {
		if (*indexFinder != inputBuffer) { // If the iterator does not match the input, move to the next letter
			indexFinder++;
			index++;
		}
		else {
			return index;
		}
	}

	return -1;
}

void caesarCypher(std::ifstream& input, std::ofstream& output, int shiftAmount) {
	char inputBuffer = ' ';
	int index = 0;

	while (true) {
		input >> inputBuffer;
		if (!input) { break; }

		index = findIndex(inputBuffer);

		if (index == -1) { // If the input is not a letter
			output << inputBuffer;
			continue;
		}

		modifyInput(inputBuffer, index, shiftAmount);

		output << inputBuffer;
	}
}

int main() {

	std::ifstream input;
	std::ofstream output;

	input >> std::noskipws;

	input.open("inputText.txt");
	output.open("outputText.txt");

	int shiftAmount = 0;
	std::cout << "Please enter the shift amount here:";
	std::cin >> shiftAmount;

	if (shiftAmount > 26 || shiftAmount < -26) {
		shiftAmount %= 26;
	}

	caesarCypher(input, output, shiftAmount); // *This is where the magic happens* (つ◕౪◕)つ━☆ﾟ.*･｡ﾟ

	std::cout << "Done.";

	return 0;
}
