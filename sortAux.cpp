#include<bits/stdc++.h>

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

int main(int argc, char* argv[]) {
    std::vector<std::vector<std::string>> v = scanDB(argv[1], ' ');
    for(auto &p: v) {
        std::sort(p.begin(), p.end());
    }
    std::sort(v.begin(), v.end());
    for(auto &p: v) {
        for(auto &q: p) {
            std::cout << q << ' ';
        }
        std::cout << '\n';
    }
}