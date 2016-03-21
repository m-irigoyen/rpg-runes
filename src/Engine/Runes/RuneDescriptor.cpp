#include "RuneDescriptor.h"

namespace Runes
{

	RuneDescriptor::RuneDescriptor()
	{
	}

	RuneDescriptor::RuneDescriptor(QString name, QString naturalName, QString description) : name_(name), naturalName_(naturalName), description_(description)
	{
	}

	// Serialization
	bool RuneDescriptor::serialize(QXmlStreamWriter& stream)
	{
		stream.writeStartElement("runeDescriptor");
		stream.writeAttribute(QXmlStreamAttribute("name", this->name_));
		stream.writeAttribute(QXmlStreamAttribute("naturalName", this->naturalName_));
		stream.writeAttribute(QXmlStreamAttribute("description", this->description_));
		stream.writeEndElement();
		return true;
	}

	bool RuneDescriptor::unserialize(QXmlStreamReader& stream)
	{
		Q_ASSERT(stream.isStartElement() && stream.name() == "runeDescriptor");
		for(const QXmlStreamAttribute &attr : stream.attributes()) 
		{
			if (attr.name().toString() == QLatin1String("name")) 
			{
				this->name_ = attr.value().toString();
			}
			else if (attr.name().toString() == QLatin1String("naturalName")) 
			{
				this->naturalName_ = attr.value().toString();
			}
			else if (attr.name().toString() == QLatin1String("description")) 
			{
				this->description_ = attr.value().toString();
			}
		}
		stream.readNextStartElement();
		return true;
	}
}