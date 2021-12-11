#include <iostream>
#include <stack>

bool isPalindrome(int &number)
{
    if (number < 0)
    {
        return false;
    }

    std::string sNumber = std::to_string(number);

    std::stack<char> stack;

    int size = sNumber.size();

    // Type 1 palindrome: 123321
    if (size % 2 == 0)
    {
        size /= 2;

        for (int index = 0; index < size; index++)
        {
            stack.push(sNumber[index]);
        }

        for (int index = size; index < sNumber.size(); index++)
        {
            if (stack.top() != sNumber[index])
            {
                return false;
            }
            stack.pop();
        }

        return true;
    }

    // Type 2 palindrome: 12321
    else
    {
        size /= 2;

        for (int index = 0; index < size; index++)
        {
            stack.push(sNumber[index]);
        }

        for (int index = size + 1; index < sNumber.size(); index++)
        {
            if (stack.top() != sNumber[index])
            {
                return false;
            }
            stack.pop();
        }

        return true;
    }

    return false;
}

void test(int testID, int input, int expected)
{
    int result = isPalindrome(input);

    if (result != expected)
    {
        std::cout << testID << " failed expected:" << expected << " got:" << result << std::endl;
    }
    else
    {
        std::cout << testID << " passed" << std::endl;
    }
}

int main()
{
    // Tests from leetcode.com
    test(0, 121, true);
    test(1, -121, false);
    test(2, 10, false);
    test(3, -101, false);

    // My own tests
    test(4, 123321, true);
    test(5, 94566549, true);
    test(6, 123454321, true);
    test(7, 9467649, true);
    test(8, 12343211, false);
    test(9, 12321232, false);
    test(10, 123212321, true);
    test(11, 1234432, false);
    return 0;
}
