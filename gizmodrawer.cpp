#include "gizmodrawer.h"
#include <math.h>
#include "vector2.h"

Vector2 GizmoDrawer::circlePoints[16] = {
    {0, 1},
    {.3827, .9239},
    {.7071, .7071},
    {.9239, .3827},
    {1, 0},
    {.9239, -.3827},
    {.7071, -.7071},
    {.3827, -.9239},
    {0, -1},
    {-.3827, -.9239},
    {-.7071, -.7071},
    {-.9239, -.3827},
    {-1, 0},
    {-.9239, .3827},
    {-.7071, .7071},
    {-.3827, .9239},
};

void GizmoDrawer::drawSphere(float radius) {
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<16; i++) {
        glVertex3d(circlePoints[i].x * radius, circlePoints[i].y * radius, 0);
    }
    glVertex3d(0, radius, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<16; i++) {
        glVertex3d(0, circlePoints[i].x * radius, circlePoints[i].y * radius);
    }
    glVertex3d(0, 0, radius);
    glEnd();

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<16; i++) {
        glVertex3d(circlePoints[i].x * radius, 0, circlePoints[i].y * radius);
    }
    glVertex3d(0, 0, radius);
    glEnd();
}

void GizmoDrawer::drawCapsule(float radius, float height) {
    glBegin(GL_LINE_STRIP);
    for(int i=0; i<5; i++) {
        glVertex3d(circlePoints[i].x * radius, circlePoints[i].y * radius + height, 0);
    }
    for(int i=4; i<13; i++) {
        glVertex3d(circlePoints[i].x * radius, circlePoints[i].y * radius - height, 0);
    }
    for(int i=12; i<16; i++) {
        glVertex3d(circlePoints[i].x * radius, circlePoints[i].y * radius + height, 0);
    }
    glVertex3d(0, radius + height, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<5; i++) {
        glVertex3d(0, circlePoints[i].y * radius + height, circlePoints[i].x * radius);
    }
    for(int i=4; i<13; i++) {
        glVertex3d(0, circlePoints[i].y * radius - height, circlePoints[i].x * radius);
    }
    for(int i=12; i<16; i++) {
        glVertex3d(0, circlePoints[i].y * radius + height, circlePoints[i].x * radius);
    }
    glVertex3d(0, radius + height, 0);
    glEnd();

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<16; i++) {
        glVertex3d(circlePoints[i].x * radius, height, circlePoints[i].y * radius);
    }
    glVertex3d(0, height, radius);
    glEnd();

    glBegin(GL_LINE_STRIP);
    for(int i=0; i<16; i++) {
        glVertex3d(circlePoints[i].x * radius, -height, circlePoints[i].y * radius);
    }
    glVertex3d(0, -height, radius);
    glEnd();
}


void GizmoDrawer::drawAABB(AABB* aabb) {
    glPushMatrix();
    glBegin(GL_LINE_STRIP);
	glVertex3d(aabb->min.x, aabb->min.y, aabb->min.z);
	glVertex3d(aabb->max.x, aabb->min.y, aabb->min.z);
	glVertex3d(aabb->max.x, aabb->max.y, aabb->min.z);
	glVertex3d(aabb->min.x, aabb->max.y, aabb->min.z);
	glVertex3d(aabb->min.x, aabb->min.y, aabb->min.z);
	glVertex3d(aabb->min.x, aabb->min.y, aabb->max.z);
	glVertex3d(aabb->min.x, aabb->max.y, aabb->max.z);
	glVertex3d(aabb->min.x, aabb->max.y, aabb->min.z);
	glEnd();
	glBegin(GL_LINES);
	glVertex3d(aabb->max.x, aabb->max.y, aabb->max.z);
	glVertex3d(aabb->min.x, aabb->max.y, aabb->max.z);
	glVertex3d(aabb->max.x, aabb->max.y, aabb->max.z);
	glVertex3d(aabb->max.x, aabb->min.y, aabb->max.z);
	glVertex3d(aabb->max.x, aabb->max.y, aabb->max.z);
	glVertex3d(aabb->max.x, aabb->max.y, aabb->min.z);
	glVertex3d(aabb->max.x, aabb->min.y, aabb->max.z);
	glVertex3d(aabb->max.x, aabb->min.y, aabb->min.z);
	glVertex3d(aabb->max.x, aabb->min.y, aabb->max.z);
	glVertex3d(aabb->min.x, aabb->min.y, aabb->max.z);
	glEnd();
    glPopMatrix();
}

void GizmoDrawer::drawCollider(Collider* collider) {
    if(collider->drawGizmo == 0) { return; }
    switch(collider->colliderShapeType) {
        case SHAPE_SPHERE: {
            drawSphere(static_cast<Sphere*>(collider->colliderShape)->radius);
        } break;
        case SHAPE_CAPSULE: {
            Capsule* capsule = static_cast<Capsule*>(collider->colliderShape);
            drawCapsule(capsule->radius, capsule->height);
        } break;
        default: {

        } break;
    }
}