#ifndef _USERS_USER_H_
#define _USERS_USER_H_

#include "Engine/Utilities/Serializable.h"
#include "Engine/Runes/Rune.h"

#include <vector>

using namespace std;

namespace Runes
{
	class User : public Serializable
	{
	public:
		User(string name);

		string getName();

	private:
		string name_;	//!< The name of this user
	};
}


#endif //_USERS_USER_H_