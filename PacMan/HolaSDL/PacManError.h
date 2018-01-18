#pragma once
#include <iostream>  
using namespace std;


class PacManError : public logic_error
{
public:
	PacManError(const string& _Message);
	~PacManError();
};

