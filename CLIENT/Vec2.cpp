#include "Vec2.h"

Vec2::Vec2(float X, float Y) {
	x = X;
	y = Y;
}
Vec2::Vec2(float radians) {
	x = cos(radians);
	y = sin(radians);
}
Vec2::Vec2() {
	x = 0;
	y = 0;
}
Vec2::Vec2(Vector2 a) {
	x = a.x;
	y = a.y;
}
Vec2 operator|(const Vec2& v1, const Vec2& v2) {
	return v2.x * v1.x + v2.y * v1.y;
}

Vec2 operator+(const Vec2& v1, const Vec2& v2) {
	return Vec2(v1.x + v2.x, v1.y + v2.y);
}

Vec2 operator-(const Vec2& v1, const Vec2& v2) {
	return Vec2(v1.x - v2.x, v1.y - v2.y);
}

Vec2 operator*(const Vec2& v1, const Vec2& v2) {
	return Vec2(v1.x * v2.x, v1.y * v2.y);
}

Vec2 operator/(const Vec2& v1, const Vec2& v2) {
	return Vec2(v1.x / v2.x, v1.y / v2.y);
}


Vec2 operator*(const Vec2& v1, const float& f1) {
	return Vec2(v1.x * f1, v1.y * f1);
}

Vector2 Vec2::toRaylib() {
	Vector2 rVec;
	rVec.x = x;
	rVec.y = y;
	return rVec;
}

float Vec2::length() {

	return sqrt((x * x) + (y * y));
}

Vec2 Vec2::random() {
	srand(time(NULL));
	return Vec2((((static_cast<float>(rand() % 200)) / 100) - 1), (((static_cast<float>(rand() % 200)) / 100) - 1));
}

Vec2 Vec2::normal() {
	return Vec2(x / length(), y / length());
}



