#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "vector3.h"
#include "ecs.h"
#include "Quaternion.h"
#include <assert.h>
#include <vector>
#include <list>

typedef enum PrimitiveShape {
	SHAPE_NONE, SHAPE_AABB, SHAPE_SPHERE, SHAPE_CUBE, SHAPE_CAPSULE
} PrimitiveShape;

typedef struct ShapeBase ShapeBase;
typedef struct AABB AABB;

struct LineSegment {
	struct Vector3 start;
	struct Vector3 end;
	LineSegment() {
		this->start = Vector3Zero;
		this->end = Vector3Zero;
	}
	LineSegment(Vector3* start, Vector3* end) {
		this->start = start;
		this->end = end;
	}
};

struct ShapeBase {
	float skin;
	virtual void generateAABB(AABB* output) = 0;
	//virtual void furthestPoint(Vector3* direction) = 0;
};

struct AABB : public ShapeBase {
	struct Vector3 max;
	struct Vector3 min;
	AABB(Vector3* max, Vector3* min) {
		this->max = max;
		this->min = min;
	}
	virtual void generateAABB(AABB* output);
};

struct EmptyShape : public ShapeBase {
	virtual void generateAABB(AABB* output);
};

struct Capsule : public ShapeBase {
	float height;
	float radius;
	Capsule(float height, float radius) {
		this->height = height;
		this->radius = radius;
	}
	void getAxis(LineSegment* output);
	virtual void generateAABB(AABB* output);
};

struct Sphere : public ShapeBase {
	float radius;
	Sphere(float radius) {
		this->radius = radius;
	}
	virtual void generateAABB(AABB* output);
};

struct Cuboid : public ShapeBase {
	struct Vector3 scale;
	struct Quaternion rotation;
	Cuboid(Vector3* scale, Quaternion* rotation) {
		this->scale = scale;
		this->rotation = rotation;
	}
	virtual void generateAABB(AABB* output);
};

struct MeshShape : public ShapeBase {
	std::vector<Vector3> vertices;
};

class Geometry {
	public:
		//static void generateAABBShape(Vector3* center, void* shape, PrimitiveShape shapeType, AABB* out);
		static void generateAABB(std::list<Vector3*>* points, AABB* out);
		static void generateAABB(Vector3* center, Vector3* extents, AABB* out);
		static void transformAABB(AABB* aabb, TransformFields* t, AABB* output);
		static void vectorToLineSegment(Vector3* point, LineSegment* segment, Vector3* output);
		static float sqrDistanceToLineSegment(Vector3* point, LineSegment* segment);
		static float distanceToLineSegment(Vector3* point, LineSegment* segment);
};


#endif