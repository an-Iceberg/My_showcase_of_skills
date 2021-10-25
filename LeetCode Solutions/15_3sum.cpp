#include <iostream>
#include <vector>

// TODO: This here
std::vector<std::vector<int>> threeSum(std::vector<int> &numbers)
{
    return {{0, 0, 0}, {0, 0}, {0, 0, 0, 0}};
}

void printNumbers(std::vector<std::vector<int>> &numbers)
{   // Iterates over all vectors
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
        std::cout << testID << " passed";
}

int main()
{
    // Tests from leetcode.com
    test(0, {-1, 0, 1, 2, -1, -4}, {{-1, -1, 2}, {-1, 0, 1}});
    return 0;
}
