#include "Rune.h"

namespace Runes
{

Runes::Rune::Rune() : index_(-1)
{

}

Runes::Rune::Rune(int index, RuneDescriptor descriptor) : index_(index), descriptor_(descriptor)
{

}

bool Runes::Rune::serialize(xml_node * node)
{
	// No need for that.
	return false;
}

bool Runes::Rune::unserialize(xml_node * node)
{
	if (!node)
	{
		// TODO error code
		return false;
	}

	name_ = node->attribute("name").as_string();
	descriptor_.unserialize(&node->child("description"));
}

void Runes::Rune::setIndex(int index)
{
	index_ = index;
}

const int Runes::Rune::getIndex()
{
	return index_;
}

const string Runes::Rune::getName()
{
	return name_;
}

const string Runes::Rune::getNaturalName()
{
	return descriptor_.getNaturalName();
}

const string Runes::Rune::getDescription()
{
	return descriptor_.getDescription();
}

const RuneDescriptor Runes::Rune::getDescriptor()
{
	return descriptor_;
}

}

