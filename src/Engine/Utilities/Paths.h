#ifndef _ENGINE_UTILITIES_PATHS_H_
#define _ENGINE_UTILITIES_PATHS_H_

#define DEBUG

#include <QString>

using namespace std;

namespace Runes
{
	namespace Paths
	{
#ifdef DEBUG 
		const QString RESOURCES = "../res/";
#elif RELEASE
		const string RESOURCES = "res/";
#endif
		const QString SPELLS = "spells/";
		const QString IMAGES(RESOURCES + "images/");
		const QString SOUNDS(RESOURCES + "sounds/");
		const QString RUNES(RESOURCES + "runes/");
		const QString USERS(RESOURCES + "users/");
		const QString EXTENSION(".xml");

		QString getUserPath(QString userName);
		QString getUserSpells(QString userName);
		
	}
}


#endif //_ENGINE_UTILITIES_PATHS_H_