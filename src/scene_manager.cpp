#include <scene_manager.h>
#include <iostream>

namespace MasterProject {

	SceneManager::SceneManager() {}

	SceneManager::~SceneManager() {}

	SceneManager& SceneManager::getInstance() {
		static SceneManager instance;
		return instance;
	}

	/// <summary>
	///Author: Sarah, Jie
	///general function for initialization 
	/// </summary>
	int SceneManager::init(const std::string& file, int position_scanner, float rotation_scanner, float  height_user, float height_room) {
		
		_app = std::make_shared<App>();
		
		if ((initOpenGL() == -1) || (initOpenVR() == -1))
			return -1;

		_app->initPointcloud(file, position_scanner,rotation_scanner, height_user, height_room);
		return 0;
	};

	/// <summary>
	///Author: Sarah
	///function to init OpenGL
	/// </summary>
	int SceneManager::initOpenGL() {
		if (_app->initGL()) {
			return 1;
		}
		return -1;
	}

	/// <summary>
	///Author: Sarah
	///function to init OpenVR 
	/// </summary>
	int SceneManager::initOpenVR() {
		 _app->initVR();
		return 1;
	}

	/// <summary>
	///Author: Sarah, Jie 
	///function to run the main update loop
	/// </summary>
	int SceneManager::run() {
		// Render loop: process input handlers and call custom rendering
		while (_app->getStopMainLoopVal()) {
			_app->update();
			_vpmatrix_eye = _app->getvpmatrix_left();
			if (!_app->render(_vpmatrix_eye))
				return -1;
			glfwPollEvents();
			_app->RenderToTexture();		
		}

		// Call custom application cleaning and termination
		return _app->end();
	}

};
