#pragma once
#include<vector>
#include"Node.h"
#include <string>
#include <Windows.h>

class SemaphoreDataHolder
{
private:
	std::vector<Node> *coll;
	std::string mes;
public:
	SemaphoreDataHolder();
	SemaphoreDataHolder(std::vector<Node>&, std::string&);
	std::vector<Node>& getCollection();
	std::string& getMessage();
};