#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ofstream file(R"(D:\code\tuvung\vocabulary.csv)", std::ios::app); // Updated file name // Open in append mode

    if (!file) {
        std::cerr << "Can't open vocabulary.csv for appending!\n"; // Updated error message
        return 1;
    }

    std::string word;
    std::string meaning;

    std::cout << "Enter vocabulary entries (type 'done' for word to finish):\n";

    while (true) {
        std::cout << "Enter word: ";
        std::getline(std::cin, word);

        if (word == "done") {
            break;
        }

        std::cout << "Enter meaning: ";
        std::getline(std::cin, meaning);

        // Write to CSV, ensuring comma separation
        file << word << "," << meaning << "\n";

        std::cout << "Entry added.\n\n";
    }

    file.close();
    std::cout << "Finished adding vocabulary.\n";

    return 0;
}