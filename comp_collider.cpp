#include "comp_collider.h"
#include "macros.h"
#include "ecs.h"
#include "geometry.h"

void Collider::updateAABB() {

	// Run checks to see if AABB needs to be updated at all
	if(colliderShapeType == SHAPE_NONE) {
		return;
	}

	int recalculateAABB = 0;
	struct TransformFields fields;
	entity->transform->hierarchyTransform(&fields);
	if (_AABBpositioncache != fields.position) {
		_AABBpositioncache = fields.position;
		recalculateAABB = 1;
	}
	if (fields.rotation != _AABBrotationcache) {
		printf("Not the same rotation");
		_AABBrotationcache = fields.rotation;
		// spheres aren't affected by rotations
		if(colliderShapeType != SHAPE_SPHERE) {
				recalculateAABB = 1;
		}
	}
	if (fields.scale != _AABBscalecache) {
		_AABBscalecache = fields.scale;
		recalculateAABB = 1;
	}
	if (colliderShapeType != _AABBshapecache) {
		_AABBshapecache = colliderShapeType;
		recalculateAABB = 1;
	}
	if(recalculateAABB == 0) { return; }

	colliderShape->generateAABB(colliderAABB, &fields);
}

void Collider::getTransformFields(TransformFields* output) {
	entity->transform->hierarchyTransform(output);
}

void Collider::awake() {
	colliderShapeType = SHAPE_NONE;
	colliderShape = new EmptyShape();
	colliderAABB = new AABB(new Vector3(0,0,0), new Vector3(0,0,0));
	drawGizmo = 1;
}