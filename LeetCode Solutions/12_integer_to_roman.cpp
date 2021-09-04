#include <iostream>
#include <string>

std::string intToRoman(int input)
{
    std::string result = "";
    int divider = 1;
    int number = 0;

    while (input > 0)
    {
        while (divider < input)
            divider *= 10;
        divider /= 10;

        number = input / divider % 10;

        if (input > 999)
        {
            result += "M";
        }
        else if (input > 99)
        {
            switch(number){
                case 0: break;
                case 1: result += "C"; break;
                case 2: result += "CC"; break;
                case 3: result += "CCC"; break;
                case 4: result += "CD"; break;
                case 5: result += "D"; break;
                case 6: result += "DC"; break;
                case 7: result += "DCC"; break;
                case 8: result += "DCCC"; break;
                case 9: result += "CM"; break;
            }
        }
        else if (input > 9)
        {
            switch(number){
                case 0: break;
                case 1: result += "X"; break;
                case 2: result += "XX"; break;
                case 3: result += "XXX"; break;
                case 4: result += "XL"; break;
                case 5: result += "L"; break;
                case 6: result += "LX"; break;
                case 7: result += "LXX"; break;
                case 8: result += "LXXX"; break;
                case 9: result += "XC"; break;
            }
        }
        else if (input > 0)
        {
            switch(number){
                case 0: break;
                case 1: result += "I";break;
                case 2: result += "II";break;
                case 3: result += "III";break;
                case 4: result += "IV";break;
                case 5: result += "V";break;
                case 6: result += "VI";break;
                case 7: result += "VII";break;
                case 8: result += "VIII";break;
                case 9: result += "IX";break;
            }
        }
        if(input > 999)
            input -= 1000;
        else
            input -= number * divider; // Chops off the biggest digit of the input
    }
    return result;
}

void test(int testID, int input, std::string expected)
{
    std::string result = intToRoman(input);

    if (result != expected)
        std::cout << testID << " failed expected:" << expected << " got:" << result << std::endl;
    else
        std::cout << testID << " passed" << std::endl;
}

int main()
{
    // Tests from leetcode.com
    test(0, 3, "III");
    test(1, 4, "IV");
    test(2, 9, "IX");
    test(3, 58, "LVIII");
    test(4, 1994, "MCMXCIV");

    // My own tests
    test(5, 1005, "MV");
    test(6, 250, "CCL");
    test(7, 94, "XCIV");
    test(8, 44, "XLIV");
    test(9, 186, "CLXXXVI");
    test(10, 1066, "MLXVI");
    test(11, 2421, "MMCDXXI");
    test(12, 3999, "MMMCMXCIX");

    // std::cin.get();
    return 0;
}
