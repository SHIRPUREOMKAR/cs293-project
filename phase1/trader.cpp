#include "receiver.h"

int main() {

    Receiver rcv;
    sleep(5);
    std::string message = rcv.readIML();
    std::cout << message<< "jr5d";
    return 0;
}
