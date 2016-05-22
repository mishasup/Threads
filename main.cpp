#include <iostream>
#include "OutputXML.h"
#include "ReadXML.h"
#include <algorithm>
#include <time.h>
#include "ChangeDATA.h"

//Create variable for mutex and thread, declare function for output data to XML-file, for sort vector data, 
//declare int variable to switch function code blocks 
void outputXML(std::vector<Node>& arr,XmlTextWriter^);
int val = 0;
HANDLE gMutex;
HANDLE gThread;
HANDLE gSemaphore;
void SORT(std::vector<Node>&currentVector);


int main()
{
	//Create two vectors to hold data from XML-Files
	std::vector<Node> collection;
	std::vector<Node> collection1;

	//variables to count time for reading,sorting and output data
	clock_t begin, end;

	//objects to read from XML-Files
	XmlTextReader^ reader = gcnew XmlTextReader("XMLFile.xml");
	XmlTextReader^ reader1 = gcnew XmlTextReader("XMLFile1.xml");

	//root names in both XML-Files
	std::string rootName;
	std::string rootName1;

	System::Console::WriteLine("First let's try to do this with one thread: ");

	//Read from XML-File until we finally find Element-Node
	while (reader->NodeType != XmlNodeType::Element)
	{
		reader->Read();
	}
	if (reader->NodeType == XmlNodeType::Element)
	{
		rootName = msclr::interop::marshal_as< std::string >(reader->Name);
	}
	reader->Close();

	//Continue reading all data until the end of the file, count seconds
	begin = clock();
	read(collection);
	end = clock();
	System::Console::WriteLine("First File was read for {0} seconds", (end - begin) / (double)CLOCKS_PER_SEC);

	//repeat same actions with second file
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

	//clear collections for reading files again,but now with two threads
	collection.clear();
	collection1.clear();

	System::Console::WriteLine("Now let's do it with two threads: ");

	//Try to create mutex to hold thread in function call
	if ((gMutex = CreateMutex(NULL, FALSE, NULL))==NULL)
	{
		System::Console::WriteLine("Mutex creation failed...{0}",GetLastError());
	}

	//open XML-Files again to read from begin
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

	//Try to create second thread andread witj it from first XML-file to first collection
	if ((gThread=CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)read, &collection, 0, 0))==NULL)
	{
		System::Console::WriteLine("Thread creation failed...{0}",GetLastError());
	}
	//When thread reads first file,we try to read second file
	read(collection1);
	WaitForSingleObject(gThread, INFINITE);
	end = clock();
	System::Console::WriteLine("Both files were read for {0}", (end - begin) / (double)CLOCKS_PER_SEC);
	//Close second thread
	CloseHandle(gThread);

	System::Console::WriteLine("Now we'll sort both documents! First with one thread: ");
	begin = clock();

	//Try to sort both collections with one thread
	sort(collection.begin(), collection.end());
	sort(collection1.begin(), collection1.end());
	end = clock();
	System::Console::WriteLine("They were sorted for {0} seconds", (end - begin) / (double)CLOCKS_PER_SEC);
	System::Console::WriteLine("Now with both threads:");
	begin = clock();
	//Create second thread and repeat sorting
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
	//close XML-Files to give access to XML-writers to files
	reader->Close();
	reader1->Close();

	System::Console::WriteLine("Now we will try to change first values in each Node! First with one thread:");
	//change first values in each element's first_value field
	//with one thread first
	begin = clock();
	SemaphoreDataHolder holder(collection, (std::string)"000000000");
	SemaphoreDataHolder holder1(collection1, (std::string)"000000000");
	changeDATA(holder);
	changeDATA(holder1);
	end = clock();
	System::Console::WriteLine("Values were changed for {0} seconds", (end - begin) / (double)CLOCKS_PER_SEC);
	//Create Semaphore for changeDATA function
	gSemaphore = CreateSemaphore(NULL, 1, 2, (LPCWSTR)"Sam_Winchester");
	//Now with two threads
	System::Console::WriteLine("Now with two threads:");
	SemaphoreDataHolder hold(collection, (std::string)"555555555");
	SemaphoreDataHolder hold1(collection1, (std::string)"555555555");
	begin = clock();
	gThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)changeDATA, &hold, 0, 0);
	if (gThread == NULL)
	{
		System::Console::WriteLine("thread creation failed {0}", GetLastError());
	}
	changeDATA(hold1);
	WaitForSingleObject(gThread, INFINITE);
	end = clock();

	System::Console::WriteLine("Both were changed with {0} seconds", (end - begin) / (double)CLOCKS_PER_SEC);
	//Now try to run sort and changeDATA function at the same time with one two threads with turned mutex off
	System::Console::WriteLine("Entering dangerous area: ");
	//CloseHandle(gMutex);
	begin = clock();
	gThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SORT, &collection, 0, 0);
	if (gThread == NULL)
	{
		System::Console::WriteLine("Thread creation failed {0}", GetLastError());
	}
	changeDATA(hold);
	end = clock();
	System::Console::WriteLine("value changed for {0} seconds", (end - begin) / (double)CLOCKS_PER_SEC);
	CloseHandle(gMutex);
	CloseHandle(gSemaphore);
	CloseHandle(gThread);
	//create xml-writers objects
	XmlTextWriter^ writer = gcnew XmlTextWriter("XMLFile.xml", nullptr);
	XmlTextWriter^ writer1 = gcnew XmlTextWriter("XMLFile1.xml", nullptr);
	//set file's begin and root elements by Xml-writers
	writer->Formatting=Formatting::Indented;
	writer->WriteStartDocument();
	writer1->Formatting = Formatting::Indented;
	writer1->WriteStartDocument();
	System::String^ help = gcnew System::String(rootName.c_str());
	writer->WriteStartElement(help);

	help = gcnew System::String(rootName1.c_str());
	writer1->WriteStartElement(help);
	//just call outputXML function to output data from collections to XML-Files
	outputXML(collection, writer);
	outputXML(collection1, writer1);
	//end XML-Files with end elements
	writer->WriteEndElement();
	writer1->WriteEndElement();
	//close XML-Files to save changes
	writer->Close();
	writer1->Close();
	system("pause");
}
//function SORT implementation
void SORT(std::vector<Node>&currentVector)
{
	WaitForSingleObject(gMutex,INFINITE);
	sort(currentVector.begin(), currentVector.end());
	ReleaseMutex(gMutex);
}