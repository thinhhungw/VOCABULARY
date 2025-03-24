#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <algorithm>

int main() {
    std::ifstream file(R"(D:\code\tuvung\tuvung.csv)");
    if (!file) {
        std::cerr << "Can't open file!\n";
        return 1;
    }

    std::vector<std::pair<std::string, std::string>> words;
    std::string line;

    // Đọc tất cả dữ liệu vào vector
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string first_column, second_column;
        if (!std::getline(ss, first_column, ',')) continue;
        if (!std::getline(ss, second_column, ',')) continue;
        words.emplace_back(first_column, second_column);
    }

    file.close();

    // Xáo trộn danh sách từ
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(words.begin(), words.end(), g);

    std::uniform_int_distribution<int> dist(0, 1); // Tạo số ngẫu nhiên 0 hoặc 1

    std::string user_input;
    int combo = 0; // Đếm số lần đúng liên tục

    while (true) {
        for (const auto& [first_column, second_column] : words) {
            bool show_first = dist(g); // Ngẫu nhiên chọn hiển thị cột 1 hay cột 2
            std::string question = show_first ? first_column : second_column;
            std::string answer = show_first ? second_column : first_column;

            do {
                std::cout << "Word: " << question << "\nEnter meaning: ";
                std::getline(std::cin, user_input);

                if (user_input == "done") {
                    std::cout << "End of program.\n";
                    return 0;
                }

                if (user_input == "?") {
                    std::cout << "Answer: " << answer << "\n\n";
                    combo = 0; // Reset combo khi người dùng yêu cầu đáp án
                    break;
                }

                if (user_input == answer) {
                    combo++;
                    std::cout << "YES!! COMBO: " << combo << "\n\n";
                    break;
                } else {
                    std::cout << "NO!! Try again.\n\n";
                    combo = 0; // Reset combo khi trả lời sai
                }
            } while (true);
        }
    }

    return 0;
}
