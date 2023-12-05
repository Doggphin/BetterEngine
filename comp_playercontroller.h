#ifndef COMP_PLAYERCONTROLLER
#define COMP_PLAYERCONTROLLER

#include "comp_cameracontroller.h"
#include "ecs.h"
#include "vector2.h"
#include "comp_collider.h"

class PlayerController : public Component {
	private:
		CameraController* cc;
		Collider* collider;
		Vector3 velocity;
		int moveonce;
	public:
		virtual void awake();
		virtual void start();
		virtual void update(float delta);

		PlayerController(int id, Entity* parent):Component(id, parent) {};
};

#endif