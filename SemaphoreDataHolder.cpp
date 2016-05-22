#include "SemaphoreDataHolder.h"

SemaphoreDataHolder::SemaphoreDataHolder() :mes("") 
{
	coll = nullptr;
}

SemaphoreDataHolder::SemaphoreDataHolder(std::vector<Node>&arr, std::string &a)
{
	mes = a;
	coll = &arr;
}

std::vector<Node>& SemaphoreDataHolder::getCollection()
{
	return *coll;
}

std::string& SemaphoreDataHolder::getMessage()
{
	return mes;
}