#include <vector>
#include <cassert>
#include <iostream>

int medianCalculator(std::vector<int> &numbers)
{
    int median = -1;

    if (numbers.size() < 1)
    {
        return median;
    }

    // If the size of 'numbers' is even
    if (numbers.size() % 2 == 0)
    {
        // Calculates the average of the two central values
        median = (numbers[numbers.size() / 2] + numbers[numbers.size() / 2 - 1]) / 2;
    }
    else
    {
        median = numbers[numbers.size() / 2];
    }

    return median;
}

int findMedianOfTwoSortedArrays(std::vector<int> &numbers1, std::vector<int> &numbers2)
{
    int median1 = medianCalculator(numbers1);
    int median2 = medianCalculator(numbers2);

    if (median1 == -1)
    {
        return median2;
    }

    if (median2 == -1)
    {
        return median1;
    }

    return (median1 + median2) / 2;
}

void test(int testIndex, std::vector<int> vector1, std::vector<int> vector2, int median)
{
    if (median != findMedianOfTwoSortedArrays(vector1, vector2))
    {
        std::cout << testIndex << " failed, returned:" << findMedianOfTwoSortedArrays(vector1, vector2) << " expected:" << median << "\n";
    }
    else
    {
        std::cout << testIndex << " passed" << "\n";
    }
}

int main()
{
    // Tests from leetcode.com
    test(1, {1, 3}, {2}, 2);
    test(2, {1, 2}, {3, 4}, 2.5);
    test(3, {0, 0}, {0, 0}, 0);
    test(4, {}, {1}, 1);
    test(5, {2}, {}, 2);

    // My own tests
    test(6, {}, {10, 11, 12}, 11);
    test(7, {2, 2, 2, 17, 127}, {}, 2);
    test(8, {17, 18, 19, 20, 21}, {1, 27, 129}, 23);
    test(9, {}, {121, 121, 122, 122, 123}, 122);
    test(10, {1, 5, 7, 10}, {21, 34, 78, 122}, 31);

    return 0;
}
