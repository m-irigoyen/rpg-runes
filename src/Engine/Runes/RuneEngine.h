#ifndef _RUNES_RUNEENGINE_H_
#define _RUNES_RUNEENGINE_H_

#include "Spell.h"
#include "Rune.h"

namespace Runes
{
	class RuneEngine
	{
	public:
		RuneEngine();

		// Init the runes
		void init();

		//! @brief returns the number of existant runes
		unsigned int getNumberOfRunes();

	private:
		vector<Rune> runes;
		vector<Spell> spells;	//!< All the spells currently loaded
	};
}


#endif //_RUNES_RUNEENGINE_H_