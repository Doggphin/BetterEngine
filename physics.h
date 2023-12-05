#ifndef PHYSICS
#define PHYSICS

#include "vector3.h"
#include "comp_collider.h"
#include "ecs.h"
#include <unordered_map>
#include <list>
#include <vector>

typedef struct CollisionInfo {
	bool isCollision;
	Vector3 point;
	Vector3 aIntoB;
	Vector3 bIntoA;
	Vector3 normal;
	float depth;
} CollisionInfo;

typedef struct RaycastResults {
	Collider* collider;
	CollisionInfo info;
} RaycastResults;

class Physics {
	private:
		static int penetrationSphereInSphere(Sphere* sphereDynamic, TransformFields* dyn, Sphere* sphereStatic, TransformFields* stat, CollisionInfo* out);
		static void applyReverseTransform(Vector3* point, TransformFields* t, Vector3* output, int doRotation);
		static void applyTransform(Vector3* point, TransformFields* t, Vector3* output, int doRotation);
	public:
		static int AABBIsInAABB(AABB* a, AABB* b);
		static int pointIsInAABB(Vector3* point, AABB* aabb);
		static int pointIsInAABB(Vector3* point, AABB* aabb, TransformFields* t);
		static int pointIsInSphere(Vector3* point, Sphere* sphere);
		static int pointIsInSphere(Vector3* point, Sphere* sphere, TransformFields* t);
		static int pointIsInCapsule(Vector3* point, Capsule* capsule);
		static int pointIsInCapsule(Vector3* point, Capsule* capsule, TransformFields* t);
		static void closestPointAABB(Vector3* point, AABB* aabb, Vector3* output);
		static void closestPointAABB(Vector3* point, AABB* aabb, TransformFields* t, Vector3* output);
		static void closestPointSphere(Vector3* point, Sphere* sphere, Vector3* output);
		static void closestPointSphere(Vector3* point, Sphere* sphere, TransformFields* t, Vector3* output);
		static void closestPointCapsule(Vector3* point, Capsule* capsule, Vector3* output);
		static void closestPointCapsule(Vector3* point, Capsule* capsule, TransformFields* t, Vector3* output);

		static void updateAABBs();
		static void findOverlappingAABBs(Collider* collider, std::list<Collider*>* overlaps);
		static void findOverlaps(Collider* collider, std::list<Collider*>* overlaps);

		static int raycast(Vector3* from, Vector3* to, int layerMask, RaycastResults* out);
		static std::list<CollisionInfo*> overlapSphere(Vector3* position, float radius, int layer);
		static std::list<CollisionInfo*> overlapCapsule(Vector3* point0, Vector3* point1, float radius, int layer);
		static int computePenetration(Collider* a, Collider* b, CollisionInfo* out);
};

#endif