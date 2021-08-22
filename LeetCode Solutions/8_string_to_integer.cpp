#include <iostream>
#include <string>

int myAtoi(std::string input)
{
    bool positive = true;
    bool sign = true;
    int number = 0;

    for (int index = 0; index < input.size(); index++)
    {
        if (sign && input[index] == ' ')
            continue;

        if (sign && input[index] == '+')
        {
            positive = true;
            sign = false;
            continue;
        }
        else if (sign && input[index] == '-')
        {
            positive = false;
            sign = false;
            continue;
        }

        switch (input[index])
        {
        case '0': number *= 10; number += 0; break;
        case '1': number *= 10; number += 1; break;
        case '2': number *= 10; number += 2; break;
        case '3': number *= 10; number += 3; break;
        case '4': number *= 10; number += 4; break;
        case '5': number *= 10; number += 5; break;
        case '6': number *= 10; number += 6; break;
        case '7': number *= 10; number += 7; break;
        case '8': number *= 10; number += 8; break;
        case '9': number *= 10; number += 9; break;
        }

        if (input[index] != '0' &&
            input[index] != '1' &&
            input[index] != '2' &&
            input[index] != '3' &&
            input[index] != '4' &&
            input[index] != '5' &&
            input[index] != '6' &&
            input[index] != '7' &&
            input[index] != '8' &&
            input[index] != '9')
        {
            break;
        }
    }

    if (number > 2147483648)
        number = 2147483648;

    if (!positive)
        number = -number;

    return number;
}

void test(int testID, std::string input, int expected)
{
    int result = myAtoi(input);
    if (result != expected)
        std::cout << testID << " failed expected:" << expected << " got:" << result << std::endl;
    else
        std::cout << testID << " passed" << std::endl;
}

int main()
{
    // Tests from leetcode.com
    test(0, "42", 42);
    test(1, "     -42", -42);
    test(2, "4193 with words", 4193);
    test(3, "words and 987", 0);

    // My own tests
    test(4, " 345", 345);
    test(5, "  h234", 0);
    test(6, "234h567", 234);
    test(7, "-98ha76", -98);
    return 0;
}
