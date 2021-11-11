/*I've started this project after trying to implement MergeSort unsuccessfully for six days.
I was able to complete this the same day as I had started it.
Quite interesting how the choice of algorithm can change stuff.
I have also been following uncle Bob's advice and I think the code turned out pretty nice and clean.*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

//// --*-- For testing purposes --*-- /////
bool isSorted(std::vector<int>& vector) {
	for (unsigned int i = 0; i < vector.size() - 1; i++) {
		if (vector[i] > vector[i + 1]) {
			return false;
		}
	}
	return true;
}
//// --*-- For testing purposes --*-- /////

//// --*-- For testing purposes --*-- /////
bool satisfiesHeapCondition(std::vector<int>& vector) {
	int parent = 0, child = 0;
	for (unsigned int i = 1; i < vector.size(); i++) {
		child = vector[i];
		if (i % 2 == 0) {
			parent = vector[(i - 2) / 2];
		}
		else {
			parent = vector[(i - 1) / 2];
		}
		if (child > parent) {
			return false;
		}
	}
	return true;
}
//// --*-- For testing purposes --*-- /////

int position(int index, int offset) {
	return (index - offset) / 2;
}

void swap(std::vector<int>& vector, int position1, int position2) {
	int a = vector[position1];
	vector[position1] = vector[position2];
	vector[position2] = a;
}

int setOffset(int index) {
	if (index % 2 == 0) { return 2; }
	return 1;
}

void moveUp(std::vector<int>& vector, int& index, int& offset) {
	swap(vector, index, position(index, offset));
	index = position(index, offset); // Move to the index of the new parent
	offset = setOffset(index);
}

void adjustHeap(std::vector<int>& vector, int index, int offset) {
	while (index > 0 && vector[index] > vector[position(index, offset)]) { // If the child is larger than the parent
			moveUp(vector, index, offset);
	}
}

void buildHeap(std::vector<int>& vector, int heapSize) {
	for (int i = 1; i < heapSize; i++) {
		adjustHeap(vector, i, setOffset(i));
	}
}

void heapSort(std::vector<int>& vector) {
	buildHeap(vector, vector.size());

	//// --*-- For testing purposes --*-- /////
	if (!satisfiesHeapCondition(vector)) {
		std::cout << vector.size() << " failed:" << ' ';
		for (unsigned int i = 0; i < vector.size(); i++) {
			std::cout << vector[i] << ' ';
		}
		std::cout << std::endl;
	}
	//// --*-- For testing purposes --*-- /////

	swap(vector, 0, vector.size() - 1);
	for (unsigned int i = vector.size() - 1; i >= 1; i--) {
		buildHeap(vector, i);
		swap(vector, 0, i - 1);
	}
}

int main() {
	std::srand(std::time(nullptr));
	bool flag = true;

	for (int i = 1; i <= 400; i++) {
		std::vector<int> vector(i);

		for (unsigned int i2 = 0; i2 < vector.size(); i2++) {
			vector[i2] = rand() % 1000 + 0;
		}

		heapSort(vector); //*This is where the magic happens* (つ◕౪◕)つ━☆ﾟ.*･｡ﾟ

		if (!isSorted(vector)) {
			flag = false;
		}
	}

	if (flag) {
		std::cout << "All tests passed!";
	}
	else {
		std::cout << "Some tests failed.";
	}
}
