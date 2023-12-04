#ifndef VR_GLAPP_H
#define VR_GLAPP_H
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <pointcloud.h>
#include <shader_program.h>
#include <memory>
#include <ptx_reader.h>
#include <iostream>
#include <openvr/headers/openvr.h>
#include <render_model.h>

namespace MasterProject {

class App {
public:
	
	//constructor and destructor
	App();
	virtual ~App();
	
	//accesor for window should close
	bool windowShouldClose();
	bool initPointcloud(const std::string& file, int position_scanner, float rotation_scanner, float height_user, float height_room);

	// Default implementation of event handlers
	void keyPressHandler(int key, int action, int mods);
	void mouseClickHandler(int button, int action);
	void mouseMoveHandler(glm::dvec2 position);

	glm::vec4 direction;
	glm::vec3 maxvector;
	glm::vec3 minvector;
	//functions
	bool initGL();
	int initVR();

	bool render(glm::mat4 vpmatrix);
	bool end();
	bool RenderToTexture();
	bool SetupFramebuffers();
	void update_pose_matrix();
	glm::mat4 GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye);
	glm::mat4 GetHMDMatrixPoseEye(vr::Hmd_Eye nEye);
	glm::mat4 steamvrtoglm(const vr::HmdMatrix34_t& matPose);
	glm::mat4 getvpmatrix_left();
	glm::mat4 getvpmatrix_right();
	void update();
	bool SetupRenderVR();
	void RenderVRPointcloud(int nreye);


	//buffer ID's for render to texture
	GLuint framebufferID_left = 0;
	GLuint renderedTexture_left;
	GLuint depthbufferID_left;
	GLuint framebufferID_right = 0;
	GLuint renderedTexture_right;
	GLuint depthbufferID_right;

	//VR System
	vr::IVRSystem* vr_environment;
	std::string _strDriver;
	std::string _strDisplay;

	//variables
	float m_fNearClip = 0.1f;
	float m_fFarClip = 30.0f;
	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	int ValidPoseCount;
	int ValidPoseCount_Last;
	std::string strPoseClasses;
	char DeviceClassChar[vr::k_unMaxTrackedDeviceCount];


	glm::mat4 _projectionmatrix_eye_left;
	glm::mat4 _posematrix_eye_left;
	glm::mat4 _projectionmatrix_eye_right;
	glm::mat4 _posematrix_eye_right;

	glm::mat4 _vpmatrix_eye_left;
	glm::mat4 _vpmatrix_eye_right;
	glm::mat4 _poseHMDmatrix;
	glm::mat4 _mvpMatrix_left;
	glm::mat4 _mvpMatrix_right;
	glm::mat4 _device_pose_matrix[k_unMaxTrackedDeviceCount];

	uint32_t RenderWidth;
	uint32_t RenderHeight;
	uint32_t VRWindowRenderWidth = 640;
	uint32_t VRWindowRenderHeight = 320;

	float x_boundary_min = 0; 
	float x_boundary_max = 0; 
	float y_boundary_min = 0;
	float y_boundary_max = 0;
	float z_boundary_min = 0;
	float z_boundary_max = 0;
	bool getStopMainLoopVal();
	void handleActions();
	vr::VRActionSetHandle_t actionsetMasterProject = vr::k_ulInvalidActionSetHandle;

	bool StopMainLoop = true;
	vr::VRActionHandle_t actionStopMainLoop = vr::k_ulInvalidActionHandle;
	vr::VRActionHandle_t actionJump = vr::k_ulInvalidActionHandle;
	bool jumping = false;
	
private:
	glm::uvec2 getFramebufferSize() const;
	bool setup_render_model(vr::TrackedDeviceIndex_t);

	//definitions 
	GLFWwindow* _window = nullptr;
	glm::uvec2 _window_size = glm::uvec2(1200, 1000);
	std::string _title = "VR";

	std::shared_ptr<Pointcloud> _pointcloud;
	std::shared_ptr<ShaderProgram> _programForPointcloud;
	std::shared_ptr<ShaderProgram> _programForPointcloudVR;
	std::shared_ptr<ShaderProgram> _programForRendermodel;

	glm::mat4 _mvpMatrix = glm::mat4(1);
	int _base_stations_count = 0;
	RenderModel* _render_models[vr::k_unMaxTrackedDeviceCount];
};

};
#endif // VR_GLAPP_H