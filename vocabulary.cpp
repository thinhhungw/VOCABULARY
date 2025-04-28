#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <algorithm>
#include <limits> // Required for numeric_limits

// Function to save the current vocabulary list to the CSV file
void saveVocabularyToFile(const std::vector<std::pair<std::string, std::string>>& words, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Can't open file for saving: " << filename << std::endl;
        return;
    }
    for (const auto& pair : words) {
        file << pair.first << "," << pair.second << "\n";
    }
    file.close();
}

int main() {
    std::string filename = R"(D:\code\tuvung\vocabulary.csv)";
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Can't open file: " << filename << std::endl;
        return 1;
    }

    std::vector<std::pair<std::string, std::string>> words;
    std::string line;

    // Read all data into vector
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string first_column, second_column;
        if (!std::getline(ss, first_column, ',')) continue;
        if (!std::getline(ss, second_column, ',')) continue;
        words.emplace_back(first_column, second_column);
    }

    file.close();

    if (words.empty()) {
        std::cout << "Vocabulary list is empty. Add words using addvocabulary.cpp first.\n";
        return 0;
    }

    // Shuffle the list of words
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(words.begin(), words.end(), g);

    std::uniform_int_distribution<int> dist(0, 1); // Generate a random number 0 or 1

    std::string user_input;
    int combo = 0; // Count consecutive correct answers
    size_t current_word_index = 0; // To keep track of the current word being displayed

    std::cout << "Vocabulary practice. Type 'done' to exit, '?' for answer, 'remove' to delete word.\n\n";

    while (true) {
        if (words.empty()) {
            std::cout << "Vocabulary list is empty.\n";
            break; // Exit the loop if list becomes empty
        }

        // Cycle through the shuffled words
        if (current_word_index >= words.size()) {
            current_word_index = 0; // Loop back to the beginning
             std::shuffle(words.begin(), words.end(), g); // Reshuffle for a new round
        }

        const auto& current_word_pair = words[current_word_index];
        std::string first_column = current_word_pair.first;
        std::string second_column = current_word_pair.second;


        bool show_first = dist(g); // Randomly choose to display column 1 or column 2
        std::string question = show_first ? first_column : second_column;
        std::string answer = show_first ? second_column : first_column;

        do {
            std::cout << "Word: " << question << "\nEnter meaning: ";
            std::getline(std::cin, user_input);

            if (user_input == "done") {
                std::cout << "End of program.\n";
                saveVocabularyToFile(words, filename); // Save before exiting
                return 0;
            }

            if (user_input == "?") {
                std::cout << "Answer: " << answer << "\n\n";
                combo = 0; // Reset combo when the user asks for the answer
                current_word_index++; // Move to the next word after showing answer
                break;
            }

            if (user_input == "remove") {
                // Find and remove the current word pair
                bool removed = false;
                for (auto it = words.begin(); it != words.end(); ++it) {
                    // Need to check both original orientations (first, second) and (second, first)
                    // to match the pair that was displayed as question and answer
                    if ((it->first == first_column && it->second == second_column) ||
                        (it->first == second_column && it->second == first_column)) {
                        words.erase(it);
                        removed = true;
                        std::cout << "Removed: " << first_column << "," << second_column << " from the list.\n\n";
                        saveVocabularyToFile(words, filename); // Save immediately after removal
                        break; // Exit inner loop after removal
                    }
                }
                if (!removed) {
                     std::cout << "Could not find the word pair to remove.\n\n";
                }
                // After removal, the current_word_index might be out of bounds
                // The outer loop's check `if (current_word_index >= words.size())` will handle resetting it.
                break; // Exit inner loop after remove command
            }

            if (user_input == answer) {
                combo++;
                std::cout << "YES!! COMBO: " << combo << "\n\n";
                current_word_index++; // Move to the next word on correct answer
                break;
            } else {
                std::cout << "NO!! Try again.\n\n";
                combo = 0; // Reset combo when the answer is wrong
            }
        } while (true);
    }

    saveVocabularyToFile(words, filename); // Save one last time if loop breaks for other reason

    return 0;
}