#ifndef _ENGINE_UTILITIES_SERIALIZABLE_H_
#define _ENGINE_UTILITIES_SERIALIZABLE_H_

#include "Paths.h"

#include <QXmlStreamWriter>
#include <QFile>

#include <string>
#include <iostream>

using namespace std;

namespace Runes
{
	class Serializable
	{
	public :
		//! @brief Serializes the content of the object into the given streamWriter. Returns true if succesful, false otherwise
		virtual bool serialize(QXmlStreamWriter& stream) = 0;
		//! @brief Unerializes the content of the object into the given streamWriter. Returns true if succesful, false otherwise
		virtual bool unserialize(QXmlStreamReader& stream) = 0;

		// Static stuff
		//! @brief Opens the file, and creates a read only file
		static bool openFile(QString fileName, QString filePath, QFile& file);

		static void initReader(QFile* file, QXmlStreamReader& reader);
		static void initWriter(QFile* file, QXmlStreamWriter& writer);

		static bool exists(QString filename, QString filePath);
	};
}


#endif //_ENGINE_UTILITIES_SERIALIZABLE_H_