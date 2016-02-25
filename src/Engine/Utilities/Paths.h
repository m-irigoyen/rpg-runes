#ifndef _ENGINE_UTILITIES_PATHS_H_
#define _ENGINE_UTILITIES_PATHS_H_

#define DEBUG

#include <string>

using namespace std;

namespace Runes
{
	namespace Paths
	{
#ifdef DEBUG 
		const string RESOURCES = "../res/";
#elif RELEASE
		const string RESOURCES = "res/";
#endif
		const string SPELLS = RESOURCES + "spells/";
		const string IMAGES = RESOURCES + "images/";
		const string SOUNDS = RESOURCES + "sounds/";
		const string RUNES = RESOURCES + "runes/";
		const string USERS = RESOURCES + "users/";
	}
}


#endif //_ENGINE_UTILITIES_PATHS_H_