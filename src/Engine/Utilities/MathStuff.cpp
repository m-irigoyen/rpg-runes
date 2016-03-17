#include "MathStuff.h"

namespace Runes
{
	namespace Mathstuff
	{
		QVector2D rotate(QVector2D v, float angleInDegrees)
		{
			float theta = degToRad(angleInDegrees);

			float cs = cos(theta);
			float sn = sin(theta);

			QVector2D res(v);
			res.setX(res.x() * cs - res.y() * sn);
			res.setY(res.x() * sn + res.y() * cs);
			return res;
		}

		float degToRad(float angleInDegrees)
		{
			// 360°				= 2pi radians
			return (2 * M_PI * angleInDegrees) / 360.0f;
		}

		float radToDeg(float angleInRadians)
		{
			// 2pi radians		= 360°
			// angleInRadians	= x
			return (angleInRadians*360.0f) / (2 * M_PI);
		}
	}
}