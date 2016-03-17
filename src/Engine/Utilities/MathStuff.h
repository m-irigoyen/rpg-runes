#ifndef _ENGINE_UTILITIES_MATHSTUFF_H_
#define _ENGINE_UTILITIES_MATHSTUFF_H_

#include <QVector2D>
#define _USE_MATH_DEFINES
#include <math.h>

namespace Runes
{
	namespace Mathstuff
	{
		QVector2D rotate(QVector2D v, float angleInDegrees);

		float degToRad(float angleInDegrees);

		float radToDeg(float angleInRadians);
	}
}



#endif


