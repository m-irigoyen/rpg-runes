#ifndef _RUNES_RUNEDESCRIPTOR_H_
#define _RUNES_RUNEDESCRIPTOR_H_

#include "Engine/Utilities/Serializable.h"

using namespace std;

namespace Runes
{
	class RuneDescriptor : public Serializable
	{
	public:
		RuneDescriptor();
		RuneDescriptor(QString name, QString naturalName, QString description);

		// Serialization
		virtual bool serialize(QXmlStreamWriter& stream) override;
		virtual bool unserialize(QXmlStreamReader& stream) override;

		QString name_;
		QString naturalName_;
		QString description_;
	};
}


#endif //_RUNES_RUNEDESCRIPTOR_H_