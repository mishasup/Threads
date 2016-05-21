#include "ReadXML.h"

extern int val;
extern HANDLE gMutex;

void read(  std::vector<Node>&collection)
{
	WaitForSingleObject(gMutex, INFINITE);
	if (val == 0)
	{
		XmlTextReader^ reader = gcnew XmlTextReader("XMLFile.xml");
		bool check = false;
		
		while (reader->Read())
		{
			if (reader->NodeType != XmlNodeType::EndElement)
			{
				if (!check)
				{
					reader->Read();
					while (reader->NodeType != XmlNodeType::Element)
					{
						reader->Read();
					}
					check = true;
				}
				if (reader->NodeType == XmlNodeType::Element&&check)
				{
					Node currentNode;
					currentNode.setHeadName(msclr::interop::marshal_as< std::string >(reader->Name));
					for (; !currentNode.isFull();)
					{
						reader->Read();
						switch (reader->NodeType)
						{
						case XmlNodeType::Element:
							if (currentNode.hasFirstName())
							{
								currentNode.setNameTwo(msclr::interop::marshal_as< std::string >(reader->Name));
							}
							else
							{
								currentNode.setNameOne(msclr::interop::marshal_as< std::string >(reader->Name));
							}
							break;
						case XmlNodeType::Text:
							if (currentNode.hasFirstVal())
							{
								currentNode.setSecondVal(msclr::interop::marshal_as< std::string >(reader->Value));
							}
							else
							{
								currentNode.setFirstVal(msclr::interop::marshal_as< std::string >(reader->Value));
							}
						}
					}
					collection.push_back(currentNode);
				}
			}
		}
		reader->Close();
		val = 1;
	}
	else
	{
		XmlTextReader^ reader1 = gcnew XmlTextReader("XMLFile1.xml");
		while (reader1->Read())
		{
			if (reader1->NodeType != XmlNodeType::EndElement)
			{
				if (reader1->NodeType == XmlNodeType::Element)
				{
					Node currentNode;
					currentNode.setHeadName(msclr::interop::marshal_as< std::string >(reader1->Name));
					for (; !currentNode.isFull();)
					{
						reader1->Read();
						switch (reader1->NodeType)
						{
						case XmlNodeType::Element:
							if (currentNode.hasFirstName())
							{
								currentNode.setNameTwo(msclr::interop::marshal_as< std::string >(reader1->Name));
							}
							else
							{
								currentNode.setNameOne(msclr::interop::marshal_as< std::string >(reader1->Name));
							}
							break;
						case XmlNodeType::Text:
							if (currentNode.hasFirstVal())
							{
								currentNode.setSecondVal(msclr::interop::marshal_as< std::string >(reader1->Value));
							}
							else
							{
								currentNode.setFirstVal(msclr::interop::marshal_as< std::string >(reader1->Value));
							}
						}
					}
					collection.push_back(currentNode);
				}
			}
		}
		reader1->Close();
		val = 0;
	}
	ReleaseMutex(gMutex);
}