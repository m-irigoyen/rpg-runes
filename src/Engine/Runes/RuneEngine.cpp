#include "RuneEngine.h"

namespace Runes
{

Runes::RuneEngine::RuneEngine(User& user)
{
	this->init(user);
}

void Runes::RuneEngine::init(User& user)
{
	// Loading global runes from memory
	if (runes_.empty())
	{
		xml_document doc;
		if (Serializable::readFile(&doc, "runes", Paths::RUNES))
		{
			xml_node& runes = doc.child("runes");
			int runeCount = 0;
			for (xml_node& r : runes.children())
			{
				runes_.push_back(Rune(
					runeCount,
					RuneDescriptor(
						r.attribute("name").as_string(),
						r.attribute("naturalName").as_string(),
						r.attribute("description").as_string()
						)));
				++runeCount;
			}
		}
		else
		{
			// TODO error code
			return;
		}
	}

	// Loading user runes
	{
		xml_document doc;
		string filepath = Paths::USERS;
		filepath += user.getName();
		filepath += "/";
		if (Serializable::readFile(&doc, "runes", filepath))
		{
			xml_node& runes = doc.child("runes");
			for (xml_node& r : runes.children())
			{
				RuneDescriptor rd;
				rd.unserialize(&r.child("descriptor"));

				userRunes_.insert(pair<int, UserRune>(r.attribute("index").as_int(), UserRune(rd, r.attribute("discovered").as_bool())));
			}
		}
		else
		{
			// TODO error code
			return;
		}
	}
}

void RuneEngine::save(Spell & spell, string filename)
{
	xml_document doc;
	spell.serialize(&(doc.append_child("spell")));

	// Building path
	string filePath = Paths::SPELLS;
	filePath += user_->getName();
	filePath += "/";

	// Serializing
	Serializable::writeFile(&doc, filename,  filePath);
}

void RuneEngine::load(string filename, Spell& spell)
{
	xml_document doc;

	// Building path
	string filePath = Paths::SPELLS;
	filePath += user_->getName();
	filePath += "/";

	// Serializing
	if (!Serializable::readFile(&doc, filename, filePath))
	{
		// TODO error code
		return;
	}

	spell.unserialize(&doc.child("spell"));
}

const Rune Runes::RuneEngine::getRune(int index)
{
	if (index >= 0 && index < runes_.size())
	{
		return runes_[index];
	}
	return Rune();
}

const Rune RuneEngine::getRuneByName(string name)
{
	for (Rune r : runes_)
	{
		if (r.getName().compare(name) == 0)
			return r;
	}
	return Rune();
}

const Rune RuneEngine::getRuneByNaturalName(string naturalName)
{
	for (Rune r : runes_)
	{
		if (r.getNaturalName().compare(naturalName) == 0)
			return r;
	}
	return Rune();
}

UserRune RuneEngine::getUserRuneByIndex(int index)
{
	map<int, UserRune>::iterator it = userRunes_.find(index);
	if (it != userRunes_.end())
	{
		return it->second;
	}
}

vector<Spell>& RuneEngine::getSpells()
{
	return spells_;
}


}

