#include <iostream>
#include <string>
#include <vector>

std::string longestCommonPrefix(std::vector<std::string>& strings)
{
    if (strings.size() == 0)
        return "";

    std::string prefix = "";

    // Since we don't know any prefixes yet, we just copy the entire word and narrow it down later on
    prefix = strings[0];

    if (strings.size() == 1)
        return prefix;

    // Iterates over the remaining strings
    for (int str = 1; str < strings.size(); str++)
    {
        int index = 0;
        
        // While the prefix matches the string, count the position
        while (prefix[index] == strings[str][index])
        {
           index++;
        }

        // Shorten the prefix accordingly
        prefix.erase(index);
    }
    return prefix;
}

void test(int testID, std::vector<std::string> input, std::string expected)
{
    std::string result = longestCommonPrefix(input);

    if (result != expected)
        std::cout << testID << " failed expected:" << expected << " got:" << result << std::endl;
    else
        std::cout << testID << " passed" << std::endl;
    
}

int main()
{
    // Tests from leetcode.com
    test(0, {"flower", "flow", "flight"}, "fl");
    test(1, {"dog", "racecar", "car"}, "");

    // My own tests
    test(2, {"abbreviation", "after", "aggressive", "alteration"}, "a");
    test(3, {"grandparent", "grandmother", "grand canyon"}, "grand");
    test(4, {"master", "good", "hello"}, "");
    test(5, {"postfix", "postman", "posterior"}, "post");

    return 0;
}
