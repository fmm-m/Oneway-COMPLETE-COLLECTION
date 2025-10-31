#include "Course.h"

time_t Course::Generate(int width, int height) {


	float lastAngle = 0;
	bias = 0;
	segments.clear();
	leftVertices.clear();
	rightVertices.clear();

	segments.push_back(Vec2(width / 2, height / 2));
	leftVertices.push_back(Vec2(width / 2, height / 2));
	rightVertices.push_back(Vec2(width / 2, height / 2));
	seed = time(NULL);
	srand(1000); //Random seed
	for (int i = 1; i < roadLength; i++) {
		float randnum = (((static_cast<float>(rand() % 200)) / 100) - 1) / 10;
		bias = (0.8 * bias) + randnum;
		//bias = 0.25;
		
		bias = std::clamp<float>(bias, -1.0, 1.0);

		Vec2 newSegment = segments.at(i-1); 
		newSegment.x += cos(bias + lastAngle) * segmentDistance;
		newSegment.y += sin(bias + lastAngle) * segmentDistance;
		segments.push_back(newSegment);

		Vec2 newSegmentLeft = segments.at(i - 1);
		newSegmentLeft.x += cos(bias + lastAngle + PI / 2) * segWidth / 2;
		newSegmentLeft.y += sin(bias + lastAngle + PI / 2) * segWidth / 2;
		leftVertices.push_back(newSegmentLeft);

		Vec2 newSegmentRight = segments.at(i - 1);
		newSegmentRight.x += cos(bias + lastAngle - PI / 2) * segWidth / 2;
		newSegmentRight.y += sin(bias + lastAngle - PI / 2) * segWidth / 2;
		rightVertices.push_back(newSegmentRight);

		lastAngle = bias + lastAngle;
		if (newSegment.x > maxCorner.x) { maxCorner.x = newSegment.x; }
		if (newSegment.y > maxCorner.y) { maxCorner.y = newSegment.y; }
		if (newSegment.x < minCorner.x) { minCorner.x = newSegment.x; }
		if (newSegment.y < minCorner.y) { minCorner.y = newSegment.y; }
	}
	Vec2 edge = maxCorner - minCorner;
	Vec2 edge1 = Vec2(300, 300);
	minimapScaleX = edge1.x / edge.x;
	minimapScaleY = edge1.y / edge.y;

	return seed;

	

}

void Course::Draw(int checkpoint) {
	if (checkpoint == -1) {
		for (int i = 0; i < roadLength; i++) {

			DrawCircleV(segments[i].toRaylib(), 1.1 * segWidth / 2, BROWN);
			DrawLineEx(segments[i].toRaylib(), segments[i + 1].toRaylib(), 1.1 * segWidth, BROWN);


		}
		for (int i = 0; i < roadLength; i++) {

			DrawCircleV(segments[i].toRaylib(), segWidth / 2, GRAY);
			DrawLineEx(segments[i].toRaylib(), segments[i + 1].toRaylib(), segWidth, GRAY);

		}
	}
	else {
		for (int i = max(0, checkpoint - 150); i < min(checkpoint + 150, roadLength - 1); i++) {

			DrawCircleV(segments[i].toRaylib(), 1.1 * segWidth / 2, BROWN);
			DrawLineEx(segments[i].toRaylib(), segments[i + 1].toRaylib(), 1.1 * segWidth, BROWN);


		}
		for (int i = max(0, checkpoint - 150); i < min(checkpoint + 150, roadLength - 1); i++) {

			DrawCircleV(segments[i].toRaylib(), segWidth / 2, GRAY);
			DrawLineEx(segments[i].toRaylib(), segments[i + 1].toRaylib(), segWidth, GRAY);

		}

		// Draw bounding box for road
		Color colour;
		for (int i = max(0, checkpoint - 150); i < min(checkpoint + 150, roadLength - 1); i++) {

			if (i % 2 == 0) { colour = RED; }
			else { colour = LIGHTGRAY; }
			DrawCircleV(segments[i].toRaylib(), 5, YELLOW);
			DrawLineEx(leftVertices[i].toRaylib(), leftVertices[i + 1].toRaylib(), 8, colour);
			DrawLineEx(rightVertices[i].toRaylib(), rightVertices[i + 1].toRaylib(), 8, colour);
			DrawLineEx(rightVertices[0].toRaylib(), leftVertices[0].toRaylib(), 8, RAYWHITE);

		}
	}
	
	

	// DEBUG: Draw checkpoint
	/*
	if (checkpoint != 1499) {
		DrawTriangle(leftVertices[checkpoint].toRaylib(), rightVertices[checkpoint + 1].toRaylib(), rightVertices[checkpoint].toRaylib(), LIME);
		DrawTriangle(leftVertices[checkpoint].toRaylib(), leftVertices[checkpoint + 1].toRaylib(), rightVertices[checkpoint + 1].toRaylib(), LIME);
		
	}
	*/


}

void Course::DrawMinimap(Car car) {
	float minimapScale = 0.008;
	for (int i = 0; i < roadLength - 1; i++) {
		
		Vec2 scaledPos = Vec2((segments[i].x - minCorner.x) * minimapScale + 10, (segments[i].y - minCorner.y) * minimapScale + 10);
		//Vec2 scaledPos = Vec2((segments[i].x - minCorner.x) * minimapScaleX + 10, (segments[i].y - minCorner.y) * minimapScaleY + 10);
		DrawCircleV(scaledPos.toRaylib(), 3, BLACK);
	}
	//DrawCircleV(Vec2((car.position.x - minCorner.x) * minimapScaleX + 10, (car.position.y - minCorner.y) * minimapScaleY + 10).toRaylib(), 5, BLUE);
	DrawCircleV(Vec2((car.position.x - minCorner.x) * minimapScale + 10, (car.position.y - minCorner.y) * minimapScale + 10).toRaylib(), 5, BLUE);
	//DrawRectangleLines(0, 0, 300 * minimapScaleX, 300 * minimapScaleY, WHITE);


}

Course::Course(float SegWidth, int RoadLength) {
	segWidth = SegWidth;
	roadLength = RoadLength;
	segmentDistance = 120;

	
	bias = 0;
}