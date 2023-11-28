
#include "market.h"
#include "rbtree.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

typedef vector<vector<string>> vvs;
typedef vector<string> vs;

using namespace std;

// ----------------------------------------------------------------
vvs sortData(vvs data, bool ascending) {
    // 1 for descending, 0 for ascending.....
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
            } else {
                break;
            }
        }
        data[j + 1] = key;
    }
    return data;
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
        
        ofstream outputFile("output_processed.txt");
        if (outputFile.is_open()) {
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
                    
                    cout<<endl;
                    cout<<"At timestamp : " << entryTime<<endl;
                    cout<<"NAME: "<<stockName<<" option: "<<option<<"Num of stonks: "<<numOfStocks<<" Price: "<<price<<" active time: "<<activeTime<<endl;

                    vvs badavec;
                    vs chotavec;
                    chotavec.push_back(tokens[4].substr(1)); // price
                    chotavec.push_back(to_string(entryTime)); //entrytime
                    chotavec.push_back(tokens[1]); // brokername
                    chotavec.push_back(tokens[5].substr(1)); // numberofstonks
                    chotavec.push_back(to_string(activeTime)); // activetime
                    badavec.push_back(chotavec);

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
                                        n -= numOfStocks;
                                        numOfStocks = 0;
                                    }
                                    else{
                                        numOfStocks -= n;
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
                                // sellTree.printNode(stockName);
                                // for(const auto &s: tas) cout<<s<<"\t";

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

                            //sort
                            // at.push_back(chotavec);

                            // find_in_buy->stockData = sortData(at, 1);
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
                            sellTree.printNode(stockName);
                            vvs& at = lala->stockData;

                            // logikkkk

                            for(auto &z: at){
                                if(numOfStocks > 0 && stoi(z[3]) > 0 && stoi(z[0]) <= price){
                                    // cout<<"No of rem stocks: "<<numOfStocks<<endl;
                                    // for(auto &w: z) cout<<w<<" ";
                                    int n;
                                    stringstream(z[3]) >> n;
                                    // cout<<n<<endl;
                                    if (numOfStocks <= n){
                                        n -= numOfStocks;
                                        numOfStocks = 0;
                                    }
                                    else{
                                        numOfStocks -= n;
                                        n = 0;
                                    }
                                    z[3] = to_string(n);
                                }
                            }

                            // cout<<"No of rem stocks: "<<numOfStocks<<endl;
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
                    cout<<"ST"<<endl;
                    sellTree.printTree();
                    cout<<"BT"<<endl;
                    buyTree.printTree();
                    cout<<"---------------"<<endl;
                    // update krna hai dono trees for time and stonks

                }
                // else
                // {
                    // cerr << "Error: Unexpected number of tokens in the line." << endl;
                // }
            }
        }
        else
        {
            cerr << "Error: Unable to open the output file for writing." << endl;
        }

        File.close();
        outputFile.close();
    }
    else
    {
        cerr << "Error: Unable to open the file." << endl;
    }
}
