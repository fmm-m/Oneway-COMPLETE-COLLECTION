#include "Car.h"
#include <algorithm>
#include <math.h>
#include <iostream>
#include "fmTools.h"
#include <time.h>


class CPU : public Car {
public:
	bool update(std::vector<Vec2> leftVertices, std::vector<Vec2> rightVertices, std::vector<Vec2> segments);
	Vec2 targetPos;
	using Car::Car;

	int* firstPlayer;

};