#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

bool isSorted(std::vector<int> vector) {
	if (vector.size() <= 1) { return true; }
	for (unsigned int i = 0; i < vector.size() - 1; i++) {
		if (vector[i] > vector[i + 1]) {
			return false;
		}
	}
	return true;
}

void pivotMoveUpRoutine(std::vector<int>& vector, int index, int& pivot) {
	int temp = vector[pivot];
	vector[pivot] = vector[index]; // The index element is moved behind the pivot (it actually replaces the pivot for the moment)
	vector[index] = vector[pivot - 1]; // The element before the pivot is moved to the index position
	pivot--;
	vector[pivot] = temp; // The pivot element is movet up the vector by one
}

void sortingRoutine(std::vector<int>& vector, int& index, int& pivot) {
	if (vector[index] < vector[pivot]) {
		index++;
		return;
	}
	pivotMoveUpRoutine(vector, index, pivot);
}

void quickSort(std::vector<int>& vector, int index, int pivot) {
	if (pivot - index < 1) { return; }

	int oldIndex = index, oldPivot = pivot;

	while(index < pivot) {
		sortingRoutine(vector, index, pivot);
	}

	quickSort(vector, oldIndex, pivot - 1);
	quickSort(vector, pivot + 1, oldPivot);
}

int main() {
	std::srand(std::time(nullptr));

	for (int i = 1; i <= 500; i++) {

		std::vector<int> vector(0);

		for (int j = 0; j < i; j++) {
			vector.push_back(rand() % 500 + 0);
		}

		quickSort(vector, 0, vector.size() - 1); // *This is where the magic happens* (つ◕౪◕)つ━☆ﾟ.*･｡ﾟ

		if (!isSorted(vector)) {
			for (unsigned int i = 0; i < vector.size(); i++) {
				std::cout << vector[i] << ' ';
			}
		}
		else {
			std::cout << "Vector is sorted." << vector.size();
		}
		std::cout << std::endl;

	}
	return 0;
}
