#include "Serializable.h"

#include <QFileInfo>

namespace Runes
{
	bool Serializable::openFile(QString fileName, QString filePath, QFile& file)
	{
		QString finalPath = filePath + fileName;
		
		if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
		{
			// TODO : error code
			return false;
		}
		return true;
	}

	void Serializable::initReader(QFile* file, QXmlStreamReader& reader)
	{
		reader.setDevice(file);
		// TODO : check this is a valid spell file
	}

	void Serializable::initWriter(QFile* file, QXmlStreamWriter& writer)
	{
		writer.setAutoFormatting(true);
		writer.setDevice(file);

		writer.writeStartDocument();
		writer.writeDTD("<!DOCTYPE xml>");
		writer.writeStartElement("xml");
	}

	bool Serializable::exists(QString filename, QString filePath)
	{
		return QFileInfo(filePath + filename).exists();
	}
}