#ifndef MARKET_H
#define MARKET_H

#include <fstream>
#include <iostream>

using namespace std;
class market
{
public:
	market(int argc, char** argv);
	void start();
private:
	fstream File;
};
#endif
