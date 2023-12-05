#ifndef COMP_COLLIDER
#define COMP_COLLIDER

#include "ecs.h"
#include "vector3.h"
#include "Quaternion.h"
#include "geometry.h"

class Collider : public Component {
	public:
		int drawGizmo;
		
		PrimitiveShape colliderShapeType;
		ShapeBase* colliderShape;
		AABB* colliderAABB;

		Vector3 _AABBpositioncache;
		Quaternion _AABBrotationcache;
		Vector3 _AABBscalecache;
		PrimitiveShape _AABBshapecache;

		virtual void awake();
		void updateAABB();

		Collider(int id, Entity* parent):Component(id, parent) { awake(); };
		void getTransformFields(TransformFields* output);
		void setShapeSphere(float radius);
};

#endif