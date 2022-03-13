#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cctype>

void clear(std::string& str) {
    for(std::size_t i = 0, maxi = str.size(); i < maxi; ++i) {
        str[i] = std::tolower(str[i]);
    }
}

int main() {
    std::map<std::string, std::size_t> dict;
    std::vector<std::pair<std::string, std::size_t>> top;
    for(;;) {
        std::string tmp; std::cin >> tmp;
        if(!std::cin) {
            break;
        }

        clear(tmp);
        if(dict.find(tmp) != dict.end()) {
            dict[tmp] += 1u;
        } else {
            dict[tmp] = 1u;
        }
    }

    for(const auto& pair : dict) {
        top.push_back(pair);
    }

    std::sort(top.begin(), top.end(), [](const std::pair<std::string, std::size_t>& lhs, const std::pair<std::string, std::size_t>& rhs) {
        return lhs.second > rhs.second;
    });

    for(const auto& elem : top) {
        std::cout << elem.first << " : " << elem.second << std::endl;
    }
}