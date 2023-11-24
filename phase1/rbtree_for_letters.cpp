#include <iostream>
#include <string>

enum Color { RED, BLACK };

struct Node {
    std::string key;
    Color color;
    Node *left, *right, *parent;

    Node(std::string key) : key(key), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
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
    void insert(std::string key);
    void remove(std::string key);
    void printTree();
};

// Helper function to print the tree (in-order traversal)

void inOrderTraversal(Node *root, int x) {
    if (root != nullptr) {
        inOrderTraversal(root->right, x+1);
        for(int i = 0; i<x; i++){
            std::cout <<"\t";
        }
        std::cout <<"|--"<< root->key <<"\n";
        inOrderTraversal(root->left, x+1);
    }
}

// Public method to print the tree
void RedBlackTree::printTree() {
    inOrderTraversal(root, 0);
    std::cout << std::endl;
}



// Helper function to fix the tree after insertion
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

// Helper function for left rotation
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

// Helper function for right rotation
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

// Helper function to find the minimum node in a subtree
Node *RedBlackTree::minimum(Node *x) {
    while (x->left)
        x = x->left;
    return x;
}

// Helper function to transplant a subtree
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

// Public method to insert a key into the tree
void RedBlackTree::insert(std::string key) {
    Node *z = new Node(key);
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

// Public method to remove a key from the tree
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

int main() {
    RedBlackTree rbTree;

    // Insert 5-letter words
    rbTree.insert("apple_576");
    rbTree.insert("grape_57");
    // rbTree.insert("grape");
    // rbTree.insert("grape");
    rbTree.insert("melon_34");
    rbTree.insert("melon_5675");
    rbTree.insert("melon");
    rbTree.insert("lemon_546");
    rbTree.insert("lemon");
    rbTree.insert("kiwik");

    // Print the tree
    std::cout << "Red-Black Tree: \n";
    rbTree.printTree();

    // Remove a word and print the tree
    rbTree.remove("kiwik");
    std::cout << "After removing 'kiwik': \n";
    rbTree.printTree();

    return 0;
}
