#include "RuneEngine.h"

#include <QInputDialog>
#include <QFileDialog>

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
					Rune* r = new Rune;
					r->unserialize(stream);
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
		this->spells_.push_back(new Spell(-1));
		this->currentSpell_ = spells_.at(0);
		this->currentSpellName_ = "";
	}
}

void RuneEngine::init(QString userName)
{
	init();
	loadRuneDictionnary(userName);

}

bool RuneEngine::isUserValid(QString name)
{
	if (name.compare(MASTER_NAME) == 0)
	{
		masterMode_ = true;
		return true;
	}

	bool result = false;
	QXmlStreamReader stream;
	QFile file;

	if (Serializable::openFile("runes", Paths::RUNES, file))
	{
		Serializable::initReader(&file, stream);
		stream.readNextStartElement();
		if (stream.isStartElement() && stream.name() == "runes")
		{
			result = true;
			file.close();
		}
	}
	return result;
}

bool RuneEngine::isMasterMode()
{
	return masterMode_;
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

void RuneEngine::checkSave()
{
	checkModifiedMaster();
	checkModifiedProfile();
	checkModifiedSpell();
}

bool RuneEngine::saveSpell(Spell& spell, QString name, QString userName)
{
	if (userName.compare(MASTER_NAME) == 0)
		userName = "Global";

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

bool RuneEngine::loadSpell(Spell& spell, QString name, QString userName)
{
	// check for unsaved changes
	checkModifiedSpell();

	if (userName.compare(MASTER_NAME) == 0)
		userName = "Global";

	// load the new spell
	QXmlStreamReader stream;
	QFile file;
	if (Serializable::openFile(name, Paths::USERS + userName + "/" + Paths::SPELLS, file))
	{
		stream.readNextStartElement();
		while (stream.name() != "spell")
			stream.readNextStartElement();

		spell.unserialize(stream);
		currentSpellName_ = name;
		emit(redrawNeeded());
		return true;
	}
	
	return false;
}

bool RuneEngine::loadSpell(Spell& spell, QString filepath)
{
	// check for unsaved changes
	checkModifiedSpell();

	// load the new spell
	QXmlStreamReader stream;
	QFile file;
	if (Serializable::openFile(filepath, file))
	{
		Serializable::initReader(&file, stream);

		stream.readNextStartElement();
		Q_ASSERT(stream.isStartElement() && stream.name() == "spell");

		spell.unserialize(stream);

		// Extracting spell name
		currentSpellName_ = filepath.split("/").last();
		emit(redrawNeeded());
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

	if (userName.compare(MASTER_NAME) == 0)
	{
		currentUserName_ = userName;
		return true;
	}
		

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

				userRunes_.insert(make_pair(r.getIndex(), r.descriptor_));
			}
		}
	}
	else
	{
		cout << "RuneEngine::init : failed to load user runes!" << endl;
		return false;
	}
	currentUserName_ = userName;
	return true;
}

bool RuneEngine::saveMasterRuneDictionnary()
{
	QXmlStreamWriter stream;
	QFile file;
	if (Serializable::openFile("runes", Paths::RUNES, file))
	{
		file.resize(0);
		Serializable::initWriter(&file, stream);
		

		// Saving user runes
		stream.writeStartElement("runes");

		for (RunesContainer::iterator it = runes_.begin(); it != runes_.end(); ++it)
			(*it)->serialize(stream);
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

Rune* RuneEngine::getRune(int index)
{
	if (index >= 0 && index < runes_.size())
	{
		return runes_[index];
	}
	return NULL;
}

Rune* RuneEngine::getRuneByName(QString name)
{
	for (Rune* r : runes_)
	{
		if (r->descriptor_.name_.compare(name) == 0)
			return r;
	}
	return NULL;
}

Rune* RuneEngine::getRuneByNaturalName(QString naturalName)
{
	for (Rune *r : runes_)
	{
		if (r->descriptor_.naturalName_.compare(naturalName) == 0)
			return r;
	}
	return NULL;
}

RuneDescriptor* RuneEngine::getUserRuneByIndex(int index)
{
	map<int, RuneDescriptor>::iterator it = userRunes_.find(index);
	if (it != userRunes_.end())
		return &it->second;
	return NULL;
}

RuneDescriptor* RuneEngine::getUserRuneByNaturalName(QString naturalName)
{
	for (UserRunesContainer::iterator it = userRunes_.begin(); it != userRunes_.end(); ++it)
	{
		if (it->second.naturalName_.compare(naturalName) == 0)
			return &it->second;
	}
	return NULL;
}

int RuneEngine::getUserRuneIndexByNaturalName(QString naturalName)
{
	if (currentUserName_.compare(MASTER_NAME) == 0)
	{
		for (int i = 0; i < runes_.size(); ++i)
		{
			if (runes_[i]->descriptor_.naturalName_.compare(naturalName) == 0)
				return i;
		}
	}
	else
	{
		for (UserRunesContainer::iterator it = userRunes_.begin(); it != userRunes_.end(); ++it)
		{
			if (it->second.naturalName_.compare(naturalName) == 0)
				return it->first;
		}
	}
	return -1;
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

QStringList RuneEngine::getRuneList()
{
	QStringList list;
	for (Rune* r : runes_)
		list << r->descriptor_.naturalName_;

	return list;
}

QStringList RuneEngine::getUserRuneList()
{
	if (currentUserName_.compare(MASTER_NAME) == 0)
		return this->getRuneList();

	QStringList list;
	for (UserRune ur : userRunes_)
		list << ur.second.naturalName_;

	return list;
}

void RuneEngine::clearSpells()
{
	for (vector<Spell*>::iterator it = spells_.begin() + 1; it != spells_.end(); ++it)
	{
		(*it)->clear();
		delete((*it));
	}
	spells_.resize(1);
	spells_.at(0)->clear();
	emit(redrawNeeded());
}

void RuneEngine::createNewSpell()
{
	checkModifiedSpell();
	clearSpells();
	emit(redrawNeeded());
}

void RuneEngine::discoverRune()
{
	// The player must enter the rune's ancient name
	bool ok;
	QString text = QInputDialog::getText(NULL, tr("Discover rune"),
		tr("Rune ancient name"), QLineEdit::Normal,
		QDir::home().dirName(), &ok);
	if (ok && !text.isEmpty())
	{
		// Find the rune by ancient name
		Rune *r = this->getRuneByName(text);
		if (r != NULL)
		{
			RuneDescriptor rd(text, "newRune", "");
			// inserting the new rune in the user dictionnary
			userRunes_.insert(std::make_pair(r->getIndex(), rd));

			QMessageBox msgBox;
			msgBox.setText("Added new rune : " + text);
			msgBox.exec();
		}
		else
		{
			QMessageBox msgBox;
			msgBox.setText("Couldn't find rune : " + text);
			msgBox.exec();
		}
	}
}

void RuneEngine::loadSpellFromFile()
{
	QString pathToUserSpells = Paths::SPELLS;
	pathToUserSpells += currentUserName_.compare(MASTER_NAME) == 0 ? "Global/" : currentUserName_ + "/";
	/*if (currentUserName_.compare(MASTER_NAME) == 0)
		pathToUserSpells += "Global/";
	else
		pathToUserSpells += currentUserName_ + "/";*/

	QString fileName = QFileDialog::getOpenFileName(NULL, tr("Open spell"), pathToUserSpells, tr("Spell Files (*.xml)"));
	if (!fileName.isEmpty())
	{
		// Clearing extension
		loadSpell(*currentSpell_, fileName);
	}
	emit(redrawNeeded());
}

void RuneEngine::addNewRune()
{
	Rune* r = new Rune(runes_.size(), RuneDescriptor("newRune", "newRune", "newRune"));
	runes_.push_back(r);
	changedRunes();
	emit(redrawNeeded());
}

void RuneEngine::newSpell()
{
	createNewSpell();
}

void RuneEngine::saveCurrentSpell()
{
	checkModifiedSpell();
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
		saveSpell(*currentSpell_, currentSpellName_, currentUserName_);
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

			saveSpell(*currentSpell_, currentSpellName_, currentUserName_);
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

void RuneEngine::clearRunes()
{
	for (Rune* r : runes_)
	{
		delete(r);
	}
	runes_.clear();
}

}

