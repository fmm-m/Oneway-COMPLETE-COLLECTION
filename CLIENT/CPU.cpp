#include "cpu.h"

bool CPU::update(std::vector<Vec2> leftVertices, std::vector<Vec2> rightVertices, std::vector<Vec2> segments) {
	
	
	

	targetPos = segments[checkpoint + 4];

	Vec2 differences = targetPos - (position + velocity * 2);

	float leftDist = abs((leftVertices[checkpoint] - position + velocity * 1).length());
	float rightDist = abs((rightVertices[checkpoint] - position + velocity * 1).length());

	float targetAngle = atan2(differences.y, differences.x);

	
	float signedAngleDiff = signedAngleDist(angle, targetAngle);
	float angleDiff = abs(signedAngleDiff);

	int rubberBandForce = 1;

	if (angleDiff >= 0.005) {
		angle += std::copysignf(1, signedAngleDiff) * turningRadius + (((static_cast<float>(rand() % 200)) / 100) - 1) / 10;

		
	}
	if (angleDiff >= PI / 6 and velocity.length() >= 0.05) { velocity = velocity - (Vec2(angle) * acceleration * decellerateForce); }

	else { velocity = velocity + (Vec2(angle) * acceleration); }
	//velocity = velocity + (Vec2(angle) * acceleration * 1);
	std::cout << "Left Dist: " << leftDist << " || Right Dist: " << rightDist << "\n";

	

	bool cpuCrossed = resolveCollisions(leftVertices, rightVertices);

	velocity = velocity * friction;

	position = position + velocity;

	//angle = std::fmod(angle, 2 * PI);

	
	return cpuCrossed;
	
}

