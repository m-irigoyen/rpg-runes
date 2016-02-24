#include "Engine/Engine.h"

using namespace Runes;

int main()
{
	xml_node node;
	node.append_attribute("index").set_value(0);
	node.append_attribute("name").set_value("Speed");

	Serializable::writeFile(&node, "test");
	
	return 0;
}