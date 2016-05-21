#include "OutputXML.h"

void outputXML(std::vector<Node>& arr,XmlTextWriter^of)
{
	std::vector<Node>::iterator it = arr.begin();


		for (int i = 0; i < arr.size(); ++i)
		{
			System::String^ just_for_output = gcnew System::String(arr[i].getHeadName().c_str());
			of->WriteStartElement(just_for_output);
			of->WriteString("\n\t\t");
			just_for_output = gcnew System::String(arr[i].getNameOne().c_str());
			of->WriteStartElement(just_for_output);
			just_for_output = gcnew System::String(arr[i].getFirstValue().c_str());
			of->WriteString(just_for_output);
			of->WriteEndElement();
			of->WriteString("\n\t\t");
			just_for_output = gcnew System::String(arr[i].getNameTwo().c_str());
			of->WriteStartElement(just_for_output);
			just_for_output = gcnew System::String(arr[i].getSecondValue().c_str());
			of->WriteString(just_for_output);
			of->WriteEndElement();
			of->WriteString("\n\t\t");
			of->WriteEndElement();
			delete just_for_output;
		}




	
}