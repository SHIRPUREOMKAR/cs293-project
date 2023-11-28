
#include "market.h"
#include "rbtree.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

typedef vector<vector<string>> vvs;
typedef vector<string> vs;

using namespace std;

vvs sortData(vvs data, bool ascending) {
    int n = data.size();
    for (int i = 1; i < n; ++i) {
        vs key = data[i];
        int j = i - 1;

        while (j >= 0) {
            int num1, num2;
            stringstream(data[j][0]) >> num1;
            stringstream(key[0]) >> num2;

            if ((ascending && num1 > num2) || (!ascending && num1 < num2)) {
                data[j + 1] = data[j];
                --j;
            } else if (num1 == num2) {
                int num3, num4;
                stringstream(data[j][1]) >> num3;
                stringstream(key[1]) >> num4;

                if (num3 > num4) {
                    data[j + 1] = data[j];
                    --j;
                } else if (num3 == num4) {
                    int num5, num6;
                    stringstream(data[j][2]) >> num5;
                    stringstream(key[2]) >> num6;

                    if (num5 > num6) {
                        data[j + 1] = data[j];
                        --j;
                    } else break;
                } else break;
            } else break;
        }
        data[j + 1] = key;
    }
    return data;
}


string const_sentence(string buyer, string seller, string stonkName, int numOfShares, int price){
    string fk = "";
    fk += buyer;
    fk += " purchased ";
    fk += to_string(numOfShares);
    fk += " share of ";
    fk += stonkName;
    fk += " from ";
    fk += seller;
    fk += " for $";
    fk += to_string(price);
    fk += "/share";
    return fk;
}


// ---------------------------------------------------------

market::market(int argc, char** argv)
{
    // You can handle argc and argv if needed
}

void market::start()
{
    File.open("output.txt");

    if (File.is_open())
    {
        string line;
        RedBlackTree buyTree;
        RedBlackTree sellTree;
        
        vs transactions;
        int totalSharesTraded = 0;        
        int totalPaisaa = 0;        
  
            while (getline(File, line)) {
                istringstream iss(line);
                vs tokens;
                // cout<<line<<endl;


                while (iss >> line) tokens.push_back(line);
                
                // cout<<tokens.size()<<endl;
                if (tokens.size() == 7){
                    int entryTime = stoi(tokens[0]);
                    string brokerName = tokens[1];
                    string option = tokens[2];
                    string stockName = tokens[3];
                    double price = stod(tokens[4].substr(1));
                    int numOfStocks = stoi(tokens[5].substr(1));
                    int activeTime = stoi(tokens[6]);
                    if(activeTime == -1) activeTime = INT32_MAX;
                    else activeTime += entryTime;
                    
                    // cout<<endl;
                    // cout<<"At timestamp : " << entryTime<<endl;
                    // cout<<"NAME: "<<stockName<<" option: "<<option<<" Num of stonks: "<<numOfStocks<<" Price: "<<price<<" active time: "<<activeTime<<endl;

                    vvs badavec;
                    vs chotavec;
                    chotavec.push_back(tokens[4].substr(1)); // price
                    chotavec.push_back(to_string(entryTime)); //entrytime
                    chotavec.push_back(tokens[1]); // brokername
                    chotavec.push_back(tokens[5].substr(1)); // numberofstonks
                    chotavec.push_back(to_string(activeTime)); // activetime
                    badavec.push_back(chotavec);

// ---------------------------------------------------------------------------------------------------------------------------------------------

                    if(option == "SELL"){
                        Node* buy_temp = buyTree.search(stockName);
                        if(buy_temp){
                            vvs& at = buy_temp->stockData;

                            // logikkkk

                            for(auto &z: at){
                                if(numOfStocks > 0 && stoi(z[3]) > 0 && stoi(z[0]) >= price){
                                    int n;
                                    stringstream(z[3]) >> n;
                                    if (numOfStocks <= n){
                                        transactions.push_back(const_sentence(z[2], brokerName, stockName, numOfStocks, stoi(z[0])));
                                        totalSharesTraded += numOfStocks;
                                        totalPaisaa += numOfStocks * stoi(z[0]);
                                        n -= numOfStocks;
                                        numOfStocks = 0;
                                    }
                                    else{
                                        transactions.push_back(const_sentence(z[2], brokerName, stockName, n, stoi(z[0])));
                                        totalSharesTraded += n;
                                        numOfStocks -= n;
                                        totalPaisaa += n * stoi(z[0]);
                                        n = 0;
                                    }
                                    z[3] = to_string(n);
                                }
                            }

                            // cout<<numOfStocks<<endl;
                            if(numOfStocks){
                                Node* ta = sellTree.search(stockName);
                                vs tas;
                                tas.push_back(tokens[4].substr(1)); // price
                                tas.push_back(tokens[0]); //entrytime
                                tas.push_back(tokens[1]); // brokername
                                tas.push_back(to_string(numOfStocks)); // numberofstonks
                                tas.push_back(to_string(activeTime)); // activetime

                                if(ta){
                                    vvs sup = ta->stockData;
                                    sup.push_back(tas);
                                    ta->stockData = sortData(sup, 1);
                                }
                                else{
                                // cout<<"hemlo"<<endl;
                                    vvs sup;
                                    sup.push_back(tas);                                    
                                    sellTree.insert(stockName, sup);
                                }
                            }
                        }
                        else{
                            Node* q = sellTree.search(stockName);
                            vvs pl;
                            if(q){
                                pl = q->stockData;
                                pl.push_back(chotavec);
                                q->stockData = sortData(pl, 1);
                            }
                            else{
                                pl.push_back(chotavec);
                                sellTree.insert(stockName, pl);
                            }
                        }
                    }
    // ------------------------------------------------------------------------

                    else if(option == "BUY"){
                        Node* lala = sellTree.search(stockName);
                        if(lala){
                            vvs& at = lala->stockData;

                            // logikkkk

                            for(auto &z: at){
                                if(numOfStocks > 0 && stoi(z[3]) > 0 && stoi(z[0]) <= price){
                                    int n;
                                    stringstream(z[3]) >> n;
                                    if (numOfStocks <= n){
                                        transactions.push_back(const_sentence(brokerName, z[2], stockName, numOfStocks, stoi(z[0])));
                                        totalSharesTraded += numOfStocks;
                                        totalPaisaa += numOfStocks * stoi(z[0]);
                                        n -= numOfStocks;
                                        numOfStocks = 0;
                                    }
                                    else{
                                        transactions.push_back(const_sentence(brokerName, z[2], stockName, n, stoi(z[0])));
                                        totalPaisaa += n * stoi(z[0]);
                                        totalSharesTraded += n;
                                        numOfStocks -= n;
                                        n = 0;
                                    }
                                    z[3] = to_string(n);
                                }
                            }

                            if(numOfStocks){
                                Node* ta = buyTree.search(stockName);
                                vs tas;
                                tas.push_back(tokens[4].substr(1)); // price
                                tas.push_back(tokens[0]); //entrytime
                                tas.push_back(tokens[1]); // brokername
                                tas.push_back(to_string(numOfStocks)); // numberofstonks
                                tas.push_back(to_string(activeTime)); // activetime

                                if(ta){
                                    vvs sup = ta->stockData;
                                    sup.push_back(tas);
                                    ta->stockData = sortData(sup, 0);
                                }
                                else{
                                    vvs sup;
                                    sup.push_back(tas);                                    
                                    buyTree.insert(stockName, sup);
                                }
                            }
                        }
                        else{
                            Node* q = buyTree.search(stockName);
                            vvs pl;
                            if(q){
                                pl = q->stockData;
                                pl.push_back(chotavec);
                                q->stockData = sortData(pl, 0);
                            }
                            else{
                                pl.push_back(chotavec);
                                buyTree.insert(stockName, pl);
                            }
                        }
                    }

                    // filterStonks();
                    
                    // cout<<"Before Update---------------"<<endl;
                    // cout<<"ST"<<endl;
                    // sellTree.printTree();
                    // cout<<"BT"<<endl;
                    // buyTree.printTree();
                    // cout<<"---------------"<<endl;

                    sellTree.updateTree(sellTree.getRoot(), to_string(entryTime));
                    buyTree.updateTree(buyTree.getRoot(), to_string(entryTime));

                    // cout<<"After Update---------------"<<endl;
                    // cout<<"ST"<<endl;
                    // sellTree.printTree();
                    // cout<<"BT"<<endl;
                    // buyTree.printTree();
                    // cout<<"---------------"<<endl;
                    // update krna hai dono trees for time and stonks
                }
                // else
                // {
                //     cerr << "Error: Unexpected number of tokens in the line." << endl;
                // }
            }

        for(auto &x: transactions) cout<<x<<endl;
        cout<<endl<<"---End of Day---"<<endl;
        cout<<"Total Amount of Money Transferred: $"<<totalPaisaa<<endl;
        cout<<"Number of Completed Trades: "<<transactions.size()<<endl;
        cout<<"Number of Shares Traded: "<<totalSharesTraded<<endl;
        
        File.close();
    }
    else
    {
        cerr << "Error: Unable to open the file." << endl;
    }
}
