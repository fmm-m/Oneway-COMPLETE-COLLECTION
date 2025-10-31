#pragma once

#include "Vec2.h"
#include <algorithm>
#include <iostream>

Vec2 getMandB(Vec2 p1, Vec2 p2);

Vec2 intersect(Vec2 a1, Vec2 a2, Vec2 b1, Vec2 b2);

bool isLeft(Vec2 a, Vec2 b, Vec2 c);

float signedDistance(Vec2 a, Vec2 b, Vec2 c);

float signedAngleDist(float a, float b); // Returns the difference from A to B (smallest possible)

bool pointInRect(Vec2 target, Vec2 a, Vec2 b, Vec2 c, Vec2 d);