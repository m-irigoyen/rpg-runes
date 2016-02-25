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

	class RuneEngine
	{
	public:
		RuneEngine(User& user);

		// Init the runes
		void init(User& user);

		// saves given spell
		void save(Spell& spell, string name);
		void load(string name, Spell& spell);

		const Rune getRune(int index);
		const Rune getRuneByName(string name);
		const Rune getRuneByNaturalName(string naturalName);

		UserRune getUserRuneByIndex(int index);

		vector<Spell>& getSpells();

	private:
		vector<Rune> runes_;	//!< Dictionnary of the runes
		map<int, UserRune> userRunes_;	//!< Personnal dictionnary of the User : his runes, his descriptions, his names, etc
		vector<Spell> spells_;	//!< All the spells currently loaded in memory
		User* user_;	//!< Current user
	};
}


#endif //_RUNES_RUNEENGINE_H_