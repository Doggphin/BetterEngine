#include "comp_entityrotator.h"
#include "ecs.h"
#include "comp_mesh.h"

void EntityRotator::update(float delta) {
	totalTime += delta;
	Mesh* m = entity->getComponent<Mesh>();
	if (m != NULL) {
		m->color = Vector3(sin(totalTime) / 2 + .5, cos(totalTime / 1.414) / 2 + .5, cos(totalTime)  / 2 + .5);
		m->color.normalize();
	}
    Vector3 rotator = Vector3(cos(totalTime), 0, sin(totalTime));
	Vector3 forward = Vector3Forward;
    Quaternion::fromToRotation(&forward, &rotator, &(entity->transform->rotation));

	//Vector3_set(cos(x->totalTime / 3), sin(x->totalTime * 1.2) / 5, sin(x->totalTime / 3), &x->entity->transform->position);
}

void EntityRotator_awake(EntityRotator* x) {
	// Blank
}

void EntityRotator_start(EntityRotator* x) {

}

void EntityRotator_lateupdate(EntityRotator* x) {

}