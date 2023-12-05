#include "geometry.h"
#include "macros.h"
#include <cmath>

void Geometry::generateAABB(std::list<Vector3*>* points, AABB* out) {
	struct Vector3 min = points->front();
	struct Vector3 max = points->front();
	for(auto i : *points) {
		min = Vector3(fmin(i->x, min.x), fmin(i->y, min.y), fmin(i->z, min.z));
		max = Vector3(fmax(i->x, max.x), fmax(i->y, max.y), fmax(i->z, max.z));
	}
	out->max = max;
	out->min = min;
}

void Geometry::generateAABB(Vector3* center, Vector3* extents, AABB* out) {
	assert(out != NULL);

	out->max = Vector3(center->x + extents->x / 2, center->y + extents->y / 2, center->z + extents->z / 2);
	out->min = Vector3(center->x - extents->x / 2, center->y - extents->y / 2, center->z - extents->z / 2);
}

void EmptyShape::generateAABB(AABB* output) {
	output = new AABB(new Vector3(0,0,0), new Vector3(0,0,0));
}

void AABB::generateAABB(AABB* output) {
	printf("AABB::GENERATEAABB NOT IMPLEMENTED YET");
}

void Sphere::generateAABB(AABB* output) {
	struct Vector3 newScale = new Vector3(radius, radius, radius);
	Geometry::generateAABB(new Vector3(0,0,0), &newScale, output);
}

void Capsule::generateAABB(AABB* output) {
	printf("making a new AABB\n");

	struct Vector3 topEnd = new Vector3(radius, height + radius, radius);
	struct Vector3 bottomEnd = topEnd * -1;

	std::list<Vector3*> testList;
	testList.push_back(&topEnd);
	testList.push_back(&bottomEnd);

	Geometry::generateAABB(&testList, output);
	output->max.print();
	output->min.print();
}

void Capsule::getAxis(LineSegment* output) {
	struct Vector3 start = (Vector3{0, .5, 0} * height);
	struct Vector3 end = (Vector3{0, -.5, 0} * height);
	output = new LineSegment(&start, &end);
}

void Cuboid::generateAABB(AABB* output) {
	/*float maxX;
	float maxY;
	float maxZ;
	float minX;
	float minY;
	float minZ;
	Vector3 trueScale = t->scale * scale;
	trueScale *= rotation * t->rotation;*/
	printf("Cuboid::generateAABB I dont do anything yet");
}

// https://www.geeksforgeeks.org/minimum-distance-from-a-point-to-the-line-segment-using-vectors/
void Geometry::vectorToLineSegment(Vector3* point, LineSegment* segment, Vector3* output) {
	struct Vector3 AB = segment->start - segment->end;
	struct Vector3 AP = *point - segment->start;
	struct Vector3 BP = *point - segment->end;
	float AB_BP = AB.dot(&BP);
	float AB_AP = AB.dot(&AP);
	if(AB_BP > 0) {
		*output = *point - segment->end;
	}
	if(AB_AP < 0) {
		*output = *point - segment->start;
	}
	else {
		struct Vector3 ret;
		Vector3::cross(&segment->start, &segment->end, &ret);
	}
}

float Geometry::sqrDistanceToLineSegment(Vector3* point, LineSegment* segment) {
	struct Vector3 temp;
	vectorToLineSegment(point, segment, &temp);
	return temp.sqrmagnitude();
}

float Geometry::distanceToLineSegment(Vector3* point, LineSegment* segment) {
	return sqrt(sqrDistanceToLineSegment(point, segment));
}

void Geometry::transformAABB(AABB* aabb, TransformFields* t, AABB* output) {

}
