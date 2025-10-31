#pragma once
#include "raylib.h"
#include <iostream>
#include "Vec2.h"

#if defined(_WIN32)           
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#endif

#include <winsock.h> // or any library that uses Windows.h

#if defined(_WIN32)           // raylib uses these names as function parameters
#undef near
#undef far
#endif

#include <math.h>
#include <format>
#include <vector>
#include "fmTools.h"



class Car
{
public:
	bool update(std::vector<Vec2> leftVertices, std::vector<Vec2> rightVertices);
	bool resolveCollisions(std::vector<Vec2> leftVertices, std::vector<Vec2> rightVertices);
	void restart(float width, float height);
	void draw();
	float angleChange(float speed);

	Vec2 position;
	Vec2 velocity;
	float edgeLength;
	
	Vec2 cameraPosition;
	Vec2 cameraVelocity;
	float cameraSpeed;

	float friction;
	float acceleration;
	float decellerateForce;
	float turningRadius;
	float angle;
	float zoom;
	
	int checkpoint;
	float wallLeeway;
	int courseLength;

	Color colour;

	Sound collideSound;
	Sound accelerateSound;
	

	Car(Vec2 Position, float Angle, int CourseLength);


};


