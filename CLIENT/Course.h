#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <raylib.h>
#include <time.h>
#include "Vec2.h"
#include <Color.hpp>
#include "Car.h"



class Course
{
public:

	Course(float SegWidth, int RoadLength);
	time_t Generate(int width, int height);
	void Draw(int checkpoint);
	void DrawMinimap(Car car);
	float minimapScaleX;
	float minimapScaleY;

	float segWidth; // Road width
	int roadLength; // Segment count
	float segmentDistance; // Distance between segments

	

	float bias; // -1 to 1
	time_t seed;

	Vec2 maxCorner;
	Vec2 minCorner;
	std::vector<Vec2> segments;
	std::vector<Vec2> leftVertices;
	std::vector<Vec2> rightVertices;

};




