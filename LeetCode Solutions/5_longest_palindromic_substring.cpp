#include <iostream>
#include <string>

std::string longestPalindrome(std::string str)
{
    if (str.size() == 0)
    {
        return "";
    }
    else if (str.size() == 1)
    {
        return str;
    }

    std::string palindrome = "-";
    int length = 0;

    for (int index = 1; index < str.size() - 1; index++)
    {
        int size = 1;

        // Checks for type 1 palindrome: 'abccba'
        if (str.at(index - size) == str.at(index))
        {
            // As long as the characters match and the size is smaller than half the string
            while (index - size > -1 && index + size - 1 < str.size() && str.at(index - size) == str.at(index + size - 1))
            {
                size++;
            }

            if (size > length)
            {
                length = size;
                palindrome = str.substr(index - size + 1, size * 2 - 2);
            }
        }

        // Checks for type 2 palindrome: 'abcba'
        else if (str.at(index - size) == str.at(index + size))
        {
            // As long as the characters match and the size is smaller than half the string
            while (index - size > -1 && index + size < str.size() && str.at(index - size) == str.at(index + size))
            {
                size++;
            }

            if (size >= length)
            {
                length = size;
                palindrome = str.substr(index - size + 1, size * 2 - 1);
            }
        }
    }

    if (palindrome == "-")
    {
        return str.substr(0, 1);
    }

    return palindrome;
}

void test(int testIndex, std::string str, std::string palindrome)
{
    std::string result = longestPalindrome(str);

    if (palindrome != result)
    {
        std::cout << testIndex << " failed, returned:" << result << " expected:" << palindrome << std::endl;
    }
    else
    {
        std::cout << testIndex << " passed" << std::endl;
    }
}

int main()
{
    // Tests from leetcode.com
    test(0, "babad", "aba");
    test(1, "cbbd", "bb");
    test(2, "a", "a");
    test(3, "ac", "a");

    // My own tests
    test(4, "succubus", "uccu");
    test(5, "sugus", "sugus");
    test(6, "poitutiop", "poitutiop");
    test(7, "suggus", "suggus");
    test(8, "adrsfughhgusfs", "ughhgu");
    test(9, "adsfpoiuiopasa", "poiuiop");
    test(10, "astrertasdpoiuiopasd", "poiuiop");
    test(11, "asdtreertasdpoiuiopasd", "poiuiop");
    test(12, "asdpoiopasdtrewwerter", "trewwert");
    return 0;
}
