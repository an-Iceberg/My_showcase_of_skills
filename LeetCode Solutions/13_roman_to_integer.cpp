#include <iostream>
#include <string>

int romanToInt(std::string str)
{
    int number = 0;

    char memory = '-';

    for (int index = 0; index < str.size(); index++)
    {
        if (index < str.size())
        {
            memory = str[index + 1];
        }

        // This could be refactored
        if (str[index] == 'C' && memory == 'M')
        {
            number += 900;
            index++;
            continue;
        }
        if (str[index] == 'C' && memory == 'D')
        {
            number += 400;
            index++;
            continue;
        }
        if (str[index] == 'X' && memory == 'C')
        {
            number += 90;
            index++;
            continue;
        }
        if (str[index] == 'X' && memory == 'L')
        {
            number += 40;
            index++;
            continue;
        }
        if (str[index] == 'I' && memory == 'X')
        {
            number += 9;
            index++;
            continue;
        }
        if (str[index] == 'I' && memory == 'V')
        {
            number += 4;
            index++;
            continue;
        }

        switch (str[index])
        {
        case 'M': number += 1000; break;
        case 'D': number += 500; break;
        case 'C': number += 100; break;
        case 'L': number += 50; break;
        case 'X': number += 10; break;
        case 'V': number += 5; break;
        case 'I': number += 1; break;
        default: return -1;
        }
    }
    return number;
}

void test(int testID, std::string input, int expected)
{
    int result = romanToInt(input);

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
    test(0, "III", 3);
    test(1, "IV", 4);
    test(2, "IX", 9);
    test(3, "LVIII", 58);
    test(4, "MCMXCIV", 1994);

    // My own tests
    test(5, "XL", 40);
    test(6, "MCMIV", 1904);
    test(7, "MMMDXLIX", 3549);
    test(8, "XLIX", 49);
    test(9, "XCIX", 99);
    test(10, "DCCCXLVII", 847);
    test(11, "MLIII", 1053);
    test(12, "MDXVI", 1516);
    test(13, "MDCCLXXVI", 1776);
    test(14, "MMXVIII", 2018);
    return 0;
}
