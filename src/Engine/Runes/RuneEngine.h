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
	typedef pair<RuneDescriptor, Discovered> UserRune;
	typedef map<int, UserRune> UserRunesContainer;
	typedef vector<Rune> RunesContainer;

	class RuneEngine
	{
	public:
		RuneEngine();

		// Init the runes
		void init();
		void init(QString userName);

		// saves given spell
		bool save(Spell& spell, QString name, QString userName);
		bool load(Spell& spell, QString name, QString userName);

		const Rune getRune(int index);
		const Rune getRuneByName(QString name);
		const Rune getRuneByNaturalName(QString naturalName);

		UserRune getUserRuneByIndex(int index);

		vector<Spell>& getSpells();
		RunesContainer& getRunes();
		UserRunesContainer& getUserRunes();

		//! @brief returns true if user has discovered that rune
		bool hasUserDiscoveredRune(int rune);

	private:
		RunesContainer runes_;	//!< Dictionnary of the runes
		UserRunesContainer userRunes_;	//!< Personnal dictionnary of the User : his runes, his descriptions, his names, etc
		vector<Spell> spells_;	//!< All the spells currently loaded in memory
	};
}


#endif //_RUNES_RUNEENGINE_H_