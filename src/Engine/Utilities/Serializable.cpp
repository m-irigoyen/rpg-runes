#include "Serializable.h"

using namespace Runes::Paths;

bool Runes::Serializable::writeFile(xml_document* document, string fileName, string filePath)
{
	if (!document)
	{
		cout << "ERROR : invalid document" << endl;
		return false;
	}

	// Set XML declaration
	auto declarationNode = document->append_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "1.0";
	declarationNode.append_attribute("encoding") = "UTF-8";
	declarationNode.append_attribute("standalone") = "yes";

	// Writing to disk
	std::string finalPath = filePath;
	finalPath += fileName;
	finalPath += ".xml";
	if (document->save_file(filePath.data()))
	{
		std::cout << "Saving file " << filePath.data() << "." << std::endl;
		return true;
	}
	else
	{
		std::cout << "ERROR : Saving file " << filePath.data() << " FAILED !" << std::endl;
		return false;
	}
}

bool Runes::Serializable::readFile(xml_document * doc, string fileName, string filePath)
{
	if (!doc)
		return false;

	std::string finalPath = filePath;
	finalPath += fileName;
	finalPath += ".xml";
	if (doc->load_file(finalPath.data()))
	{
		std::cout << "Saving file " << filePath.data() << "." << std::endl;
		return true;
	}
	else
	{
		std::cout << "ERROR : Saving file " << filePath.data() << " FAILED !" << std::endl;
		return false;
	}
}
