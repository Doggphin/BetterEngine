#include "physics.h"
#include <assert.h>
#include <list>
#include <unordered_map>
#include "ecs.h"

// https://gdbooks.gitbooks.io/3dcollisions/content/

// Applies the opposite of a transformfields
void Physics::applyReverseTransform(Vector3* point, TransformFields* t, Vector3* output, int doRotation) {
	struct Vector3 ret;
	ret = point;
	ret -= t->position;
	if(doRotation) {
		struct Quaternion conjugate;
		t->rotation.conjugate(&conjugate);
		Quaternion::rotate(&conjugate, &ret, output);
	}
	*output /= t->scale;
}
void Physics::applyTransform(Vector3* point, TransformFields* t, Vector3* output, int doRotation) {
	struct Vector3 ret;
	ret = point;
	ret *= t->scale;
	if(doRotation) {
		Quaternion::rotate(&t->rotation, &ret, output);
	}
	*output += t->position;
}

int Physics::pointIsInAABB(Vector3* point, AABB* aabb) {
	return point->x > aabb->min.x &&
		point->y > aabb->min.y &&
		point->z > aabb->min.z &&
		point->x < aabb->max.x &&
		point->y < aabb->max.y &&
		point->z > aabb->max.z;
}
int Physics::pointIsInAABB(Vector3* point, AABB* aabb, TransformFields* t) {
	struct Vector3 newPoint; Physics::applyReverseTransform(point, t, &newPoint, 1);
	return Physics::pointIsInAABB(&newPoint, aabb);
}

int Physics::pointIsInSphere(Vector3* point, Sphere* sphere) {
	return point->sqrmagnitude() <= sphere->radius * sphere->radius;
}
int Physics::pointIsInSphere(Vector3* point, Sphere* sphere, TransformFields* t) {
	struct Vector3 newPoint; Physics::applyReverseTransform(point, t, &newPoint, 0);
	return Physics::pointIsInSphere(&newPoint, sphere);
}

int Physics::pointIsInCapsule(Vector3* point, Capsule* capsule) {
	struct LineSegment line;
	capsule->getAxis(&line);
	return Geometry::sqrDistanceToLineSegment(point, &line) < (capsule->radius * capsule->radius);
}
int Physics::pointIsInCapsule(Vector3* point, Capsule* capsule, TransformFields* t) {
	struct Vector3 newPoint; Physics::applyReverseTransform(point, t, &newPoint, 1);
	return Physics::pointIsInCapsule(&newPoint, capsule);
}

int Physics::AABBIsInAABB(AABB* a, AABB* b) {
	return (a->min.x <= b->max.x && a->max.x >= b->min.x) &&
	(a->min.y <= b->max.y && a->max.y >= b->min.y) &&
	(a->min.z <= b->max.z && a->max.z >= b->min.z);
}
void Physics::closestPointAABB(Vector3* point, AABB* aabb, Vector3* output) {
	assert(output != NULL);
	output = new Vector3(
		point->x > aabb->max.x ? aabb->max.x :
		point->x < aabb->min.x ? aabb->min.x :
		point->x,

		point->y > aabb->max.y ? aabb->max.y :
		point->y < aabb->min.y ? aabb->min.y :
		point->y,

		point->z > aabb->max.z ? aabb->max.z :
		point->z < aabb->min.z ? aabb->min.z :
		point->z
	);
}

void Physics::closestPointSphere(Vector3* point, Sphere* sphere, Vector3* output) {
	if (Physics::pointIsInSphere(point, sphere)) {
		*output = point;
	}
	else {
		// Get magnitude of point relative to origin to normalize it later
		float magnitude = point->magnitude();
		// Normalize vector and scale it by the radius of the sphere
		// This MIGHT cause problems because it's reading and writing to same buffer but I don't think it will. check later
		*output = new Vector3((output->x / magnitude) * sphere->radius, (output->y / magnitude) * sphere->radius, (output->z / magnitude) * sphere->radius);
	}
}
void Physics::closestPointSphere(Vector3* point, Sphere* sphere, TransformFields* t, Vector3* output) {
	struct Vector3 newPoint; Physics::applyReverseTransform(point, t, &newPoint, 0);
	struct Vector3 ret;
	closestPointSphere(&newPoint, sphere, &ret);
	Physics::applyTransform(&ret, t, output, 0);
	// WORK ON ME
}

void Physics::closestPointCapsule(Vector3* point, Capsule* capsule, Vector3* output) {
	struct LineSegment line; capsule->getAxis(&line);
	struct Vector3 toCapsule; Geometry::vectorToLineSegment(point, &line, &toCapsule);
	if(toCapsule.sqrmagnitude() < (capsule->radius * capsule->radius)) {
		*output = point;
	}
	else {
		struct Vector3 pointOnAxis = toCapsule + point;
		// Make toCapsule represent line pointing out of capsule axis towards the point
		toCapsule.normalize();
		toCapsule *= -1;
		toCapsule *= capsule->radius;
		*output = pointOnAxis + toCapsule;
	}
}

int Physics::raycast(Vector3* from, Vector3* to, int layerMask, RaycastResults* out) {
	return 0;
}

int Physics::penetrationSphereInSphere(Sphere* sphereDynamic, TransformFields* dyn, Sphere* sphereStatic, TransformFields* stat, CollisionInfo* out) {
	assert(out != NULL);

	struct Vector3 dynamicToStatic;
	dynamicToStatic = stat->position - dyn->position;
	float sphereDynamicRadius = sphereDynamic->radius * dyn->scale.max();
	float sphereStaticRadius = sphereStatic->radius * stat->scale.max();
	float cumradius = sphereDynamicRadius + sphereStaticRadius;
	float distance = dynamicToStatic.magnitude();

	//printf("Distance is %.6f, cumulative radius is %.6f\n", distance, cumradius);
	if (distance >= cumradius) { return 0; }
	// First, set distance of out distance
	//out->distance = distance;

	// Next, generate normalized fromTo and assign out normal to its negative
	struct Vector3 fromTo = dynamicToStatic;
	dynamicToStatic.normalize();
	out->normal = fromTo;
	out->normal *= -1;

	// Set depth to distance minus their radii
	out->depth = distance - cumradius;

	// Set safeposition to dynamic sphere's center minus depth * the negative fromTo vector
	//out->safePosition = &fromTo;
	//out->safePosition *= out->depth - sphereDynamic->skin;
	//out->safePosition += &sphereDynamic->center;

	// Set point to bounded point trying to go from safeposition to static sphere's center
	out->point = fromTo * sphereDynamicRadius;
	//out->point += &out->safePosition;

	return 1;
}

void Physics::findOverlappingAABBs(Collider* collider, std::list<Collider*>* overlaps) {
	for(auto i : ECS::hashes[typeid(Collider*).hash_code()]) {
		Collider* check = dynamic_cast<Collider*>(i.second);
		if(check != collider && Physics::AABBIsInAABB(collider->colliderAABB, check->colliderAABB)) {
			overlaps->push_back(check);
		}
	}
}

void Physics::findOverlaps(Collider* collider, std::list<Collider*>* overlaps) {

}

int Physics::computePenetration(Collider* a, Collider* b, CollisionInfo* output) {
	struct TransformFields atf; a->getTransformFields(&atf);
	struct TransformFields btf; b->getTransformFields(&btf);
	switch(a->colliderShapeType) {
		case SHAPE_SPHERE:
			switch(b->colliderShapeType) {
				case SHAPE_SPHERE:
					return Physics::penetrationSphereInSphere(static_cast<Sphere*>(a->colliderShape), &atf, static_cast<Sphere*>(b->colliderShape), &btf, output);

				default:
					printf("Unimplemented collision detection for shape types %d and %d!\n", a->colliderShapeType, b->colliderShapeType); return 0;
			}
		default:
			printf("Unimplemented collision detection for shape types %d and %d!\n", a->colliderShapeType, b->colliderShapeType); return 0;
	}
}

void Physics::updateAABBs() {
	int colls=0;
	for(auto i : ECS::hashes[typeid(Collider*).hash_code()]) {
		Collider* collider = static_cast<Collider*>(i.second);
		collider->updateAABB();
		colls++;
	}
}



/*void Physics::findAllOverlappingAABBs(std::unordered_map<int, Component*>* colliders) {
	// Check for all overlaps between all AABBs.
	std::unordered_map<int, Component*>::iterator it = colliders->begin();
	while(it != colliders->end()) {
		Collider* check = dynamic_cast<Collider*>(it->second);

		std::unordered_map<int, Component*>::iterator it2 = it;
		it2++;
		while(it2 != colliders->end()) {
			Collider* base = dynamic_cast<Collider*>(it2->second);
			if(base != check && Physics::AABBIsInAABB(base->colliderAABB, check->colliderAABB)) {	// probably don't need to do check for check != base
					printf("Overlap!\n");
					it2++;
			}

		it++;
		}
	}
}*/
