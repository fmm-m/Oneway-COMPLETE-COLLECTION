#pragma once
#include <iostream>
#include <vector>
#include <string>

class Client
{
public:
	std::string username;
	float x;
	float y;
	float angle;

	Client(std::string USERNAME, float X, float Y, float ANGLE);
};

