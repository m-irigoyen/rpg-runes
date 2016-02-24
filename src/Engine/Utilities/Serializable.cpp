#include "Serializable.h"

using namespace Runes::Paths;

bool Runes::Serializable::writeFile(xml_node * node, string fileName)
{
	if (!node)
	{
		cout << "ERROR : invalid node" << endl;
		return false;
	}

	// XML Document creation
	pugi::xml_document doc;

	// Set XML declaration
	auto declarationNode = doc.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "1.0";
	declarationNode.append_attribute("encoding") = "UTF-8";
	declarationNode.append_attribute("standalone") = "yes";

	// Set XML root node
	doc.append_child(node->name());

	// Writing to disk
	std::string filePath = SPELLS;
	filePath += fileName;
	if (doc.save_file(filePath.data()))
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

bool Runes::Serializable::readFile(xml_node * node, string fileName)
{
	return false;
}
