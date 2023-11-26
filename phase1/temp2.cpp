#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

// Structure to represent an order
struct Order {
    vector<pair<string, int>> stock_combinations;
    int price;
    char type;  // 'b' for buy, 's' for sell
    int line_number;
    Order* next;
    Order* prev;  // Pointer to the previous node

    Order(const vector<pair<string, int>>& combinations, int p, char t, int line) {
        stock_combinations = combinations;
        price = p;
        type = t;
        line_number = line;
        next = nullptr;
        prev = nullptr;
    }
};

// Class to represent a doubly linked list of orders
class OrderList {
public:
    Order* head;
    Order* tail;  // Pointer to the last node

    OrderList() : head(nullptr), tail(nullptr) {}

    // Function to add an order to the back of the linked list
    void addOrder(string orderString, int lineNumber) {
        orderString.pop_back();

        istringstream iss(orderString);
        vector<pair<string, int>> stock_combinations;
        string stock;
        int amt;
        int price;
        char type;

        while (iss >> stock >> amt) {
            stock_combinations.push_back(make_pair(stock, amt));
        }

        type = orderString.back();
        orderString.pop_back();
        orderString.pop_back();

        int x = orderString.length() - 1;
        string k = "";
        while (orderString[x] != ' ') {
            k += orderString[x];
            x--;
        }

        for (size_t i = 0, j = k.length() - 1; i < j; ++i, --j) {
            char temp = k[i];
            k[i] = k[j];
            k[j] = temp;
        }
        price = stoi(k);

        Order* newOrder = new Order(stock_combinations, price, type, lineNumber);
        newOrder->next = nullptr;

        if (head == nullptr) {
            // If the list is empty, make the new order the head and the tail
            head = tail = newOrder;
        } else {
            // Add the new order after the current tail
            newOrder->prev = tail;
            tail->next = newOrder;
            // Update the tail to be the new order
            tail = newOrder;
        }
    }

    // Function to delete a node by its line number
    void deleteNodeByLineNumber(int lineNumber) {
        Order* current = head;
        while (current != nullptr) {
            if (current->line_number == lineNumber) {
                if (current->prev != nullptr) {
                    // Update the next pointer of the previous node
                    current->prev->next = current->next;
                } else {
                    // If the deleted node is the head, update the head
                    head = current->next;
                }

                if (current->next != nullptr) {
                    // Update the previous pointer of the next node
                    current->next->prev = current->prev;
                } else {
                    // If the deleted node is the tail, update the tail
                    tail = current->prev;
                }

                // Delete the node
                delete current;
                break;
            }
            current = current->next;
        }
    }

    // Function to print the entire linked list and all the containing data
    void printList() {
        Order* current = head;
        while (current != nullptr) {
            cout << "Line Number: " << current->line_number << endl;
            cout << "Stock Combinations:" << endl;
            for (const auto& pair : current->stock_combinations) {
                cout << pair.first << " - " << pair.second << "  |  ";
            }
            cout<<endl;
            cout << "Price: " << current->price << ", Type: " << current->type << endl;
            cout << "--------------------------" << endl;
            current = current->next;
        }
    }

    // Function to find a node in linked list by its line number
    Order* findNodeByLineNumber(int lineNumber) {
        Order* current = head;
        while (current != nullptr) {
            if (current->line_number == lineNumber) {
                return current;
            }
            current = current->next;
        }
        return nullptr; // Not found
    }

    void printNodeDetails(Order* node) {
        if (node != nullptr) {
            cout << "Node Details:" << endl;
            cout << "Line Number: " << node->line_number << endl;
            cout << "Stock Combinations:" << endl;
            for (const auto& pair : node->stock_combinations) {
                cout << pair.first << " - " << pair.second << "  |  ";
            }
            cout<<endl;
            cout << "Price: " << node->price << ", Type: " << node->type << endl;
            cout << "--------------------------" << endl;
        } else {
            cout << "Node is nullptr." << endl;
        }
    }
};

int main() {
    OrderList orderList;

    // Example usage: Add orders to the linked list
    orderList.addOrder("A 1 B 3 10 b#", 1);
    orderList.addOrder("A 3 B 1 15 b#", 2);
    orderList.addOrder("A 4 C 5 D 20 5 b#", 3);
    orderList.addOrder("baba 4 re 5 lele 2 69 s#", 4);

    // Print the entire linked list
    cout << "Printing the entire linked list:" << endl;
    orderList.printList();

    // Find a node by its line number
    int lineNumberToFind = 2;
    Order* foundNode = orderList.findNodeByLineNumber(lineNumberToFind);
    if (foundNode != nullptr) {
        cout << "Node with Line Number " << lineNumberToFind << " found!" << endl;
        orderList.printNodeDetails(foundNode);
    } else {
        cout << "Node with Line Number " << lineNumberToFind << " not found." << endl;
    }

    // Delete a node by its line number
    int lineNumberToDelete = 2;
    cout << "Deleting node with Line Number " << lineNumberToDelete << endl;
    orderList.deleteNodeByLineNumber(lineNumberToDelete);

    // Print the updated linked list
    cout << "Printing the updated linked list:" << endl;
    orderList.printList();

    return 0;
}
