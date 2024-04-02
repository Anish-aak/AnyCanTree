#ifndef CANTREE_HPP
#define CANTREE_HPP

#include<fstream>
#include<iostream>
#include<vector>
#include<algorithm>
#include<string>
#include<unordered_map>
#include<chrono>
#include<queue>
#include<cassert>

#define BUFFER_SIZE 100
using namespace std;

class bufferNode {
    public:
        std::string id;
        int count;
        std::vector<std::vector<std::string>> transList;

        bufferNode(std::string i = "", int c = 0)
            : id(i), count(c){}
};

class Node{
    public:
        std::unordered_map<std::string,Node *> children;
        std::string key;
        int count, endCount, bufferCount;
        std::unordered_map<std::string, bufferNode> buffer;

        Node(std::string k = "", int c = 0, int ec = 0, int bc = 0)
            : key(k), count(c), endCount(ec), bufferCount(bc){}

};


class CanTree{

    private:
        void print_buffer(Node* cur){
            cout<<cur->bufferCount<<" "<<cur->key<<endl;
            for(auto &p : cur->buffer){
                cout<<"parent node -> "<<p.first<<endl;
                for(auto &s : p.second.transList){
                    for(auto &str : s) cout<<str<<" ";
                    cout<<endl;
                }

            }
        }
        void interrupt(std::vector<std::string> &tr, int index, Node *cur) {
            if(cur->bufferCount >= BUFFER_SIZE){
                // cout<<"here"<<endl;
                return;
            }
            // cout<<"interrupt"<<endl;
            cur->bufferCount += 1;
            std::vector<std::string> toStore(tr.begin() + index, tr.end());
            if(cur->buffer.find(tr[index]) == cur->buffer.end()) {
                cur->buffer[tr[index]] = bufferNode(tr[index], 1);
            } else {
                cur->buffer[tr[index]].count += 1;
            }
            cur->buffer[tr[index]].transList.push_back(toStore);
            // print_buffer(cur);
        }


        void hitchHiker(Node *t, std::string p) {
            Node * child = t->children[p];
            t->bufferCount -= t->buffer[p].count;
            for(auto &v: t->buffer[p].transList) {
                child->count += 1;
                if(v.empty()) continue;
                if(child->bufferCount >= BUFFER_SIZE) break;
                child->bufferCount += 1;
                std::vector<std::string> toStore(v.begin(), v.end());
                child->buffer[v[0]].transList.push_back(toStore);
                child->buffer[v[0]].count += 1;
                child->buffer[v[0]] = v[0];
            }
        }

        std::vector<std::vector<std::string>> bfsTraversal;
        void bfs(Node * source){
            std::queue<std::pair<Node *, int>> q;
            q.push({source, 0});
            while(!q.empty()) {
                auto a = q.front();
                q.pop();
                if(bfsTraversal.size() <= a.second) {
                    bfsTraversal.resize(a.second + 1);
                }
                bfsTraversal[a.second].push_back(a.first->key);
                for(auto [u, v]: a.first->children) {
                    q.push({v, a.second + 1});
                }
            }
        }

        std::vector<std::vector<std::string>> dfsTraversal;
        void dfs(Node * root, std::vector<std::string> &tr) {
            if(root == NULL) return;
            tr.push_back(root->key);
            std::vector<std::string> temp;
            for(auto &j: tr) {
                temp.push_back(j);
            }
            for(int i = 0; i < root->endCount; i++) {
                dfsTraversal.push_back(temp);
            }
            for(auto &i: root->children) {
                dfs(i.second, tr);
            }
            tr.pop_back();
        }

    public:
        Node * root;
        int size;

        CanTree(Node * r = nullptr,int c = 0)
            : root(r), size(c){}
        
        ~CanTree(){}

        void insert_node(Node * root, std::vector<std::string> &tr, double time_allowance){
            if(root == NULL || tr.empty()) return;
            Node * t = root;
            std::sort(tr.begin(), tr.end());
            auto start = std::chrono::high_resolution_clock::now();
            for(int i = 0; i < (int)tr.size(); i++) {
                auto p = tr[i];
                if(p.empty())           // FIX
                    continue;
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
                if(duration.count() > (int)(time_allowance * 1000)) {
                    // cout<<"interrupttt"<<" "<<tr[i]<<endl;
                    return interrupt(tr, i, t);
                }
                if(t->children.find(p)==t->children.end()){
                    Node * new_node = new Node(p,1);
                    t->children[p] = new_node;
                    size++;
                }else{
                    t->children[p]->count++;
                }
                hitchHiker(t, p);
                // if(t->buffer.find(p) != t->buffer.end())
                //     t->buffer.erase(p);
                t = t->children[p];
            }
            t->endCount++;
        }

        std::vector<std::vector<std::string>> bfsT(){
            bfsTraversal.clear();
            bfs(root);
            return bfsTraversal;
        }

        std::vector<std::vector<std::string>> dfsT() {
            std::vector<std::string> temp;
            dfs(root, temp);
            return dfsTraversal;
        }

        void flushBuffers(Node * root) {
            if(root == NULL) return;
            for(auto &p: root->buffer) {
                for(auto &q: p.second.transList) {
                    insert_node(root, q, 1e5);
                }
            }
            for(auto &v: root->children) {
                flushBuffers(v.second);
            }
        }

};

#endif