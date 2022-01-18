#include <iostream>
#include <vector>

// Returns true, if the combination of pointers is already present in the solution set
bool isAlreadyInSet(const int *&num1, const int *&num2, const int *&num3, const std::vector<std::vector<int>> &solutionSet)
{
    int foulCounter = 0;

    // Iterates over all solution sets
    for (std::vector<int> solution : solutionSet)
    {
        // Resets the foul counter for each solution set
        foulCounter = 0;

        // Iterates over all numbers in a solution set
        for (int number : solution)
        {
            // If a number in the set equals a pointer value, increment the foul counter
            if (number == *num1 || number == *num2 || number == *num3)
            {
                foulCounter++;
            }

            // If two numbers equal their pointers, the third one also equals its pointer, thus the solution is already present
            if (foulCounter >= 2)
            {
                return true;
            }
        }
    }

    return false;
}

// Finds all solutions and returns only one permutation of each solution
std::vector<std::vector<int>> threeSum(const std::vector<int> &numbers)
{
    std::vector<std::vector<int>> solutionSet = {};

    const int *num1;
    const int *num2;
    const int *num3;

    num1 = &numbers[0];
    num2 = &numbers[0];
    num3 = &numbers[0];

    // Limiting the number of iterations to size^3 of numbers (that's at max how many iterations it takes to find all solutions)
    for (int i = 0; i < (numbers.size() * numbers.size() * numbers.size()); i++)
    {
        // If all pointers point to different numbers and their sum is 0 and the combination is not already present in the solution set
        // then add the combination to the solution set
        if (num1 != num2 && num1 != num3 && num2 != num3 && *num1 + *num2 + *num3 == 0 && !isAlreadyInSet(num1, num2, num3, solutionSet))
        {
            solutionSet.push_back({*num1, *num2, *num3});
        }

        // If all three pointers are about to reach the end of the vector, all cases have been evaluated
        if (num1 == &numbers[numbers.size() - 3] && num2 == &numbers[numbers.size() - 2] && num3 >= &numbers[numbers.size() - 1])
        {
            break;
        }

        // If the second pointer has reached the end and the first is about to reach the second, reset their positions and increment the third pointer
        if (num1 == &numbers[numbers.size() - 2] && num2 >= &numbers[numbers.size() - 1])
        {
            num3++;
            num2 = &numbers[0];
            num1 = &numbers[0];
            continue;
        }

        // If the first pointer has reached the end of the vector, reset it to the beginning of the vector and increment the second pointer
        if (num1 >= &numbers[numbers.size() - 1])
        {
            num1 = &numbers[0];
            num2++;
            continue;
        }

        num1++;
    }

    return solutionSet;
}

void printNumbers(std::vector<std::vector<int>> &numbers)
{
    // Iterates over all vectors
    for (int vector = 0; vector < numbers.size(); vector++)
    {
        std::cout << "[";

        // Iterates over all numbers
        for (int number = 0; number < numbers[vector].size(); number++)
        {
            std::cout << numbers[vector][number];

            if (number < numbers[vector].size() - 1)
                std::cout << ",";
        }

        std::cout << "]";

        if (vector < numbers.size() - 1)
            std::cout << ",";
    }
}

void test(int testID, std::vector<int> input, std::vector<std::vector<int>> expected)
{
    std::vector<std::vector<int>> result = threeSum(input);

    if (result != expected)
    {
        std::cout << testID << " failed expected:" << "[";

        printNumbers(expected);

        std::cout << "]" << " got:" << "[";

        printNumbers(result);

        std::cout << "]" << "\n";
    }
    else
        std::cout << testID << " passed" << "\n";
}

int main()
{
    // Tests from leetcode.com
    test(0, {-1, 0, 1, 2, -1, -4}, {{1, 0, -1}, {-1, 2, -1}}); // The order of the numbers is not the same as on leetcode but the solution is still correct
    test(1, {}, {});
    test(2, {0}, {});

    // My own tests
    test(3, {1, 12, 2, 23, 3, -5, -11}, {{-5, 3, 2}});
    test(4, {4, 7, -9, -5, 4, -2}, {{-2, -5, 7}});
    test(5, {-13, -8, 4, -18, -8, 14, -16, -20, -11, 3, 20, 0}, {{14, -18, 4}, {0, 20, -20}});
    return 0;
}
