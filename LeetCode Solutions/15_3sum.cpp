#include <iostream>
#include <vector>

// Helper function
bool contains(std::vector<int> &numbers, int number)
{
    for (int index = 0; index < numbers.size(); index++)
    {
        if (numbers[index] == number)
            return true;
    }

    return false;
}
// /Helper function

// This is not an efficient solution by any means. But it does deliver a correct solution
std::vector<std::vector<int>> threeSum(std::vector<int> &numbers)
{
    std::vector<std::vector<int>> solutionSet = {}; // This stores the values
    std::vector<int> exclusionSet = {};             // This stores the indices of the values

    // Iterates over 'numbers' on 3 levels, checking each number
    // If the indices are the same, the iteration is ignored
    // Solutions are progressively added to the exclusion set; this is to avoid duplicate solutions (because the order of the solution doesn't matter and we only want it once)
    // If an index is in the exclusion set, the iteration is ignored
    for (int firstIndex = 0; firstIndex < numbers.size(); firstIndex++)
    {
        if (contains(exclusionSet, firstIndex))
            continue;

        for (int secondIndex = 0; secondIndex < numbers.size(); secondIndex++)
        {
            if (contains(exclusionSet, secondIndex) || secondIndex == firstIndex)
                continue;

            for (int thirdIndex = 0; thirdIndex < numbers.size(); thirdIndex++)
            {
                if (contains(exclusionSet, thirdIndex) || thirdIndex == secondIndex || thirdIndex == firstIndex)
                    continue;

                if (numbers[firstIndex] + numbers[secondIndex] + numbers[thirdIndex] == 0)
                {
                    solutionSet.push_back({numbers[firstIndex], numbers[secondIndex], numbers[thirdIndex]});
                    exclusionSet.push_back(firstIndex);
                    exclusionSet.push_back(secondIndex);
                    exclusionSet.push_back(thirdIndex);
                }
            }
        }
    }

    return solutionSet;
}

void printNumbers(std::vector<std::vector<int>> &numbers)
{ // Iterates over all vectors
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

        std::cout << "]" << std::endl;
    }
    else
        std::cout << testID << " passed" << std::endl;
}

int main()
{
    // Tests from leetcode.com
    test(0, {-1, 0, 1, 2, -1, -4}, {{-1, 0, 1}, {-1, 2, -1}}); // The order of the numbers is not the same as on leetcode but the solution is still correct
    test(1, {}, {});
    test(2, {0}, {});

    // My own tests
    test(3, {1, 12, 2, 23, 3, -5, -11}, {{2, 3, -5}});
    test(4, {4, 7, -9, -5, 4, -2}, {{7, -5, -2}});
    test(5, {-13, -8, 4, -18, -8, 14, -16, -20, -11, 3, 20, 0}, {{4, -18, 14}, {-20, 20, 0}});
    return 0;
}
