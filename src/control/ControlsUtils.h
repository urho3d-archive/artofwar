#pragma once
#include "Game.h"
#include "HitData.h"
#include "camera/CameraManager.h"
#include "objects/Physical.h"
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/OctreeQuery.h>
#include <Urho3D/Math/Ray.h>
#include <Urho3D/UI/UI.h>


inline void resultQuery(const Ray& cameraRay, PODVector<RayQueryResult>& results) {
	RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, 300, DRAWABLE_GEOMETRY);
	Game::getScene()->Node::GetComponent<Octree>()->Raycast(query);
}

bool raycast(hit_data& hitData) {
	const IntVector2 pos = Game::getUI()->GetCursorPosition();
	if (!Game::getUI()->GetCursor()->IsVisible() || Game::getUI()->GetElementAt(pos, true)) {
		return false;
	}

	const Ray cameraRay = Game::getCameraManager()
	                                 ->getComponent()
	                                 ->GetScreenRay((float)pos.x_ / Game::getGraphics()->GetWidth(),
	                                                (float)pos.y_ / Game::getGraphics()->GetHeight());

	PODVector<RayQueryResult> results;
	resultQuery(cameraRay, results);
	int i = 0;
	while (i < results.Size()) {
		RayQueryResult& result = results[i];

		Node* node = result.node_;

		auto lc = node->GetComponent<LinkComponent>();
		if (lc) {
			hitData.set(result, lc);
			return true;
		}
		lc = node->GetParent()->GetComponent<LinkComponent>();
		if (lc) {
			hitData.set(result, lc);
			return true;
		}
		++i;
	}
	hitData.drawable = nullptr;
	hitData.link = nullptr;

	return false;
}

bool raycast(hit_data& hitData, ObjectType type) {
	const IntVector2 pos = Game::getUI()->GetCursorPosition();
	if (!Game::getUI()->GetCursor()->IsVisible() || Game::getUI()->GetElementAt(pos, true)) {
		return false;
	}

	const Ray cameraRay = Game::getCameraManager()
	                                 ->getComponent()
	                                 ->GetScreenRay((float)pos.x_ / Game::getGraphics()->GetWidth(),
	                                                (float)pos.y_ / Game::getGraphics()->GetHeight());

	PODVector<RayQueryResult> results;
	resultQuery(cameraRay, results);
	int i = 0;
	while (i < results.Size()) {
		RayQueryResult& result = results[i];

		Node* node = result.node_;

		auto lc = node->GetComponent<LinkComponent>();
		if (lc && lc->getPhysical()->getType() == type) {
			hitData.set(result, lc);
			return true;
		}
		lc = node->GetParent()->GetComponent<LinkComponent>();
		if (lc && lc->getPhysical()->getType() == type) {
			hitData.set(result, lc);
			return true;
		}
		++i;
	}
	hitData.drawable = nullptr;
	hitData.link = nullptr;

	return false;
}
