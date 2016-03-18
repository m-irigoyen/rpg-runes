#include "Rune.h"

namespace Runes
{

Rune::Rune() : index_(-1)
{

}

Rune::Rune(int index, RuneDescriptor descriptor) : index_(index), descriptor_(descriptor)
{

}

void Rune::setIndex(int index)
{
	index_ = index;
}

// Serialization
bool Rune::serialize(QXmlStreamWriter& stream)
{
	stream.writeStartElement("rune");
	stream.writeAttribute(QXmlStreamAttribute("index", QString::number(this->index_)));
	this->descriptor_.serialize(stream);
	stream.writeEndElement();
	return true;
}

bool Rune::unserialize(QXmlStreamReader& stream)
{
	if (stream.name() == "rune")
	{
		for (const QXmlStreamAttribute &attr : stream.attributes())
		{
			if (attr.name().toString() == QLatin1String("index"))
			{
				this->index_ = attr.value().toInt();
			}
		}
		stream.readNextStartElement();


		if (stream.name() == "runeDescriptor")
		{
			this->descriptor_.unserialize(stream);
			stream.readNextStartElement();
			return true;
		}
		return false;
	}
	return false;

	
}

const int Rune::getIndex()
{
	return index_;
}

QString Rune::getName() const
{
	return descriptor_.getName();
}

QString Rune::getNaturalName() const
{
	return descriptor_.getNaturalName();
}

QString Rune::getDescription() const
{
	return descriptor_.getDescription();
}

RuneDescriptor Rune::getDescriptor() const
{
	return descriptor_;
}

void Rune::setName(QString name)
{
	descriptor_.setName(name);
}

void Rune::setNaturalName(QString naturalName)
{
	descriptor_.setNaturalName(naturalName);
}

void Rune::setDescription(QString description)
{
	descriptor_.setNaturalName(description);
}

Runes::RuneDescriptor* Rune::getDescriptorReference()
{
	return &descriptor_;
}

}

