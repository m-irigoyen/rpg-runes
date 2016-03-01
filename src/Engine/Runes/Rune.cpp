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
	Q_ASSERT(stream.isStartElement() && stream.name() == "rune");
	for (const QXmlStreamAttribute &attr : stream.attributes())
	{
		if (attr.name().toString() == QLatin1String("index"))
		{
			this->index_ = attr.value().toInt();
		}
	}
	stream.readNextStartElement();
	
	
	Q_ASSERT(stream.isStartElement() && stream.name() == "runeDescriptor");
	this->descriptor_.unserialize(stream);
	return true;
}

const int Rune::getIndex()
{
	return index_;
}

const QString Rune::getName()
{
	return descriptor_.getName();
}

const QString Rune::getNaturalName()
{
	return descriptor_.getNaturalName();
}

const QString Rune::getDescription()
{
	return descriptor_.getDescription();
}

const RuneDescriptor Rune::getDescriptor()
{
	return descriptor_;
}

}

