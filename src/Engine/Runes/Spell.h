#ifndef _RUNES_SPELL_H_
#define _RUNES_SPELL_H_

#include "Engine/Utilities/Serializable.h"
#include <vector>

using namespace std;

namespace Runes
{
	class RuneEngine;

	class Spell : public Serializable
	{
	public:
		Spell(RuneEngine* runeEngine);
		~Spell();

		//! @brief Clears the Spell : clears and deletes all children, reset rune to none
		void clear();

		//! @brief Set the index of the main rune to the given index.
		void setRune(int rune);

		void setParent(Spell* parent);
		void addChild(Spell* child);

	private:
		RuneEngine* runeEngine;

		int rune; //!< The index of the main rune of this spell
		vector<Spell*> children; //!< The other spells that apply on this one
		Spell* parent; //! The spell this spell applies on. If it's null, then this is the centermost spell
	};
}


#endif //_RUNES_SPELL_H_