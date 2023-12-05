#ifndef COMP_ENTITYROTATOR
#define COMP_ENTITYROTATOR

#include "ecs.h"

class EntityRotator : public Component {
	private:
		float totalTime;
	public:
		virtual void update(float delta);

		EntityRotator(int id, Entity* parent):Component(id, parent) {};
};

#endif