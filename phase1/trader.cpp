#include "receiver.h"
using namespace std;

int main(int argc , char** argv) {

    Receiver rcv;
    sleep(5);
    std::string message = rcv.readIML();
    std::cout << message;

    if(stoi(argv[1]) == 1){
        //part1
    }

    if(stoi(argv[1]) == 2){
        //part2
    }

    if(stoi(argv[1]) == 3){
        //part3
    }


    return 0;
}
