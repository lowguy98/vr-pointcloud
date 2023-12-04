#ifndef VR_MANAGER_H
#define VR_MANAGER_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include <app.h>


namespace MasterProject {
class SceneManager {
public:
	static SceneManager& getInstance();
	int init(const std::string& file, int position_scanner = 1, float rotation_scanner = 0, float  height_user = 170, float height_room = 250);
	int run();
	glm::mat4 _vpmatrix_eye;

	int position_scanner; 
	float rotation_scanner; 

private:
	SceneManager();
	~SceneManager();
	int initOpenGL();
	int initOpenVR();

	std::shared_ptr<App> _app;

	
};

};
#endif // VR_MANAGER_H