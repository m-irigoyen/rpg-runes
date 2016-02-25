#ifndef _ENGINE_UTILITIES_SERIALIZABLE_H_
#define _ENGINE_UTILITIES_SERIALIZABLE_H_

#include "Paths.h"
#include "pugi/pugixml.hpp"

#include <string>
#include <iostream>

using namespace pugi;
using namespace std;

namespace Runes
{
	class Serializable
	{
	public :
		//! @brief Serializes the content of the object into the given node. Returns true if succesful, false otherwise
		virtual bool serialize(xml_node* node) = 0;

		//! @brief Unserializes the content of the given node into the object. Returns true if succesful, false otherwise
		virtual bool unserialize(xml_node* node) = 0;

		static bool writeFile(xml_document* document, string fileName, string filePath);
		static bool readFile(xml_document * doc, string fileName, string filePath);
	};
}


#endif //_ENGINE_UTILITIES_SERIALIZABLE_H_