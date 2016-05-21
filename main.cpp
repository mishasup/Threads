#include <iostream>
#include <string>
#include "OutputXML.h"
#include "ReadXML.h"
#include <algorithm>
#include <time.h>


void outputXML(std::vector<Node>& arr,XmlTextWriter^);
int val = 0;
HANDLE gMutex;
HANDLE gThread;
void SORT(std::vector<Node>&currentVector);


int main()
{
	std::vector<Node> collection;
	std::vector<Node> collection1;

	clock_t begin, end;

	XmlTextReader^ reader = gcnew XmlTextReader("XMLFile.xml");
	XmlTextReader^ reader1 = gcnew XmlTextReader("XMLFile1.xml");

	std::string rootName;
	std::string rootName1;

	System::Console::WriteLine("First let's try to do this with one thread: ");

	while (reader->NodeType != XmlNodeType::Element)
	{
		reader->Read();
	}
	if (reader->NodeType == XmlNodeType::Element)
	{
		rootName = msclr::interop::marshal_as< std::string >(reader->Name);
	}
	reader->Close();

	begin = clock();
	read(collection);
	end = clock();
	System::Console::WriteLine("First File was read for {0} seconds", (end - begin) / (double)CLOCKS_PER_SEC);


	while (reader1->NodeType != XmlNodeType::Element)
	{
		reader1->Read();
	}
	if (reader1->NodeType == XmlNodeType::Element)
	{
		rootName1 = msclr::interop::marshal_as< std::string >(reader1->Name);
	}
	reader1->Close();

	begin = clock();
	read( collection1);
	end = clock();
	System::Console::WriteLine("Second File was read for {0} seconds", (end - begin) / (double)CLOCKS_PER_SEC);

	collection.clear();
	collection1.clear();

	System::Console::WriteLine("Now let's do it with two threads: ");

	if ((gMutex = CreateMutex(NULL, FALSE, NULL))==NULL)
	{
		System::Console::WriteLine("Mutex creation failed...{0}",GetLastError());
	}

	reader = gcnew XmlTextReader("XMLFile1.xml");
	reader1 = gcnew XmlTextReader("XMLFile1.xml");
	while (reader->NodeType != XmlNodeType::Element)
	{
		reader->Read();
	}
	if (reader->NodeType == XmlNodeType::Element)
	{
		rootName = msclr::interop::marshal_as< std::string >(reader->Name);
	}

	begin = clock();

	if ((gThread=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)read, &collection, 0, 0))==NULL)
	{
		System::Console::WriteLine("Thread creation failed...{0}",GetLastError());
	}

	read(collection1);
	WaitForSingleObject(gThread, INFINITE);
	end = clock();
	System::Console::WriteLine("Both files were read for {0}", (end - begin) / (double)CLOCKS_PER_SEC);
	CloseHandle(gThread);

	System::Console::WriteLine("Now we'll sort both documents! First with one thread: ");
	begin = clock();

	sort(collection.begin(), collection.end());
	sort(collection1.begin(), collection1.end());
	end = clock();
	System::Console::WriteLine("They were sorted for {0} seconds", (end - begin) / (double)CLOCKS_PER_SEC);
	System::Console::WriteLine("Now with both threads:");
	begin = clock();
	gThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SORT, &collection, 0, 0);
	if (gThread == NULL)
	{
		System::Console::WriteLine("Thread creation failed...{0}", GetLastError());
	}
	SORT(collection1);
	WaitForSingleObject(gThread, INFINITE);
	end = clock();

	System::Console::WriteLine("They were sorted for {0} seconds", (end - begin) / (double)CLOCKS_PER_SEC);
	CloseHandle(gThread);

	reader->Close();
	reader1->Close();

	XmlTextWriter^ writer = gcnew XmlTextWriter("XMLFile.xml", nullptr);
	XmlTextWriter^ writer1 = gcnew XmlTextWriter("XMLFile1.xml", nullptr);

	writer->Formatting=Formatting::Indented;
	writer->WriteStartDocument();
	writer1->Formatting = Formatting::Indented;
	writer1->WriteStartDocument();
	System::String^ help = gcnew System::String(rootName.c_str());
	writer->WriteStartElement(help);

	help = gcnew System::String(rootName1.c_str());
	writer1->WriteStartElement(help);

	outputXML(collection, writer);
	outputXML(collection1, writer1);

	writer->WriteEndElement();
	writer1->WriteEndElement();

	writer->Close();
	writer1->Close();
	system("pause");
}

void SORT(std::vector<Node>&currentVector)
{
	WaitForSingleObject(gMutex,INFINITE);
	sort(currentVector.begin(), currentVector.end());
	ReleaseMutex(gMutex);
}