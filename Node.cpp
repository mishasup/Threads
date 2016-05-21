#include "Node.h"

Node::Node():name_of_first_node(""),name_of_second_node(""),first_value(""),second_value(""),head_name("")
{

}

Node::Node(std::string first_name, std::string second_name, std::string first_val, std::string second_val,std::string head)
{
	name_of_first_node = first_name;
	name_of_second_node = second_name;
	first_value = first_val;
	second_value = second_val;
	head_name = head;
}

std::string Node::getHeadName()
{
	return head_name;
}

std::string Node::getNameOne()
{
	return name_of_first_node;
}

std::string Node::getNameTwo()
{
	return name_of_second_node;
}

std::string Node::getFirstValue()
{
	return first_value;
}

std::string Node::getSecondValue()
{
	return second_value;
}

void Node::setNameOne(std::string cur)
{
	name_of_first_node = cur;
}

void Node::setNameTwo(std::string cur)
{
	name_of_second_node = cur;
}

void Node::setFirstVal(std::string cur)
{
	first_value = cur;
}

void Node::setSecondVal(std::string cur)
{
	second_value = cur;
}

void Node::setHeadName(std::string cur)
{
	head_name = cur;
}

bool Node::hasFirstName()
{
	return name_of_first_node != "";
}

bool Node::hasFirstVal()
{
	return first_value != "";
}

bool Node::operator<(Node&cur)
{
	return second_value < cur.second_value;
}

bool Node::operator>(Node&cur)
{
	return !(*this < cur);
}

bool Node::isFull()
{
	return second_value != "";
}

Node::~Node()
{

}