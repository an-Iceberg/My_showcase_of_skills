// [] is fast but .at() is safe

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

// Returns true, if the letter is a valid number
bool isNumber(const char &letter)
{
  switch (letter)
  {
  case '2': return true; break;
  case '3': return true; break;
  case '4': return true; break;
  case '5': return true; break;
  case '6': return true; break;
  case '7': return true; break;
  case '8': return true; break;
  case '9': return true; break;
  default: return false; break;
  }
}

// Returns the letters of the respective numer
std::vector<char> returnLetters(const char &number)
{
  switch (number)
  {
  case '2': return {'a', 'b', 'c'}; break;
  case '3': return {'d', 'e', 'f'}; break;
  case '4': return {'g', 'h', 'i'}; break;
  case '5': return {'j', 'k', 'l'}; break;
  case '6': return {'m', 'n', 'o'}; break;
  case '7': return {'p', 'q', 'r', 's'}; break;
  case '8': return {'t', 'u', 'v'}; break;
  case '9': return {'w', 'x', 'y', 'z'}; break;
  default: return {}; break;
  }
}

std::vector<std::string> letterCombinations(std::string &digits)
{
  std::sort(digits.begin(), digits.end());

  // Empty input
  if (digits == "")
  {
    return {};
  }

  // Input validation
  for (int letter = 0; letter < digits.size(); letter++)
  {
    // If any one of the input letters is not a valid number, abort the operation
    if (!isNumber(digits.at(letter)))
    {
      return {};
    }
  }

  std::vector<std::string> solution;
  std::vector<std::vector<char>> letters;

  // Building the construction data for the solution vector
  for (int i = 0; i < digits.size(); i++)
  {
    // Adds each vector of letters to 'letters'
    letters.push_back(returnLetters(digits.at(i)));
  }

  // Building the solution vector using the construction data
  // (This only works for a permutation depth of 2)
  // Iterating over the letters of the first vector
  for (int i = 0; i < letters.at(0).size(); i++)
  {
    // Building the components for permutation depth 1
    std::string depth1 = "";
    depth1 += letters.at(0).at(i);

    bool pushDepth1 = true;

    // Iterating over the rest of the vectors
    for (int j = 1; j < letters.size(); j++)
    {
      // Iterating over all letters of the vectors
      for (int k = 0; k < letters.at(j).size(); k++)
      {
        // Building the components for permutation depth 2
        std::string depth2 = "";
        depth2 += letters.at(0).at(i);
        depth2 += letters.at(j).at(k);
        solution.push_back(depth2);

        pushDepth1 = false;
      }
    }

    if (pushDepth1)
    {
      solution.push_back(depth1);
    }
  }

  return solution;
}

void printVector(const std::vector<std::string> &vector)
{
  std::cout << '[';

  // Prints all the strings in the vector
  for (int i = 0; i < vector.size(); i++)
  {
    std::cout << vector.at(i);

    // Only prints the separator if it's not the very last vector element
    if (i < vector.size() - 1)
    {
      std::cout << ',';
    }
  }

  std::cout << ']';
}

void test(int testID, std::string input, std::vector<std::string> expected)
{
  std::vector<std::string> result = letterCombinations(input);

  // If the program didn't give the desired results, inform the user
  if (result != expected)
  {
    std::cout << testID << " failed expected:";
    printVector(expected);
    std::cout << " got:";
    printVector(result);
    std::cout << "\n";
  }
  else
  {
    std::cout << testID << " passed" << "\n";
  }
}

int main()
{
  std::cout << "\n";

  // Tests from leetcode.com
  test(0, "23", {"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"});
  test(1, "", {});
  test(2, "2", {"a", "b", "c"});

  // TODO: write more of my own tests
  // My own tests
  test(3, "2b3", {});
  test(4, "1001", {});
  test(5, "32", {"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"});

  std::cout << "\n";

  return 0;
}
