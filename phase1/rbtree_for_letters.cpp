#include <iostream>
#include <string>
#include <sstream>

enum Color { RED, BLACK };

struct Node {
    std::string key;
    double stockPrice;  // Add stock price field
    double t;
    double s;
    double b;
    Color color;
    Node *left, *right, *parent;

    Node(std::string key, double stockPrice, double t, double s, double b) : key(key), stockPrice(stockPrice), t(t), s(s), b(b), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RedBlackTree {
private:
    Node *root;

    // Helper functions
    void insertFixup(Node *z);
    void leftRotate(Node *x);
    void rightRotate(Node *y);
    void transplant(Node *u, Node *v);
    Node *minimum(Node *x);

public:
    RedBlackTree() : root(nullptr) {}

    // Public interface
    void insert(std::string key, double stockPrice, double t, double s, double b);
    void remove(std::string key);
    void printTree();
    Node* search(std::string key);
};

std::string printNode(Node *proxy){
    std::ostringstream oss;
    oss <<"|--"<< proxy->key << " "<< proxy->stockPrice << " "<< proxy->t << " "<< proxy->s << " "<< proxy->b <<"\n";
    return oss.str();
}

void inOrderTraversal(Node *root, int x) {
    if (root != nullptr) {
        inOrderTraversal(root->right, x+1);
        for(int i = 0; i<x; i++){
            std::cout <<"\t";
        }
        std::cout <<"|--"<< root->key << " "<< root->stockPrice << " "<< root->t << " "<< root->s << " "<< root->b <<"\n";
        // printNode(root);
        inOrderTraversal(root->left, x+1);
    }
}

void RedBlackTree::printTree() {
    inOrderTraversal(root, 0);
    std::cout << std::endl;
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
    if (y->left)
        y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

Node* RedBlackTree::search(std::string key) {
    Node* current = root;

    while (current) {
        if (key == current->key) {
            // Key found
            return current;
        } else if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    // Key not found
    return nullptr;
}

void RedBlackTree::rightRotate(Node *y) {
    Node *x = y->left;
    y->left = x->right;
    if (x->right)
        x->right->parent = y;
    x->parent = y->parent;
    if (!y->parent)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    x->right = y;
    y->parent = x;
}

Node *RedBlackTree::minimum(Node *x) {
    while (x->left)
        x = x->left;
    return x;
}

void RedBlackTree::transplant(Node *u, Node *v) {
    if (!u->parent)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v)
        v->parent = u->parent;
}

void RedBlackTree::insert(std::string key, double stockPrice, double t, double s, double b) {
    Node *z = new Node(key, stockPrice, t, s, b);  // Pass stockPrice to the Node constructor
    Node *y = nullptr;
    Node *x = root;

    while (x) {
        y = x;
        if (z->key < x->key)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;
    if (!y)
        root = z;
    else if (z->key < y->key)
        y->left = z;
    else
        y->right = z;

    insertFixup(z);
}

void RedBlackTree::remove(std::string key) {
    Node *z = root;
    while (z) {
        if (key == z->key)
            break;
        else if (key < z->key)
            z = z->left;
        else
            z = z->right;
    }

    if (!z)
        return;

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
        while (x != root && x->color == BLACK)
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

    if (x)
        x->color = BLACK;
}


    // Function aagar use karne hai for the operations on tree:
    // Add a node      : tree_name.insert("stock_name", stock_price, threshold, s, b)
    // Delete a node   : tree_name.remove("stock_name")
    // Print a tree    : tree_name.printTree();

    // Function aagar use karne hai for the operations on individual stock:
    // Search a stock  : 
    // Node *result = tree_name.search("stock_name")
    // if found, the stock is stored in node result, else nullptr.
    // stock_name = result->key
    // stock_price = result->stockPrice
    // threshold = result->t
    // s = result->s
    // b = result->b


int main() {
    RedBlackTree rbTree;

    rbTree.insert("apple", 150.25, 150.25, INT32_MAX, 0);
    rbTree.insert("grape", 75.50, 75.50, INT32_MAX, 0);
    rbTree.insert("krish", 120.75, 120.75, INT32_MAX, 0);
    rbTree.insert("hutiy", 200.30, 200.30, INT32_MAX, 0);
    rbTree.insert("kiwi", 29.80,  29.80, INT32_MAX, 0);
    rbTree.insert("suruu", 180.00, 180.00, INT32_MAX, 0);
    rbTree.insert("lemen", 90.20,  90.20, INT32_MAX, 0);
    rbTree.insert("lmon", 110.60, 110.60, INT32_MAX, 0);
    rbTree.insert("kiwik", 250.80, 250.80, INT32_MAX, 0);
    rbTree.insert("zigot", 250.80, 250.80, INT32_MAX, 0);

    std::cout << "Red-Black Tree: \n";
    rbTree.printTree();

    // rbTree.remove("lmon");
    // std::cout << "After removing 'lmon': \n";
    // rbTree.printTree();

    std::string stock;
    double stockPrice;
    char option;

    std::string messgs = "zigot 150 s#\nA 140 b#\nlemen 140 s#\nA 120 b#";
    for(int i = 0; i < messgs.length(); i++){
        std::string k = "";
        while(messgs[i] != '#'){
            k += messgs[i];
            i++;
        }
        std::istringstream iss(k);
        iss >> stock >> stockPrice >> option;

        Node* search_result = rbTree.search(stock);
        if(search_result){
            std::cout << printNode(search_result);
        }

        i++;
    }
}
