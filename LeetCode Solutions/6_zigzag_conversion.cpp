#include <iostream>
#include <string>
#include <vector>

std::string zigZagConvert(std::string &input, int &rows)
{
    std::vector<std::string> subStrings;

    // Initiating the right number of substrings
    for (int i = 0; i < rows; i++)
    {
        subStrings.push_back("");
    }
    
    rows--;

    int counter = 0;
    bool increment = true;

    // Appends each letter to the respective substring
    for (int index = 0; index < input.size(); index++)
    {
        subStrings[counter] += input[index];

        if (counter == rows)
        {
            increment = false;
        }

        if (counter == 0)
        {
            increment = true;
        }

        if (increment)
        {
            counter++;
        }
        else
        {
            counter--;
        }
    }

    std::string encryptedString;

    // Assembling the final result
    for (int index = 0; index < subStrings.size(); index++)
    {
        encryptedString += subStrings[index];
    }

    return encryptedString;
}

void test(int testID, std::string input, int rows, std::string expectedOutput)
{
    std::string result = zigZagConvert(input, rows);

    if (result != expectedOutput)
    {
        std::cout << testID << " failed expected:" << expectedOutput << " got:" << result << "\n";
    }
    else
    {
        std::cout << testID << " passed" << "\n";
    }
}

int main()
{
    // Tests from leetcode.com
    test(0, "PAYPALISHIRING", 3, "PAHNAPLSIIGYIR");
    test(1, "PAYPALISHIRING", 4, "PINALSIGYAHRPI");
    test(2, "A", 1, "A");

    // My own tests
    test(3, "avocadowatermelon", 2, "aoaoaemlnvcdwtreo");
    test(4, "sneezingelectroninthefridge", 5, "seidnglnnigeneotreeicrhfzte");
    return 0;
}
