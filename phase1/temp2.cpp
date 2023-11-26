#include <iostream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <algorithm>

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
            cout << endl;
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
            cout << endl;
            cout << "Price: " << node->price << ", Type: " << node->type << endl;
            cout << "--------------------------" << endl;
        } else {
            cout << "Node is nullptr." << endl;
        }
    }

    // Function to check for arbitrage opportunities in the order list
    // Function to check for arbitrage opportunities in the order list
    bool findArbitrage(vector<int>& arbitrageLineNumbers) {
        vector<vector<int>> matrix;  // Matrix to store the coefficients of stocks in each order
        vector<int> constants;       // Vector to store the constant term in each order

        Order* current = head;

        // Construct the matrix and constants vector
        while (current != nullptr) {
            vector<int> row;  // Coefficients of stocks in the order
            for (const auto& pair : current->stock_combinations) {
                row.push_back(pair.second * (current->type == 'b' ? -1 : 1));  // Fix here
            }
            matrix.push_back(row);
            constants.push_back(current->price * (current->type == 'b' ? -1 : 1));  // Fix here
            current = current->next;
        }

        // Add a row with coefficients of stocks in the arbitrage equation (sum of stocks = 0)
        vector<int> arbitrageRow(matrix[0].size(), 0);
        matrix.push_back(arbitrageRow);
        constants.push_back(0);

        // Use Gaussian elimination to solve the system of linear equations
        if (solveSystem(matrix, constants)) {
            // The system has a solution, indicating arbitrage
            for (int i = 0; i < matrix.size() - 1; ++i) {
                int lineNumber = i + 1;  // Line number of the order causing arbitrage
                arbitrageLineNumbers.push_back(lineNumber);
            }
            return true;
        }

        // No solution, indicating no arbitrage
        return false;
    }


    // Function to solve a system of linear equations using Gaussian elimination
    bool solveSystem(vector<vector<int>>& matrix, vector<int>& constants) {
        int numRows = matrix.size();
        int numCols = matrix[0].size();

        for (int col = 0; col < numCols; ++col) {
            int pivotRow = -1;

            // Find the row with the maximum value in the current column
            for (int row = col; row < numRows; ++row) {
                if (matrix[row][col] != 0) {
                    if (pivotRow == -1 || abs(matrix[row][col]) > abs(matrix[pivotRow][col])) {
                        pivotRow = row;
                    }
                }
            }

            if (pivotRow == -1) {
                // No non-zero pivot element in this column, the system is underdetermined
                continue;
            }

            // Swap the current row with the pivot row
            swap(matrix[col], matrix[pivotRow]);
            swap(constants[col], constants[pivotRow]);

            // Make the pivot element 1
            int pivot = matrix[col][col];
            if (pivot != 0) {
                for (int i = col; i < numCols; ++i) {
                    matrix[col][i] /= pivot;
                }
                constants[col] /= pivot;
            }

            // Make the other elements in the current column 0
            for (int i = 0; i < numRows; ++i) {
                if (i != col) {
                    int factor = matrix[i][col];
                    for (int j = col; j < numCols; ++j) {
                        matrix[i][j] -= factor * matrix[col][j];
                    }
                    constants[i] -= factor * constants[col];
                }
            }
        }

        // Check if the system is inconsistent (no solution)
        for (int i = numRows - 1; i >= 0; --i) {
            bool isZeroRow = true;
            for (int j = 0; j < numCols; ++j) {
                if (matrix[i][j] != 0) {
                    isZeroRow = false;
                    break;
                }
            }
            if (isZeroRow && constants[i] != 0) {
                return false;
            }
        }

        return true;
    }
};

int main() {
    OrderList orderList;

    // Example usage: Add orders to the linked list
    orderList.addOrder("X 1 Y -1 10 b#", 1);
    orderList.addOrder("Z -1 Y 1 -15 b#", 2);
    orderList.addOrder("Z 1 X -1 10 b#", 3);

    // Print the entire linked list
    cout << "Printing the entire linked list:" << endl;
    orderList.printList();

    // Find arbitrage opportunities
    vector<int> arbitrageLineNumbers;
    if (orderList.findArbitrage(arbitrageLineNumbers)) {
        cout << "Arbitrage opportunity found! Orders causing arbitrage:" << endl;
        for (int lineNumber : arbitrageLineNumbers) {
            cout << "Line Number: " << lineNumber << endl;
        }
    } else {
        cout << "No arbitrage opportunity found." << endl;
    }

    return 0;
}
