#include "Paths.h"

namespace Runes
{
	namespace Paths
	{
		QString getUserPath(QString userName)
		{
			return USERS + userName + "/";
		}

		QString getUserSpells(QString userName)
		{
			return getUserPath(userName) + SPELLS;
		}
	}
}

