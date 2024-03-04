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
        int count;
        std::unordered_map<std::string, bufferNode> buffer;

        Node(std::string k = "", int c = 0)
            : key(k), count(c){}

};


class CanTree{

    private:
        void interrupt(std::vector<std::string> &tr, int index, Node *cur) {
            std::vector<std::string> toStore(tr.begin() + index + 1, tr.end());
            // cur->buffer[tr[index]] = bufferNode(tr[index], );
            if(cur->buffer.find(tr[index]) == cur->buffer.end()) {
                cur->buffer[tr[index]] = bufferNode(tr[index], 1);
            } else {
                cur->buffer[tr[index]].count += 1;
            }
            cur->buffer[tr[index]].transList.push_back(toStore);
        }

        // void insertNextLevel(Node *t, std::string key, std::vector<std::string> &toStore {
        //     if(t->children.find(p)==t->children.end()){
        //         Node * new_node = new Node(p,1);
        //         t->children[p] = new_node;
        //         size++;
        //     }else{
        //         t->children[p]->count++;
        //     }
        // }

        void hitchHiker(Node *t, std::string p) {
            Node *child = t->children[p];
            for(auto &v: t->buffer[p].transList) {
                child->count += 1;
                if(v.empty()) continue;
                std::vector<std::string> toStore(v.begin() + 1, v.end());
                // insertNextLevel(child, v[0], toStore);
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

    public:
        Node * root;
        int size;

        CanTree(Node * r = nullptr,int c = 0)
            : root(r), size(c){}
        
        ~CanTree(){}

        void insert_node(std::vector<std::string> &tr, double time_allowance){
            Node * t = root;
            auto start = std::chrono::high_resolution_clock::now();
            for(int i = 0; i < (int)tr.size(); i++) {
                auto p = tr[i];
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop-start);
                if(duration.count() > (int)(time_allowance * 1000)) {
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
                t->buffer.erase(p);
                t = t->children[p];
            }
            // for(auto& p : tr){
            //     if(t->children.find(p)==t->children.end()){
            //         Node * new_node = new Node(p,1);
            //         t->children[p] = new_node;
            //         size++;
            //     }else{
            //         t->children[p]->count++;
            //     }
            //     t = t->children[p];
            // }
        }

        std::vector<std::vector<std::string>> traversal(){
            bfsTraversal.clear();
            bfs(root);
            return bfsTraversal;
        }



};

#endif