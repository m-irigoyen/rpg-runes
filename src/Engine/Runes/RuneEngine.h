#ifndef _RUNES_RUNEENGINE_H_
#define _RUNES_RUNEENGINE_H_

#define MASTER_NAME "M4st3R"

#include "Spell.h"
#include "Rune.h"
#include "Engine/Users/User.h"

#include <map>
#include <QObject>
#include <QMessageBox>

using namespace std;

namespace Runes
{
	typedef bool Discovered;
	typedef pair<int, RuneDescriptor> UserRune;
	typedef map<int, RuneDescriptor> UserRunesContainer;
	typedef vector<Rune*> RunesContainer;

	class RuneEngine : public QObject
	{
		Q_OBJECT

	public:
		RuneEngine();
		~RuneEngine();

		// Init the runes
		void init();
		void init(QString userName);
		bool isUserValid(QString name);	// User init
		bool isMasterMode();

		// clear is called at the end of the application, or before changing profiles
		void clear();

		void checkSave();

		// save / load spell
		bool saveSpell(Spell& spell, QString name, QString userName);
		bool loadSpell(Spell& spell, QString name, QString userName);
		bool loadSpell(Spell& spell, QString filepath);

		// save / load user runes
		bool saveRuneDictionnary(QString userName);
		bool loadRuneDictionnary(QString userName);
		bool saveMasterRuneDictionnary();

		Rune* getRune(int index);
		Rune* getRuneByName(QString name);
		Rune* getRuneByNaturalName(QString naturalName);
		RuneDescriptor* getUserRuneByIndex(int index);
		RuneDescriptor* getUserRuneByNaturalName(QString naturalNme);
		int getUserRuneIndexByNaturalName(QString naturalName);

		Spell* getCurrentSpell();
		vector<Spell*>& getSpells();
		RunesContainer& getRunes();
		UserRunesContainer& getUserRunes();
		QStringList getRuneList();
		QStringList getUserRuneList();

		// Spells management
		void clearSpells();
		void createNewSpell();

		// Rune Manager related stuff
		void discoverRune();

	signals:
		void redrawNeeded();

	public slots:
	void changedSpell();
	void changedSpellButDontRedraw();
	void changedProfile();
	void changedRunes();

	void saveChanges();

	void addNewRune();

	void loadSpellFromFile();
	void newSpell();
	void saveCurrentSpell();

	private:
		Spell* currentSpell_;	//!< The spell currently being editing (the top level spell)
		RunesContainer runes_;	//!< Dictionnary of the runes
		UserRunesContainer userRunes_;	//!< Personnal dictionnary of the User : his runes, his descriptions, his names, etc
		vector<Spell*> spells_;	//!< All the spells currently loaded in memory
		bool masterMode_;

		// modified stuff
		QString currentSpellName_;
		QString currentUserName_;
		bool modifiedSpell_, modifiedProfile_, modifiedRunes_;

		// helper functions
		void checkModifiedMaster();
		void checkModifiedSpell();
		void checkModifiedProfile();
		void clearRunes();
	};
}


#endif //_RUNES_RUNEENGINE_H_