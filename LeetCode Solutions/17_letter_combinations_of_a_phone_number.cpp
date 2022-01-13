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

std::vector<std::string> letterCombinations(const std::string &digits)
{
  // Empty input
  if (digits == "")
  {
    return {};
  }

  // Input validation
  for (int letter = 0; letter < digits.size(); letter++)
  {
    // If any one of the input letters is not a valid number, abort the operation
    if (!isNumber(digits[letter]))
    {
      std::cout << "  invalid input" << std::endl;

      return {};
    }
  }

  std::vector<std::string> solution;
  std::vector<std::vector<char>> letters;

  // Building the permutation vector
  for (int i = 0; i < digits.size(); i++)
  {
    // Adds each vector of letters to the letters vector
    letters.push_back(returnLetters(digits[i]));
  }

  // TODO: build the solution vector
  // dbg
  std::cout << '[';
  for (int i = 0; i < letters.size(); i++)
  {
    std::cout << '[';
    for (int j = 0; j < letters[i].size(); j++)
    {
      std::cout << letters[i][j];
      if (j < letters[i].size() - 1)
      {
        std::cout << ',';
      }
    }
    std::cout << ']';
    if (i < letters.size() - 1)
    {
      std::cout << ',';
    }
  }
  std::cout << ']';
  std::cout << std::endl << "letters.size:" << letters.size();
  std::cout << std::endl << "letters[0].size:" << letters[0].size() << std::endl;
  // /dbg

  int iteration = 0;

  // Puts the number of solution strings into 'iteration'
  for (int i = 0; i < letters.size(); i++)
  {
    iteration += letters[i].size();
  }
  
  for (int i = 0; i < iteration; i++)
  {
    
  }
  

  return {"af", "fa"};
}

void printVector(const std::vector<std::string> &vector)
{
  std::cout << '[';

  // Prints all the strings in the vector
  for (int i = 0; i < vector.size(); i++)
  {
    std::cout << vector[i];

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
    std::cout << std::endl;
  }
  else
  {
    std::cout << testID << " passed" << std::endl;
  }
}

int main()
{
  std::cout << std::endl;

  // Tests from leetcode.com
  test(0, "23", {"ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"});
  test(1, "", {});
  test(2, "2", {"a", "b", "c"});

  // TODO: write my own tests
  // My own tests
  test(3, "2b3", {});
  test(4, "1001", {});

  std::cout << std::endl;

  return 0;
}
