#include <iostream>

int reverse(int &number)
{
    bool negate = false;

    if (number < 0)
    {
        negate = true;
        number *= -1;
        // -number; // Fore some reason, this doesn't work on negative numbers
    }

    int reversedNumber = 0;

    while (number != 0)
    {
        reversedNumber *= 10;
        reversedNumber += (number % 10);
        number /= 10;
    }

    if (reversedNumber > 2147483648 || reversedNumber < -2147483648)
    {
        return 0;
    }

    if (negate)
    {
        return -reversedNumber;
    }

    return reversedNumber;
}

void test(int testID, int input, int expected)
{
    int result = reverse(input);

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
    test(0, 123, 321);
    test(1, -123, -321);
    test(2, 120, 21);
    test(3, 0, 0);

    // My own tests
    test(4, 452345, 543254);
    test(5, 123000, 321);
    test(6, -45, -54);
    test(7, -678000, -876);
    test(8, 10021, 12001);
    return 0;
}
