#include "CSCIx229.h"
#include "vector3.h"
#include "vector2.h"
#include "Quaternion.h"
#include "ecs.h"
#include "input.h"
#include "window.h"
#include <math.h>
#include <unordered_map>
#include "physics.h"
#include <stdlib.h>
#include <iostream>
#include "gizmodrawer.h"

#include "comp_camera.h"
#include "comp_collider.h"
#include "comp_mesh.h"
#include "comp_cameracontroller.h"
#include "comp_playercontroller.h"
#include "comp_entityrotator.h"

using namespace std;

double DIM = 2;
int windowLength = 800;
int windowHeight = 800;

float current_dim = 3;

float deltaTime = 0;
float cumulativeTime = 0;
int mouseDeltaX = 0;
int mouseDeltaY = 0;
float current_fov = 90;
int shadingMode = GL_FLAT;

int axes = 1;
int drawColliderGizmos = 1;
int drawAABBGizmos = 1;

void translateFromVector(Vector3* vec) {
	glTranslated(vec->x, vec->y, vec->z);
}

void rotateFromQuat(Quaternion* quat) {
	float* qm = quat->toMat4();
	GLdouble rotMatrix[16] = {qm[0], qm[1], qm[2], qm[3], qm[4], qm[5], qm[6], qm[7], qm[8], qm[9], qm[10], qm[11], qm[12], qm[13], qm[14], qm[15]};
	glMultMatrixd(rotMatrix);
	free(qm);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(shadingMode);

	for(auto i : ECS::hashes[typeid(Camera*).hash_code()]) {
		Camera* cam = static_cast<Camera*>(i.second);
		current_fov = cam->fov;
		Entity* e = cam->entity;

		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		Project(current_fov, Window::asp, current_dim, 0.01, 25);
		Vector3 position = e->transform->position;
		Vector3 positionLook;
		Vector3 forward = Vector3Forward;
		Quaternion::rotate(&cam->entity->transform->rotation, &forward, &positionLook);
		positionLook += &e->transform->position;
		Vector3 up = Vector3Up;
		gluLookAt(position.x, position.y, position.z, positionLook.x, positionLook.y, positionLook.z, up.x, up.y, up.z);
	}

	if(drawColliderGizmos || drawAABBGizmos) {
		for(auto i : ECS::hashes[typeid(Collider*).hash_code()]) {
			Collider* col = static_cast<Collider*>(i.second);
			glPushMatrix();

			if(drawAABBGizmos) {
				glColor3f(1, 0, 0);
				GizmoDrawer::drawAABB(col->colliderAABB);
			}
			translateFromVector(&col->entity->transform->position);
			rotateFromQuat(&col->entity->transform->rotation);
			if(drawColliderGizmos) {
				glColor3f(0, 1, 0);
				GizmoDrawer::drawCollider(col);
			}

			glPopMatrix();
		}
	}

	for(auto i : ECS::hashes[typeid(Mesh*).hash_code()]) {
		Mesh* mesh = static_cast<Mesh*>(i.second);
		glPushMatrix();

		translateFromVector(&mesh->entity->transform->position);
		rotateFromQuat(&mesh->entity->transform->rotation);
		mesh->draw();

		glPopMatrix();
	}

	glColor3f(1, 1, 1);
	if (axes)
	{
		const double len = 1.5;
		glBegin(GL_LINES);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(len, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, len, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, len);
		glEnd();
		//  Label axes
		glRasterPos3d(len, 0.0, 0.0);
		Print("X");
		glRasterPos3d(0.0, len, 0.0);
		Print("Y");
		glRasterPos3d(0.0, 0.0, len);
		Print("Z");
	}

	ErrCheck("display");							//<-- this thing says stack overflow?
	glFlush();
	glutSwapBuffers();
}

void idle() {
	// Do time thingies
	static double t0 = -1.;
	double dt, t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	if (t0 < 0.0)
	  t0 = t;
	dt = t - t0;
	t0 = t;
	deltaTime = dt;
	cumulativeTime += deltaTime;

	// Run game loop step
	ECS::runStarts();				// First run start functions on any newly instantiated components
	Physics::updateAABBs();
	//Vector3_print(&(((Collider*)ECS_getComponent(testEntity, CTYPE_COLLIDER))->AABB->extents));
	ECS::runUpdates(deltaTime);		// Next, run updates on all subscribed components
	Physics::updateAABBs();
	Input::clearMouseDelta();		// Clear mouse delta buffer after updates are run
	glutPostRedisplay();			// Next, draw scene to the screen
	ECS::runLateUpdates();			// Next, run late updates on all subscribed components
	Input::setBufferToCurrent();			// Clear buffered inputs for next frame
}

void initScene() {
	Entity* player = ECS::instantiate();
		player->transform->position = new Vector3(-2, 0.5, 0);
	Camera* player_camera = player->addComponent<Camera>();
		player_camera->fov = 100;
		player_camera->dim = 1;
	Collider* player_collider = player->addComponent<Collider>();
		player_collider->colliderShape = new Sphere(.15);
		player_collider->colliderShapeType = SHAPE_SPHERE;
		player_collider->drawGizmo = 0;
	player->addComponent<CameraController>();
	player->addComponent<PlayerController>();

	Entity* object1 = ECS::instantiate();
		object1->transform->position = new Vector3(1, 0, 0);
	Collider* object1_collider = object1->addComponent<Collider>();
		//object1_collider->colliderShape = new Capsule(.5, .2, new Vector3{0,1,0});
		//object1_collider->colliderShapeType = SHAPE_CAPSULE;
		object1_collider->colliderShape = new Sphere(.25);
		object1_collider->colliderShapeType = SHAPE_SPHERE;
	Mesh* object1_mesh = object1->addComponent<Mesh>();
		object1_mesh->mesh_type = MESHTYPE_TEAPOT;
		object1_mesh->color = Vector3One;
	object1->addComponent<EntityRotator>();


	Input::setCursorLocked(1);
	Physics::updateAABBs();
}


int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(windowLength, windowHeight);
	glutCreateWindow("PainEngine");
#ifdef USEGLEW
	//  Initialize GLEW
	if (glewInit() != GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
	glutDisplayFunc(display);
	glutReshapeFunc(Window::reshape);
	glutKeyboardFunc(Input::setKeyDown);
	glutKeyboardUpFunc(Input::setKeyUp);
	glutPassiveMotionFunc(Input::passive);
	glutIdleFunc(idle);
	ErrCheck("init");

	initScene();

	glutMainLoop();
	return 0;
}