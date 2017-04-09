#pragma once
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Material.h>
#include "Entity.h"

class ObjectManager;
using namespace Urho3D;

class LevelBuilder
{
public:
	LevelBuilder();
	~LevelBuilder();
	SharedPtr<Scene> CreateScene(ObjectManager* objectManager);

private:
	SharedPtr<Scene> scene;
	Entity* createZone();
	Entity* createLight();
	Entity* createGround();
};
