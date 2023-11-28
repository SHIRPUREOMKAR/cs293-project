#ifndef RBTREE_H
#define RBTREE_H

#include <iostream>
#include <sstream>
#include <vector>
#include <functional>

using namespace std;

// ---------------------------------------------------------------------------------------------------

enum Color { RED, BLACK };

struct Node {
    string name;
    vector<vector<string>> stockData;
    Color color;
    Node *left, *right, *parent;

    Node(string name, vector<vector<string>> stockData) : name(name), stockData(stockData), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};
class RedBlackTree {
private:
    Node *root;

    void insertFixup(Node *z);
    void leftRotate(Node *x);
    void rightRotate(Node *y);
    void transplant(Node *u, Node *v);
    Node *minimum(Node *x);

public:
    RedBlackTree() : root(nullptr) {}
    void insert(string name, vector<vector<string>> stockData);
    void remove(string name);
    void printTree();
    Node* search(string name);
    Node* getRoot();
    void printNode(string proxy);
    void updateTree(Node* root, const string curr_time);
};

void traverse_print(Node *root, int x) {
    if (root == nullptr) return;
    traverse_print(root->right, x+1);
    for (int i = 0; i < x; i++) cout << "\t";
    cout << "|-- " << root->name << ", Data: ";
    for (const auto& dataVec : root->stockData) {
        for (const auto& data : dataVec) cout << data << " ";
        cout << "--- ";
    }
    cout << endl;
    traverse_print(root->left, x+1);
}

void RedBlackTree::printTree() {
    traverse_print(root, 0);
    cout << endl;
}

void RedBlackTree::insertFixup(Node *z) {
    while (z->parent && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(z->parent->parent);
            }
        } else {
            Node *y = z->parent->parent->left;
            if (y && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

void RedBlackTree::leftRotate(Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

Node* RedBlackTree::search(string name) {
    Node* current = root;
    while (current) {
        if (name == current->name) return current;
        else if (name < current->name) current = current->left;
        else current = current->right;
    }
    return nullptr;
}

void RedBlackTree::rightRotate(Node *y) {
    Node *x = y->left;
    y->left = x->right;
    if (x->right) x->right->parent = y;
    x->parent = y->parent;
    if (!y->parent) root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

Node *RedBlackTree::minimum(Node *x) {
    while (x->left) x = x->left;
    return x;
}

void RedBlackTree::transplant(Node *u, Node *v) {
    if (!u->parent) root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    if (v) v->parent = u->parent;
}

void RedBlackTree::insert(string name, vector<vector<string>> stockData) {
    Node *z = new Node(name, stockData);  // Change here
    Node *y = nullptr;
    Node *x = root;

    while (x) {
        y = x;
        if (z->name < x->name) x = x->left;
        else x = x->right;
    }

    z->parent = y;
    if (!y) root = z;
    else if (z->name < y->name) y->left = z;
    else y->right = z;

    insertFixup(z);
}

void RedBlackTree::remove(string name) {
    Node *z = root;
    while (z) {
        if (name == z->name) break;
        else if (name < z->name) z = z->left;
        else z = z->right;
    }

    if (!z) return;

    Node *y = z;
    Color yOriginalColor = y->color;
    Node *x;

    if (!z->left) {
        x = z->right;
        transplant(z, z->right);
    } else if (!z->right) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z)
            x->parent = y;
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    delete z;

    if (yOriginalColor == BLACK)
        while (x != root && x->color == BLACK){
            if (x == x->parent->left) {
                Node *w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if ((!w->left || w->left->color == BLACK) && (!w->right || w->right->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (!w->right || w->right->color == BLACK) {
                        if (w->left)
                            w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->right)
                        w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root; // Terminate the loop
                }
            } else {
                Node *w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if ((!w->right || w->right->color == BLACK) && (!w->left || w->left->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (!w->left || w->left->color == BLACK) {
                        if (w->right)
                            w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->left)
                        w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root; // Terminate the loop
                }
            }

        if (x) x->color = BLACK;
    }
}

void RedBlackTree::printNode(string proxy) {
    Node* p = search(proxy);
    if (p) {
        cout << endl << "|-- Stock Name: " << p->name << ", Stock Data: ";
        for (const auto &dataVec : p->stockData) {
            for (const auto &data : dataVec) cout << data << " ";
            cout << "| ";
        }
        cout << endl;
    } else cout << "Not Found!!";
}

void RedBlackTree::updateTree(Node* node, const string curr_time){
    if (node == nullptr) return;
    else{
        auto& stockData = node->stockData;
        vector<vector<string>> temp;
        for (const auto& subvec : stockData) {
            int value1, value2;
            stringstream(subvec[1]) >> value1;
            stringstream(subvec[4]) >> value2;
            if ((value1 <= stoi(curr_time) && stoi(curr_time) <= value2) && subvec[3] != "0") temp.push_back(subvec);
        }
        stockData = temp;
    }
    updateTree(node->left, curr_time);
    updateTree(node->right, curr_time);
}

Node* RedBlackTree::getRoot(){
    return root;
}

// -------------------------------------------------------------------------------------------------------------------------

struct Broker {
    std::string name;
    int sharesBought;
    int sharesSold;
    int totalMoney;
};

class brokManager {
private:
    std::vector<Broker> brokers;

public:
    bool brokerExists(const std::string& brokerName) const {
        for (const auto& broker : brokers) {
            if (broker.name == brokerName) {
                return true;
            }
        }
        return false;
    }
    void updateOrAddBroker(const std::string& brokerName, int bought, int sold, double price) {
        for (auto& broker : brokers) {
            if (broker.name == brokerName) {
                broker.sharesBought += bought;
                broker.sharesSold += sold;
                broker.totalMoney += price * (sold - bought);
                return;
            }
        }
        int total = price * (sold - bought);
        Broker newBroker{brokerName, bought, sold, total};
        brokers.push_back(newBroker);
    }

    void displayBrokers() const {
        for (const auto& broker : brokers) {
            cout<<broker.name<<" bought "<<broker.sharesBought<<" and sold "<<broker.sharesSold<<" for a net transfer of $"<<broker.totalMoney<<endl;
        }
    }
};

#endif
