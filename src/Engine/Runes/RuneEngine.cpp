#include "RuneEngine.h"

#include <QInputDialog>

namespace Runes
{

	RuneEngine::RuneEngine() : currentSpell_(NULL), modifiedSpell_(false), modifiedProfile_(false), modifiedRunes_(false)
{
	this->init();
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
		this->currentSpellName_ = "";
	}
}

void RuneEngine::init(QString userName)
{
	init();
	loadRuneDictionnary(userName);

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

void RuneEngine::clear()
{
	for (Spell* s : spells_)
	{
		if (s != NULL)
			s->clear();
		delete(s);
	}
	spells_.clear();
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
	modifiedSpell_ = false;
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
		currentSpellName_ = name;
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

		for (UserRunesContainer::iterator it = userRunes_.begin(); it != userRunes_.end(); ++it)
		{
			int i = it->first;
			RuneDescriptor rd = it->second;
			Rune r(i, rd);
			r.serialize(stream);
		}
	}
	else
	{
		//TODO: error code
		return false;
	}

	stream.writeEndDocument();
	file.close();
	modifiedProfile_ = false;
	return true;
}

bool RuneEngine::loadRuneDictionnary(QString userName)
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
				Rune r;
				r.unserialize(stream);

				userRunes_.insert(make_pair(r.getIndex(), r.getDescriptor()));
			}
		}
	}
	else
	{
		cout << "RuneEngine::init : failed to load user runes!" << endl;
		return false;
	}
	return true;
}

bool RuneEngine::saveMasterRuneDictionnary()
{
	QXmlStreamWriter stream;
	QFile file;
	if (Serializable::openFile("runes", Paths::RUNES, file))
	{
		Serializable::initWriter(&file, stream);
		file.resize(0);

		// Saving user runes
		stream.writeStartElement("runes");

		for (RunesContainer::iterator it = runes_.begin(); it != runes_.end(); ++it)
		{
			it->serialize(stream);
		}
	}
	else
	{
		//TODO: error code
		return false;
	}

	stream.writeEndDocument();
	file.close();
	modifiedRunes_ = false;
	return true;
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

Runes::RuneDescriptor& RuneEngine::getRuneDescriptorByNaturalName(QString naturalName)
{
	for (Rune r : runes_)
	{
		if (r.getNaturalName().compare(naturalName) == 0)
			return r.getDescriptorReference();
	}
	abort();
}

RuneDescriptor RuneEngine::getUserRuneByIndex(int index)
{
	map<int, RuneDescriptor>::iterator it = userRunes_.find(index);
	if (it != userRunes_.end())
		return it->second;
}

Runes::RuneDescriptor RuneEngine::getUserRuneByNaturalName(QString name)
{
	for (UserRunesContainer::iterator it = userRunes_.begin(); it != userRunes_.end(); ++it)
	{
		if (it->second.getNaturalName().compare(name) == 0)
			return it->second;
	}
}

Runes::RuneDescriptor& RuneEngine::getUserRuneByNaturalName(QString name, UserRunesContainer& container, int* index)
{
	for (UserRunesContainer::iterator it = container.begin(); it != container.end(); ++it)
	{
		if (it->second.getNaturalName().compare(name) == 0)
		{
			if (index != NULL)
				*index = it->first;
			return it->second;
		}
			
	}
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

void RuneEngine::clearSpells()
{
	for (Spell* s : spells_)
	{
		s->clear();
		delete(s);
	}
	spells_.clear();
}

void RuneEngine::addNewRune()
{
	Rune r(runes_.size(), RuneDescriptor("newRune", "newRune", "newRune"));
	runes_.push_back(r);
	changedRunes();
}

void RuneEngine::changedSpell()
{
	modifiedSpell_ = true;
}

void RuneEngine::changedProfile()
{
	modifiedProfile_ = true;
}

void RuneEngine::changedRunes()
{
	modifiedRunes_ = true;
}

void RuneEngine::saveChanges()
{
	if (modifiedRunes_)
		saveMasterRuneDictionnary();
	if (modifiedSpell_)
		save(*currentSpell_, currentSpellName_, currentUserName_);
	if (modifiedProfile_)
		saveRuneDictionnary(currentUserName_);
}

void RuneEngine::checkModifiedMaster()
{
	if (modifiedRunes_)
	{
		QMessageBox runesModifiedDialog;
		runesModifiedDialog.setText("The master rune dictionnary has been modified.");
		runesModifiedDialog.setInformativeText("Do you want to save changes?");
		runesModifiedDialog.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
		runesModifiedDialog.setDefaultButton(QMessageBox::Save);
		int ret = runesModifiedDialog.exec();
		switch (ret)
		{
		case QMessageBox::Save:
			saveMasterRuneDictionnary();
			break;
		}
	}
}

void RuneEngine::checkModifiedSpell()
{
	if (modifiedSpell_)
	{
		QMessageBox spellModifiedDialog;
		spellModifiedDialog.setText("The current spell has been modified.");
		spellModifiedDialog.setInformativeText("Do you want to save changes?");
		spellModifiedDialog.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
		spellModifiedDialog.setDefaultButton(QMessageBox::Save);
		int ret = spellModifiedDialog.exec();
		switch (ret)
		{
		case QMessageBox::Save:
			if (currentSpellName_.isEmpty())
			{
				// Ask the file name
				bool ok;
				QString text = QInputDialog::getText(NULL, tr("Spell name"),
					tr("Spell name:"), QLineEdit::Normal,
					"name", &ok);
				if (ok && !text.isEmpty())
					currentSpellName_ = text;
			}

			save(*currentSpell_, currentSpellName_, currentUserName_);
			break;
		}
	}
}

void RuneEngine::checkModifiedProfile()
{
	if (modifiedProfile_)
	{
		QMessageBox profileModifiedDialog;
		profileModifiedDialog.setText("The rune dictionnary has been modified.");
		profileModifiedDialog.setInformativeText("Do you want to save changes?");
		profileModifiedDialog.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
		profileModifiedDialog.setDefaultButton(QMessageBox::Save);
		int ret = profileModifiedDialog.exec();
		switch (ret)
		{
		case QMessageBox::Save:
			saveRuneDictionnary(currentUserName_);
			break;
		}
	}
}

}

