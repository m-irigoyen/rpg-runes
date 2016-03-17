#ifndef _RUNES_RUNEENGINE_H_
#define _RUNES_RUNEENGINE_H_

#include "Spell.h"
#include "Rune.h"
#include "Engine/Users/User.h"

#include <map>

using namespace std;

namespace Runes
{
	typedef bool Discovered;
	typedef map<int, RuneDescriptor> UserRunesContainer;
	typedef vector<Rune> RunesContainer;

	class RuneEngine
	{
	public:
		RuneEngine();
		~RuneEngine();

		// Init the runes
		void init();
		void init(QString userName);
		void testInit();

		// save / load spell
		bool save(Spell& spell, QString name, QString userName);
		bool load(Spell& spell, QString name, QString userName);

		// save / load user runes
		bool saveRuneDictionnary(QString userName);
		bool loadRuneDictionnary(QString userName);

		const Rune getRune(int index);
		const Rune getRuneByName(QString name);
		const Rune getRuneByNaturalName(QString naturalName);

		RuneDescriptor getUserRuneByIndex(int index);
		RuneDescriptor getUserRuneByNaturalName(QString name);
		static RuneDescriptor& getUserRuneByNaturalName(QString name, UserRunesContainer& container, int* index = NULL);

		
		Spell* getCurrentSpell();
		vector<Spell*>& getSpells();
		RunesContainer& getRunes();
		UserRunesContainer& getUserRunes();

		void clearSpells();

	private:
		Spell* currentSpell_;	//!< The spell currently being editing (the top level spell)
		RunesContainer runes_;	//!< Dictionnary of the runes
		UserRunesContainer userRunes_;	//!< Personnal dictionnary of the User : his runes, his descriptions, his names, etc
		vector<Spell*> spells_;	//!< All the spells currently loaded in memory

		bool profileLoaded_;
	};
}


#endif //_RUNES_RUNEENGINE_H_