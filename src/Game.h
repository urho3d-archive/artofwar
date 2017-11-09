#pragma once
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Scene/Scene.h>


namespace Urho3D {
	class Console;
	class Graphics;
	class Context;
	class ResourceCache;
	class UI;
}

class ActionCommandList;
class CommandList;
class Mediator; 
class BuildList;
class CreationCommandList;
class Enviroment;
class DatabaseCache;
class CameraManager;
class PlayersManager;

class Game
{
public:
	static Game* get();
	static void init();
	~Game();
	Game* setCache(Urho3D::ResourceCache* _cache);
	Game* setContext(Urho3D::Context* _context);
	Game* setUI(Urho3D::UI* _ui);
	Game* setGraphics(Urho3D::Graphics* _graphics);
	Game* setScene(const Urho3D::SharedPtr<Urho3D::Scene>& scene);
	Game* setEngine(const Urho3D::SharedPtr<Urho3D::Engine>& _engine);
	Game* setCameraManager(CameraManager* cameraManager);
	Game* setActionCommmandList(ActionCommandList* _actionCommmandList);
	Game* setConsole(Urho3D::Console* _console);
	Game* setMediator(Mediator* _mediator);
	Game* setBuildList(BuildList* _buildList);
	Game* setCreationCommandList(CreationCommandList* _simCommandList);
	Game* setDatabaseCache(DatabaseCache* _databaseCache);
	Game* setPlayersManager(PlayersManager* _playersManager);
	Game* setEnviroment(Enviroment* _enviroment);


	CameraManager* getCameraManager() const;
	Urho3D::ResourceCache* getCache() const;
	Urho3D::SharedPtr<Urho3D::Scene> getScene() const;
	Urho3D::Context* getContext() const;
	Urho3D::UI* getUI() const;
	Urho3D::Graphics* getGraphics() const;
	Urho3D::SharedPtr<Urho3D::Engine> getEngine() const;
	ActionCommandList* getActionCommandList() const;
	Urho3D::Console* getConsole() const;
	Mediator* getMediator() const;
	BuildList* getBuildList() const;
	CreationCommandList* getCreationCommandList() const;
	DatabaseCache* getDatabaseCache() const;
	PlayersManager* getPlayersManager() const;
	Enviroment* getEnviroment() const;
private:
	Game();
	static Game* instance;
	Urho3D::ResourceCache* cache;
	Urho3D::SharedPtr<Urho3D::Scene> scene;
	Urho3D::Context* context;
	Urho3D::UI* ui;
	Urho3D::Graphics* graphics;
	Urho3D::SharedPtr<Urho3D::Engine> engine;

	CameraManager* cameraManager;
	Urho3D::Console* console;
	Mediator* mediator;
	BuildList* buildList;
	ActionCommandList* actionCommandList;
	CreationCommandList * creationCommandList;
	DatabaseCache * databaseCache;
	PlayersManager * playersManager;
	Enviroment * enviroment;

};
