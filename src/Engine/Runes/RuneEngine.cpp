#include "RuneEngine.h"

namespace Runes
{

RuneEngine::RuneEngine()
{
	this->init();
}

void RuneEngine::init()
{
	// Loading global runes from memory
	if (runes_.empty())
	{
		QXmlStreamReader stream;
		QFile file;

		if (Serializable::openFile("runes.xml", Paths::RUNES, file))
		{
			Serializable::initReader(&file, stream);
			Q_ASSERT(stream.isStartElement() && stream.name() == "runes");
			stream.readNextStartElement();
			for (const QXmlStreamAttribute &attr : stream.attributes())
			{
				if (attr.name().toString() == QLatin1String("runeCount"))
				{
					this->runes_.resize(attr.value().toInt());
				}
			}

			while (stream.readNextStartElement())
			{
				Rune r;
				r.unserialize(stream);

				runes_.at(r.getIndex()) = r;
			}
		}
		else
		{
			// TODO error code
			return;
		}
	}
}

void RuneEngine::init(QString userName)
{
	init();

	//TODO : load user runes
}

bool RuneEngine::save(Spell& spell, QString name, QString userName)
{
	QXmlStreamWriter stream;
	QFile file;
	if (Serializable::openFile(name, Paths::SPELLS + "/" + userName, file))
	{
		Serializable::initWriter(&file, stream);
		spell.serialize(stream);
	}
	else
	{
		//TODO: error code
		return false;
	}
	
	file.close();
	return true;
}

bool RuneEngine::load(Spell& spell, QString name, QString userName)
{
	QXmlStreamReader stream;
	QFile file;
	if (Serializable::openFile(name, Paths::USERS + userName + "/" + Paths::SPELLS, file))
	{
		stream.readNextStartElement();
		while (stream.name() != "spell")
			stream.readNextStartElement();

		spell.unserialize(stream);
		return true;
	}
	
	return false;
}

const Rune RuneEngine::getRune(int index)
{
	if (index >= 0 && index < runes_.size())
	{
		return runes_[index];
	}
	return Rune();
}

const Rune RuneEngine::getRuneByName(QString name)
{
	for (Rune r : runes_)
	{
		if (r.getName().compare(name) == 0)
			return r;
	}
	return Rune();
}

const Rune RuneEngine::getRuneByNaturalName(QString naturalName)
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

RunesContainer& RuneEngine::getRunes()
{
	return runes_;
}

UserRunesContainer& RuneEngine::getUserRunes()
{
	return userRunes_;
}

bool RuneEngine::hasUserDiscoveredRune(int rune)
{
	map<int, UserRune>::iterator it = userRunes_.find(rune);
	Q_ASSERT(it != userRunes_.end());

	if (it->second.second)
		return true;
}


}

