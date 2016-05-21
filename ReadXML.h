#pragma once
#include <Windows.h>
#include "Node.h"
#include <vector>
#include <msclr\marshal_cppstd.h>
#using <System.xml.dll>

using namespace System::Xml;

void read(  std::vector<Node>&);
