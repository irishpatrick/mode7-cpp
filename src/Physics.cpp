#include "Physics.hpp"
#if defined __linux__
#include <btBulletDynamicsCommon.h>
#elif defined _WIN32
#include <bullet/btBulletDynamicsCommon.h>
#endif

static btBroadphaseInterface* broadphase = nullptr;
static btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
static btCollisionDispatcher* dispatcher = nullptr;
static btSequentialImpulseConstraintSolver* solver = nullptr;
static btDiscreteDynamicsWorld* world = nullptr;

void Physics::init()
{
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    world->setGravity(btVector3(0, -9.81f, 0));
}

void Physics::destroy()
{
    delete world;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}
