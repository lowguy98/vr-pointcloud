#include <ptx_reader.h>
#include <iostream>
#include <stdio.h>
#include <Pointcloud.h>
#include <memory>
#include <scene_manager.h>
#include <glm/glm.hpp>

using namespace MasterProject;

int main() {
	SceneManager& manager = SceneManager::getInstance();

	// input user: height in cm
	float height_room = 300; 
	float height_user = 160; 
	
	//const std::string& file = "C:/Users/feuzs/OneDrive/Desktop/2C05_1.ptx";
	//const std::string& file = "C:/Users/Sarah/Desktop/2C05_1.ptx";
	const std::string& file = "D:/study/UZH/MasterProject/data/2C05_1.ptx";
	//rotation with respect to north position
	float rotation_scanner = -17.0;

	manager.init(file,1, rotation_scanner, height_user, height_room);
	manager.run();


}
