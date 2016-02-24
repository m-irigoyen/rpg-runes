#include "Spell.h"

Runes::Spell::Spell(RuneEngine * runeEngine) : runeEngine(runeEngine)
{
}

Runes::Spell::~Spell()
{
	this->clear();
}

void Runes::Spell::clear()
{
	for (Spell* s : this->children)
	{
		s->clear();
		delete(s);
	}
	this->children.clear();
	this->rune = -1;
}

void Runes::Spell::setRune(int rune)
{
	this->rune = rune;
}

void Runes::Spell::setParent(Spell * parent)
{
	if (parent != nullptr)
		this->parent = parent;
}

void Runes::Spell::addChild(Spell * child)
{
	if (child)
	{
		this->children.push_back(child);
	}
}
