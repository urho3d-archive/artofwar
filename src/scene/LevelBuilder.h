#pragma once
#include "SceneObjectManager.h"
#include "load/SceneLoader.h"
#include "objects/Entity.h"
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Terrain.h>
#include <Urho3D/Scene/Scene.h>



using namespace Urho3D;

class LevelBuilder
{
public:
	LevelBuilder();
	~LevelBuilder();
	void createScene(SceneLoader& loader);
	Terrain* getTerrian();
private:
	SharedPtr<Urho3D::Scene> scene;

	Entity* createZone();
	Entity* createLight(Vector3 direction, Color color, LightType lightType);
	Entity* createGround(String heightMap, String texture, float horScale, float verScale);

	SceneObjectManager *objectManager;
	Terrain* terrain;
};
