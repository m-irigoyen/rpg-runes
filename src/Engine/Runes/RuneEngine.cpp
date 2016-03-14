#include "RuneEngine.h"

namespace Runes
{

	RuneEngine::RuneEngine() : currentSpell_(NULL)
{
	this->init();

	//this->testInit();
}

RuneEngine::~RuneEngine()
{

}

void RuneEngine::init()
{
	spells_.clear();
	userRunes_.clear();
	currentSpell_ = NULL;

	// Loading global runes from memory
	if (runes_.empty())
	{
		QXmlStreamReader stream;
		QFile file;

		if (Serializable::openFile("runes", Paths::RUNES, file))
		{
			Serializable::initReader(&file, stream);
			stream.readNextStartElement();
			Q_ASSERT(stream.isStartElement() && stream.name() == "runes");

			while (!stream.atEnd())
			{
				stream.readNextStartElement();

				if (stream.isStartElement() && stream.name() == "rune")
				{
					Rune r;
					r.unserialize(stream);
					runes_.push_back(r);
					stream.readNextStartElement();
					stream.readNextStartElement();
				}
			}
		}
		else
		{
			// TODO error code
			return;
		}
	}

	if (spells_.empty())
	{
		this->spells_.push_back(new Spell(0));
		this->currentSpell_ = spells_.at(0);
	}
}

void RuneEngine::init(QString userName)
{
	init();

	// Loading user runes
	{
		QXmlStreamReader stream;
		QFile file;

		if (Serializable::openFile("profile", Paths::USERS + userName + "/", file))
		{
			Serializable::initReader(&file, stream);
			stream.readNextStartElement();
			Q_ASSERT(stream.isStartElement() && stream.name() == "user");

			for (const QXmlStreamAttribute &attr : stream.attributes())
			{
				if (attr.name().toString() == QLatin1String("name"))
				{
					Q_ASSERT(attr.value().toString() == userName);
				}
			}

			while (!stream.atEnd())
			{
				stream.readNextStartElement();

				if (stream.isStartElement() && stream.name() == "rune")
				{
					bool discovered;

					for (const QXmlStreamAttribute &attr : stream.attributes())
					{
						if (attr.name().toString() == QLatin1String("discovered"))
						{
							discovered = attr.value().toInt();
						}
					}

					Rune r;
					r.unserialize(stream);

					userRunes_.insert(make_pair(r.getIndex(), r.getDescriptor()));

					stream.readNextStartElement(); // exiting rune descriptor element
					stream.readNextStartElement(); // exiting rune element
				}
			}
		}
		else
		{
			cout << "RuneEngine::init : failed to load user runes!" << endl;
			return;
		}
	}

}

void RuneEngine::testInit()
{
	init("Edrevan");
	Spell* s = new Spell(1);
	s->addComponent(new Spell(2));
	s->addComponent(new Spell(2));
	s->addComponent(new Spell(2));
	s->addChild(new Spell(0));
	s->addChild(new Spell(1));
	s->addChild(new Spell(2));
	currentSpell_->addComponent(s);
}

bool RuneEngine::save(Spell& spell, QString name, QString userName)
{
	QXmlStreamWriter stream;
	QFile file;
	if (Serializable::openFile(name, Paths::SPELLS + userName + "/", file))
	{
		Serializable::initWriter(&file, stream);
		spell.serialize(stream);
	}
	else
	{
		//TODO: error code
		return false;
	}

	stream.writeEndDocument();
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

bool RuneEngine::saveRuneDictionnary(QString userName)
{
	QXmlStreamWriter stream;
	QFile file;
	if (Serializable::openFile("profile", Paths::USERS + userName + "/", file))
	{
		Serializable::initWriter(&file, stream);

		// Saving user runes
		stream.writeStartElement("user");
		stream.writeAttribute(QXmlStreamAttribute("name", userName));

		for (int i = 0; i < userRunes_.size(); ++i)
		{
			// TODO FINISH THIS
		}

	}
	else
	{
		//TODO: error code
		return false;
	}

	stream.writeEndDocument();
	file.close();
	return true;
}

bool RuneEngine::loadRuneDictionnary(QString userName)
{

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

RuneDescriptor RuneEngine::getUserRuneByIndex(int index)
{
	map<int, RuneDescriptor>::iterator it = userRunes_.find(index);
	if (it != userRunes_.end())
		return it->second;
}

Spell* RuneEngine::getCurrentSpell()
{
	return this->currentSpell_;
}

vector<Spell*>& RuneEngine::getSpells()
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

void RuneEngine::clearSpells()
{
	for (Spell* s : spells_)
	{
		s->clear();
		delete(s);
	}
	spells_.clear();
}


}

