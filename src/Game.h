#pragma once
#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Resource/Localization.h>
#include <Urho3D/Scene/Scene.h>


namespace Urho3D {
	class Console;
	class Graphics;
	class Context;
	class ResourceCache;
	class UI;
}

class CommandList;
class CreationCommandList;
class Enviroment;
class DatabaseCache;
class CameraManager;
class PlayersManager;
class QueueManager;
class ColorPeletteRepo;
class FormationManager;

class Game
{
public:
	static Game* get() { return instance; }
	static void init();
	static void dispose();
	~Game();
	Game* setCache(Urho3D::ResourceCache* _cache);
	Game* setContext(Urho3D::Context* _context);
	Game* setUI(Urho3D::UI* _ui);
	Game* setGraphics(Urho3D::Graphics* _graphics);
	Game* setScene(const Urho3D::SharedPtr<Urho3D::Scene>& scene);
	Game* setEngine(const Urho3D::SharedPtr<Urho3D::Engine>& _engine);
	Game* setConsole(Urho3D::Console* _console);
	Game* setLocalization(Urho3D::Localization* _localization);
	Game* setCameraManager(CameraManager* cameraManager);
	Game* setActionCommmandList(CommandList* _actionCommmandList);
	Game* setCreationCommandList(CreationCommandList* _simCommandList);
	Game* setDatabaseCache(DatabaseCache* _databaseCache);
	Game* setPlayersManager(PlayersManager* _playersManager);
	Game* setEnviroment(Enviroment* _enviroment);
	Game* setQueueManager(QueueManager* _queueManager);
	Game* setFormationManager(FormationManager* _formationManager);
	Game* setColorPeletteRepo(ColorPeletteRepo* _colorPeletteRepo);


	Urho3D::ResourceCache* getCache() const;
	Urho3D::SharedPtr<Urho3D::Scene> getScene() const;
	Urho3D::Context* getContext() const;
	Urho3D::UI* getUI() const;
	Urho3D::Graphics* getGraphics() const;
	Urho3D::SharedPtr<Urho3D::Engine> getEngine() const;
	Urho3D::Console* getConsole() const;
	Urho3D::Localization* getLocalization() const;

	QueueManager* getQueueManager() const;
	CameraManager* getCameraManager() const;
	CommandList* getActionCommandList() const;
	CreationCommandList* getCreationCommandList() const;
	DatabaseCache* getDatabaseCache() const;
	PlayersManager* getPlayersManager() const;
	Enviroment* getEnviroment() const;
	FormationManager* getFormationManager() const;
	ColorPeletteRepo* getColorPeletteRepo() const;

private:
	Game();
	static Game* instance;

	Urho3D::ResourceCache* cache;
	Urho3D::SharedPtr<Urho3D::Scene> scene;
	Urho3D::Context* context;
	Urho3D::UI* ui;
	Urho3D::Graphics* graphics;
	Urho3D::SharedPtr<Urho3D::Engine> engine;
	Urho3D::Console* console;
	Urho3D::Localization* localization;

	CameraManager* cameraManager;
	CommandList* actionCommandList;
	CreationCommandList* creationCommandList;
	DatabaseCache* databaseCache;
	PlayersManager* playersManager;
	Enviroment* enviroment;
	QueueManager* queue;
	FormationManager* formationManager;
	ColorPeletteRepo* colorPeletteRepo;
};
