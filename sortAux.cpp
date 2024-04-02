#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

std::vector<std::vector<std::string>> scanDB(const std::string& path, char separation) {
    std::vector<std::vector<std::string>> db;
    std::ifstream f(path);
    std::string line;
    while (getline(f, line)) {
        std::vector<std::string> transaction;
        size_t start = 0, end = 0;
        while ((end = line.find(separation, start)) != std::string::npos) {
            transaction.push_back(line.substr(start, end - start));
            start = end + 1;
        }
        transaction.push_back(line.substr(start));
        db.push_back(transaction);
    }
    f.close();
    return db;
}

bool compare(std::vector<std::string> &a, std::vector<std::string> &b) {
    for(int i = 1; i < std::min((int)a.size(), (int)b.size()); i++) {
        if(a[i] != b[i]) 
            return a[i] < b[i];
    }
    return 0;
}

int main(int argc, char* argv[]) {
    std::vector<std::vector<std::string>> v = scanDB(argv[1], ' ');
    for(auto &p: v) {
        std::sort(p.begin() + 1, p.end());
    }
    sort(v.begin(), v.end(), compare);
    for(auto &p: v) {
        bool skipSize = 0;
        for(auto &q: p) {
            if(skipSize == 0) {
                skipSize = 1;
                continue;
            }
            std::cout << q << ' ';
        }
        std::cout << '\n';
    }
}