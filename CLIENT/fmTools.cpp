#include "fmTools.h"

Vec2 getMandB(Vec2 p1, Vec2 p2) { // M is the graidnet and B is the y intercept
	float m = (p1.y - p2.y) / (p1.x - p2.x);

	float b = p1.y - m * p1.x;

	return Vec2(m, b);
}

Vec2 intersect(Vec2 a1, Vec2 a2, Vec2 b1, Vec2 b2) {

	Vec2 aMB = getMandB(a1, a2);
	float aM = aMB.x;
	float aB = aMB.y;

	Vec2 bMB = getMandB(b1, b2);
	float bM = bMB.x;
	float bB = bMB.y;

	float iX = (bB - aB) / (aM - bM);
	float iY = aM * iX + aB;

	return Vec2(iX, iY);

}

bool isLeft(Vec2 a, Vec2 b, Vec2 c) {
	return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) > 0;
}

float signedDistance(Vec2 a, Vec2 b, Vec2 c) {
	return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
}

float signedAngleDist(float a, float b) {
	a = a * RAD2DEG;
	b = b * RAD2DEG;

	float d = fmodf((b - a + 180), 360) - 180;
	return d < -180 ? (d + 360) * DEG2RAD: d * DEG2RAD;
}

bool pointInRect(Vec2 target, Vec2 a, Vec2 b, Vec2 c, Vec2 d) {
	return (
		not isLeft(a, b, target) and
		not isLeft(b, c, target) and
		not isLeft(c, d, target) and
		not isLeft(d, a, target)

		);
}