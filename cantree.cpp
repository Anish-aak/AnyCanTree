#include "cantree.hpp"
#include<sstream>

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

void readTimeAllowance(std::vector<double>& time_allowance, const int& speed){
	std::ifstream infile ("poisson.ignore");
	std::string line;
	int count=1e6;

	while(std::getline(infile, line) && count--){
		std::stringstream ls(line);
		double token;
		ls >> token;
		time_allowance.emplace_back(token/speed);
	}
}

int main(int argc, char * argv[]){
    std::vector<std::vector<std::string>> db = scanDB(argv[1],' ');
    std::ofstream f_perf, f_result;

    f_perf.open(std::string(argv[2]));
    f_result.open(std::string(argv[3]));

    int stream_speed = 100;
    std::vector<double> time_allowance;
    readTimeAllowance(time_allowance, stream_speed);

    Node * root = new Node();
    CanTree * tree = new CanTree(root);
    auto start = std::chrono::high_resolution_clock::now();
    // for(auto& v : db){
    //     tree->insert_node(v);
    // }
    for(int i = 0; i < (int)db.size(); i++) {
        tree->insert_node(db[i], time_allowance[i]);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
    std::vector<std::vector<std::string>> x = tree->traversal();
    for(auto& v : x){
        for(auto &str : v) 
            f_result<<str<<" ";
        f_result<<'\n';
    }
    f_perf<<duration.count()<<"\n";
    f_perf.close();
    f_result.close();

}