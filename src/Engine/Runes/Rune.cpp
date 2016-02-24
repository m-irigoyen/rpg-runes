#include "Rune.h"

Runes::Rune::Rune(int index, string name) : index_(index), name_(name)
{

}

bool Runes::Rune::serialize(xml_node * node)
{
	//TODO: implement that
	return false;
}

bool Runes::Rune::unserialize(xml_node * node)
{
	//TODO: implement that
	return false;
}

const int Runes::Rune::getIndex()
{
	return index_;
}

const string Runes::Rune::getName()
{
	return name_;
}
