#include "Engine/Engine.h"

using namespace Runes;

int main()
{
	xml_document doc;
	xml_node& node = doc.append_child("test");
	node.append_attribute("index").set_value(0);
	node.append_attribute("name").set_value("Speed");

	Serializable::writeFile(&doc, "test");
	
	return 0;
}