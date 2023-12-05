#include "comp_playercontroller.h"
#include "ecs.h"
#include "input.h"
#include "Quaternion.h"
#include "comp_collider.h"
#include "physics.h"
#include <stdlib.h>

void PlayerController::awake() {
	cc = NULL;
	collider = NULL;
	moveonce = 0;
}

void PlayerController::start() {
	cc = entity->getComponent<CameraController>();
	collider = entity->getComponent<Collider>();
	if(cc == NULL) {printf("PlayerController requires component CameraController to function!\n");}
	if(collider == NULL) {printf("PlayerController requires component Collider to function!\n");}
}

void PlayerController::update(float delta) {
	if(cc != NULL){
		cc->apply();
	}

	struct Vector2* joystick = new Vector2();
	Input::getJoystickNormalized(KEYCODE_W, KEYCODE_D, KEYCODE_S, KEYCODE_A, 1, joystick);
	Vector3 wasd = new Vector3(-joystick->x * delta, 0, joystick->y * delta);
	struct Quaternion rotation;
	struct Vector3 up = Vector3Up;
	struct Vector3 lookVector; cc->getLookVector(&lookVector);
	Quaternion::lookRotation(&lookVector, &up, &rotation);
	struct Vector3 finalMove;
	Quaternion::rotate(&rotation, &wasd, &finalMove);
	finalMove *= 1 * delta;
	velocity = finalMove + velocity;
	struct Vector3 drag = velocity * (3 * delta);
	velocity -= (drag);
	entity->transform->position += velocity;

	/*

	FIX ME LATER

	Collider* collisions = NULL;
	
	struct Collider* hash = (Collider*)ECS_getAllInstancesOfComponent(CTYPE_COLLIDER);
	collisions = Physics_findOverlappingAABBs(x->collider, hash);	// adding to new hash here ruins colliders- why?
	struct Collider* possibleCollision;
	for(possibleCollision = collisions; possibleCollision != NULL; possibleCollision = possibleCollision->collisionhh.next) {
		struct CollisionInfo collisionInfo;
		if(Physics_getCollisionInfo(x->collider, possibleCollision, &collisionInfo) == 1) {
			Vector3_copy(&collisionInfo.safePosition, &x->entity->transform->position);
			struct Vector3 velocityOnNormal;
			Vector3_project(&collisionInfo.normal, &x->velocity, &velocityOnNormal);
			//printf("Velocity is= "); Vector3_print(&x->velocity);
			//printf("Velocity on normal is= "); Vector3_print(&velocityOnNormal);
			Vector3_sub(&x->velocity, &velocityOnNormal, &x->velocity);
		}
	}
	*/
}