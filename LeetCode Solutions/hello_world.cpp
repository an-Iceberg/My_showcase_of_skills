#include <iostream>

int main()
{
    std::cout << "To exit the program enter 'exit'." << std::endl;

    std::string userInput = "";

    while (true)
    {
        std::cin >> userInput;
        if (userInput == "exit")
            break;
        std::cout << userInput << std::endl;
    }

    std::cout << "Hello World!" << std::endl;
    return 0;
}
