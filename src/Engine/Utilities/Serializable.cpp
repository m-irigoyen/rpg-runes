#include "Serializable.h"

#include <QFileInfo>

namespace Runes
{
	bool Serializable::openFile(QString fileName, QString filePath, QFile& file)
	{
		QString finalPath = filePath + fileName + Paths::EXTENSION;
		cout << "opening : " << finalPath.toStdString() << endl;
		file.setFileName(finalPath);
		
		if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
		{
			// TODO : error code
			cout << "ERROR : couldn't open file" << endl;
			return false;
		}
		return true;
	}

	void Serializable::initReader(QFile* file, QXmlStreamReader& reader)
	{
		reader.setDevice(file);
		reader.readNextStartElement();
		
		if (reader.name() != "xml")
		{
			cout << "Error : wrong file" << endl;
			while (reader.readNextStartElement())
			{
				cout << "Reading : " << reader.name().toString().toStdString() << endl;
			}
		}
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