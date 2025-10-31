#pragma once

#include "raylib.h"
#include <math.h>
#include <time.h>
#include <iostream>

// Look at inheritance to make them effectively interchangeable

struct Vec2 
{
public:

	Vec2(float x, float y);
	Vec2(float radians);
	Vec2();
	Vec2(Vector2 a);

	friend Vec2 operator+(const Vec2& v1, const Vec2& v2);
	friend Vec2 operator*(const Vec2& v1, const Vec2& v2);
	friend Vec2 operator-(const Vec2& v1, const Vec2& v2);
	friend Vec2 operator/(const Vec2& v1, const Vec2& v2);
	friend Vec2 operator*(const Vec2& v1, const float& f1);
	friend Vec2 operator|(const Vec2& v1, const Vec2& v2);

	float dot(Vec2 v1);
	Vec2 normal();
	

	Vector2 toRaylib();
	Vec2 random();
	float length();
	float x;
	float y;
	

	
};




