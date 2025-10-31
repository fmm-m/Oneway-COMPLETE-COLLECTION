#include "Car.h"

Car::Car(Vec2 Position, float Angle, int CourseLength) {
	position = Position;
	cameraPosition = Position;
	cameraVelocity = Vec2();
	cameraSpeed = 19;
	velocity.x = 0;
	velocity.y = 0;
	friction = 0.97f;
	angle = Angle;
	turningRadius = PI / 96;
	acceleration = 0.85;
	edgeLength = 10;
	decellerateForce = 0.5f;
	zoom = 1;
	checkpoint = 1;
	wallLeeway = 0.98;
	colour = Color((static_cast<float>(rand() % 128)), (static_cast<float>(rand() % 128)), (static_cast<float>(rand() % 128)), 255);
	collideSound = LoadSound("C:\\Users\\tobyb\\source\\repos\\Oneway (LAN)\\Audio\\hitHurt.wav");

	courseLength = CourseLength;


}

float Car::angleChange(float speed) {
	if (speed <= 0.2) { return speed * speed; }
	return 1;
}

bool Car::update(std::vector<Vec2> leftVertices, std::vector<Vec2> rightVertices) {

	

	int gamepad = 0;
	if (IsGamepadAvailable(gamepad)) {
		float leftStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_X);
		float leftStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_Y);
		float rightStickX = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_X);
		float rightStickY = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_Y);
		float leftTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_LEFT_TRIGGER);
		float rightTrigger = GetGamepadAxisMovement(gamepad, GAMEPAD_AXIS_RIGHT_TRIGGER);
		
		

		if (abs(rightStickY) <= 0.17f) { rightStickY = 0; }

		if (abs(leftStickX) <= 0.17f) { leftStickX = 0; }

		zoom = expf(logf(zoom) + ((float)rightStickY * 0.1f));

		angle += leftStickX * turningRadius * angleChange(velocity.length());

		float accelerationMultiplier = ((1 + rightTrigger) / 2) - ((1 + leftTrigger) / 2) * decellerateForce;
		velocity = velocity + (Vec2(angle) * acceleration * accelerationMultiplier);

		//std::cout << leftStickX << "\n";

		


	}

	if (IsKeyDown(KEY_W)) {
		velocity = velocity + (Vec2(angle) * acceleration);
	}
	if (IsKeyDown(KEY_S)) {
		velocity = velocity - (Vec2(angle) * acceleration);
	}

	if (IsKeyDown(KEY_A)) {
		angle -= turningRadius * angleChange(velocity.length());
	}
	if (IsKeyDown(KEY_D)) {
		angle += turningRadius * angleChange(velocity.length());
	}
	position.x += velocity.x;
	position.y += velocity.y;

	

	zoom = 20 / (velocity.length() + 10);

	bool crossedFinishLine = resolveCollisions(leftVertices, rightVertices);

	velocity = velocity * friction;

	
	
	return crossedFinishLine;
}

bool Car::resolveCollisions(std::vector<Vec2> leftVertices, std::vector<Vec2> rightVertices) {
	if (checkpoint != courseLength - 1) {
		if (not isLeft(leftVertices[checkpoint], leftVertices[checkpoint + 1], position) and
			isLeft(rightVertices[checkpoint], rightVertices[checkpoint + 1], position) and
			not isLeft(leftVertices[checkpoint + 1], rightVertices[checkpoint + 1], position) and
			isLeft(leftVertices[checkpoint], rightVertices[checkpoint], position)) { //Inside curr checkpoint

		}
		else {
			if (checkpoint != courseLength - 2) {
				if (not isLeft(leftVertices[checkpoint + 1], leftVertices[checkpoint + 2], position) and
					isLeft(rightVertices[checkpoint + 1], rightVertices[checkpoint + 2], position) and
					not isLeft(leftVertices[checkpoint + 2], rightVertices[checkpoint + 2], position) and
					isLeft(leftVertices[checkpoint + 1], rightVertices[checkpoint + 1], position)) { //Inside next checkpoint
					checkpoint++;
				}
				else if (not isLeft(leftVertices[checkpoint - 1], leftVertices[checkpoint], position) and
					isLeft(rightVertices[checkpoint - 1], rightVertices[checkpoint], position) and
					not isLeft(leftVertices[checkpoint], rightVertices[checkpoint], position) and
					isLeft(leftVertices[checkpoint - 1], rightVertices[checkpoint - 1], position)) { //Inside prev checkpoint
					checkpoint--;
				}

			}
		}
	}

	// This code is convoluted and should probably've been broken down into functions
	// It checks if the player is on the left of the left wall. If they are, it moves them back, rotates them slightly to face in the correct position, and sets their velocity to zero.
	// But the wall its checking against is shifted towards the opposite segment by wallLeeway (1 is its normal spot, 0 is the other segments spot.)

	if (checkpoint >= courseLength - 2) {
		return true;
	}
	bool hit = false;
	if (isLeft(((leftVertices[checkpoint] - rightVertices[checkpoint]) * wallLeeway) + rightVertices[checkpoint], ((leftVertices[checkpoint + 1] - rightVertices[checkpoint + 1]) * wallLeeway) + rightVertices[checkpoint + 1], position)) {
		hit = true;
		
	}
	if (not isLeft(((rightVertices[checkpoint] - leftVertices[checkpoint]) * wallLeeway) + leftVertices[checkpoint], ((rightVertices[checkpoint + 1] - leftVertices[checkpoint + 1]) * wallLeeway) + leftVertices[checkpoint + 1], position)) {

		hit = true;
		
	}

	if (checkpoint == 1) {
		if (not isLeft(rightVertices[0], leftVertices[0], position)) {
			hit = true;
		}
	}

	if (hit) {
		//angle += turningRadius * 5;
		position = position - velocity * 1.1;
		velocity = velocity * -0.5f;
		
	}

	return false;
}

void Car::draw() {

	Vec2 nose =  position + Vec2(angle) * edgeLength;
	Vec2 left =  position + Vec2(angle + PI / 2) * edgeLength;
	Vec2 right = position + Vec2(angle - PI / 2) * edgeLength;

	/*
	DrawTriangle(nose.toRaylib(), right.toRaylib(), left.toRaylib(), colour);
	DrawTriangleLines(nose.toRaylib(), right.toRaylib(), left.toRaylib(), WHITE);
	DrawText(TextFormat("%d", (int)(angle * RAD2DEG)), position.x, position.y, 5, WHITE);
	*/
	DrawCircleV(position.toRaylib(), 12, WHITE);
	DrawCircleV(nose.toRaylib(), 7, WHITE);
	DrawCircleV(position.toRaylib(), 10, colour);
	DrawCircleV(nose.toRaylib(), 5, colour);
	
	
}

void Car::restart(float width, float height) {
	position = Vec2(width / 2 + 20, height / 2);
	velocity = Vec2();
	angle = 0;
	checkpoint = 1;
}
