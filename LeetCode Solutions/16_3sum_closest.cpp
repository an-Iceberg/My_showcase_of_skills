
#include <iostream>
#include <vector>

// Returns the positive differernce between target and sum
int distance(const int &target, const int &sum)
{
    if (target > sum)
    {
        return target - sum;
    }
    else if (target < sum)
    {
        return sum - target;
    }
    else
    {
        return 0;
    }
}

int threeSumClosest(const std::vector<int> &numbers, const int &target)
{
    int sum;
    int diff = 999;
    int result;
    const int *num1;
    const int *num2;
    const int *num3;

    num1 = &numbers[0];
    num2 = &numbers[0];
    num3 = &numbers[0];

    for (int iteration = 0; iteration < (numbers.size() * numbers.size() * numbers.size()); iteration++)
    {
        // Only calcualting the sum if the pointers point to distinct addresses
        if (num1 != num2 && num1 != num3 && num2 != num3)
        {
            sum = *num1 + *num2 + *num3;

            if (sum == target)
            {
                return sum;
            }

            // If the current sum is closer to the target than the previous sum, replace the previous result with the current sum
            if (distance(target, sum) < diff)
            {
                diff = distance(target, sum);
                result = sum;
            }
        }

        // If this is the constellation of the pointer, all cases have been evaluated
        if (num1 == &numbers[numbers.size() - 3] && num2 == &numbers[numbers.size() - 2] && num3 >= &numbers[numbers.size() - 1])
        {
            break;
        }

        // If the second pointer has reached the end and the first is about to finish, reset their positions and increment the third pointer
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

    return result;
}

void test(int testID, std::vector<int> numbers, int target, int expected)
{
    int result = threeSumClosest(numbers, target);

    if (result != expected)
    {
        std::cout << testID << " failed expected:" << expected << " got:" << result << "\n";
    }
    else
    {
        std::cout << testID << " passed" << "\n";
    }
}

int main()
{
    // Tests from leetcode.com
    test(0, {-1, 2, 1, -4}, 1, 2);
    test(1, {0, 0, 0}, 1, 0);

    // My own tests
    test(2, {2, 3, 6, -2}, 8, 7);
    test(3, {1, 2, 3, 4, 5, 6}, 11, 11);
    test(4, {1, 2, 3, 4, 5, 6}, 10, 10);
    test(5, {1, 2, 3, 4, 5, 6}, 9, 9);
    test(6, {-1, -2, 5, 5, 6, 7}, 7, 8);
    test(7, {-1, -2, 5, 5, 6, 7}, 19, 18);
    test(8, {-5, 3, 4, 10}, 1, 2);
    test(9, {-5, 3, 4, 10}, 10, 9);
    test(10, {-5, -1, 10, 34, 26, 72, 82}, 3, 4);
    test(11, {-5, -1, 10, 34, 26, 72, 82}, 60, 59);
    test(12, {-5, -1, 10, 34, 26, 72, 82}, 95, 93);

    return 0;
}
