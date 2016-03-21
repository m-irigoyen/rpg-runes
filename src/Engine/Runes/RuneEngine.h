#ifndef _RUNES_RUNEENGINE_H_
#define _RUNES_RUNEENGINE_H_

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
	typedef map<int, RuneDescriptor> UserRunesContainer;
	typedef vector<Rune> RunesContainer;

	class RuneEngine : public QObject
	{
		Q_OBJECT

	public:
		RuneEngine();
		~RuneEngine();

		// Init the runes
		void init();
		void init(QString userName);
		void testInit();

		// clear is called at the end of the application, or before changing profiles
		void clear();

		// save / load spell
		bool save(Spell& spell, QString name, QString userName);
		bool load(Spell& spell, QString name, QString userName);

		// save / load user runes
		bool saveRuneDictionnary(QString userName);
		bool loadRuneDictionnary(QString userName);
		bool saveMasterRuneDictionnary();

		Rune* getRune(int index);
		Rune* getRuneByName(QString name);
		Rune* getRuneByNaturalName(QString naturalName);
		RuneDescriptor* getUserRuneByIndex(int index);
		RuneDescriptor* getUserRuneByNaturalName(QString name);

		Spell* getCurrentSpell();
		vector<Spell*>& getSpells();
		RunesContainer& getRunes();
		UserRunesContainer& getUserRunes();

		void clearSpells();

		// Rune Manager related stuff
		

	public slots:
	void changedSpell();
	void changedProfile();
	void changedRunes();

	void saveChanges();

	void addNewRune();

	private:
		Spell* currentSpell_;	//!< The spell currently being editing (the top level spell)
		RunesContainer runes_;	//!< Dictionnary of the runes
		UserRunesContainer userRunes_;	//!< Personnal dictionnary of the User : his runes, his descriptions, his names, etc
		vector<Spell*> spells_;	//!< All the spells currently loaded in memory

		// modified stuff
		QString currentSpellName_;
		QString currentUserName_;
		bool modifiedSpell_, modifiedProfile_, modifiedRunes_;

		// helper functions
		void checkModifiedMaster();
		void checkModifiedSpell();
		void checkModifiedProfile();
	};
}


#endif //_RUNES_RUNEENGINE_H_