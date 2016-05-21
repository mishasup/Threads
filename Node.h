#pragma once
#include <string>

class Node
{
private:
	std::string name_of_first_node;
	std::string name_of_second_node;
	std::string first_value;
	std::string second_value;
	std::string head_name;
public:
	Node();
	Node(std::string, std::string, std::string, std::string,std::string);
	~Node();
	std::string getNameOne();
	std::string getNameTwo();
	std::string getFirstValue();
	std::string getSecondValue();
	std::string getHeadName();
	void setNameOne(std::string);
	void setNameTwo(std::string);
	void setFirstVal(std::string);
	void setSecondVal(std::string);
	void setHeadName(std::string);
	bool hasFirstName();
	bool hasFirstVal();
	bool isFull();
	bool operator<(Node&);
	bool operator>(Node&);
};