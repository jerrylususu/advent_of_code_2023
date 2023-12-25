#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>

int main() {
    std::ifstream file("day8.txt");
    std::vector<std::string> lines;
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    }

    std::string steps = lines[0];
    std::map<std::string, std::pair<std::string, std::string>> mappings;
    for (size_t i = 2; i < lines.size(); ++i) {
        std::string line = lines[i];
        line.erase(std::remove(line.begin(), line.end(), '('), line.end());
        line.erase(std::remove(line.begin(), line.end(), ')'), line.end());
        line.erase(std::remove(line.begin(), line.end(), ','), line.end());
        line.erase(std::remove(line.begin(), line.end(), '='), line.end());
        std::string current, left, right;
        std::stringstream ss(line);
        ss >> current >> left >> right;
        mappings[current] = std::make_pair(left, right);
    }

    std::string target = "ZZZ";
    std::vector<std::string> current_arr;
    for (const auto& entry : mappings) {
        if (entry.first.back() == 'A') {
            current_arr.push_back(entry.first);
        }
    }

    int seq_count = 0;
    int used = 0;
    while (used < 1000000000) {
        used++;
        // int a;
        // std::cin >> a;
        for (char i : steps) {
            for (std::string& current_pos : current_arr) {
                if (i == 'L') {
                    current_pos = mappings[current_pos].first;
                } else if (i == 'R') {
                    current_pos = mappings[current_pos].second;
                }
            }
        }
        seq_count++;
        
       

        if (seq_count % 10000 == 0) {
            std::cout << seq_count << " ";
             std::cout << "Current Array: ";
        for (const auto& entry : current_arr) {
            std::cout << entry << " ";
        }
        std::cout << std::endl;
        }

        if (std::all_of(current_arr.begin(), current_arr.end(), [](const std::string& s) {
            return s.back() == 'Z';
        })) {
            break;
        }
    }

    std::cout << seq_count * steps.size() << std::endl;

    return 0;
}
