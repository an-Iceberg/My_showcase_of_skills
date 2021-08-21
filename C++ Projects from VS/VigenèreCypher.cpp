#include <iostream>
#include <fstream>
#include <string>

const std::string upperCase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; // !! WARNING !! This is a huge safety hazard and is highly advised against by every and all safety personnel and all departements
const std::string lowerCase = "abcdefghijklmnopqrstuvwxyz"; // !! WARNING !! This is a huge safety hazard and is highly advised against by every and all safety personnel and all departements

void transformIntoLookupAlphabet(std::string& abc) {
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

void vigenereCypher(std::ifstream& input, std::ofstream& output, std::string codeWord, char mode) {
	char inputBuffer = ' ';
	int charIndex = 0;
	int shiftIndex = 0;
	std::string::iterator code = codeWord.begin();

	while (true) {
		input >> inputBuffer;
		if (!input) { break; }
		if (code == codeWord.end()) { code = codeWord.begin(); }

		charIndex = findIndex(inputBuffer);

		if (charIndex == -1) { // If the input is not a letter
			output << inputBuffer;
			continue;
		}

		shiftIndex = findIndex(*code);
		if (shiftIndex > 26) { shiftIndex -= 26; }
		if (mode == 'd') { shiftIndex = -shiftIndex; }

		modifyInput(inputBuffer, charIndex, shiftIndex);

		output << inputBuffer;

		code++;
	}
}

int main() {

	std::ifstream input;
	std::ofstream output;

	input >> std::noskipws;

	input.open("inputText.txt");
	output.open("outputText.txt");

	std::string codeWord = "-";
	std::cout << "Please enter the code word here:";
	std::cin >> codeWord;

	char mode = '-';
	do {
		std::cout << "Encode/decode?(e/d):";
		std::cin >> mode;
	} while (mode != 'e' && mode != 'd');

	vigenereCypher(input, output, codeWord, mode); // *This is where the magic happens* (つ◕౪◕)つ━☆ﾟ.*･｡ﾟ

	std::cout << "Done.";

	return 0;
}
