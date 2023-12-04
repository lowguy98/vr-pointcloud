#include <app.h>
#include <iostream>
using namespace std;

namespace MasterProject {

	//constructor and destructor 
	App::App() {
		
	}
	App::~App() {
		glfwDestroyWindow(_window);
		glfwTerminate();
	}


	/// <summary>
	/// Author: OpenVR Documentation,  https://github.com/ValveSoftware/openvr
	/// This is a helper function to get a string form a tracked device property.
	/// </summary>
	string GetTrackedDeviceString(vr::IVRSystem* pHmd, vr::TrackedDeviceIndex_t unDevice, vr::TrackedDeviceProperty prop, vr::TrackedPropertyError* peError = NULL) {
		uint32_t requiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, NULL, 0, peError);
		if (requiredBufferLen == 0)
			return "";

		char* pchBuffer = new char[requiredBufferLen];
		requiredBufferLen = pHmd->GetStringTrackedDeviceProperty(unDevice, prop, pchBuffer, requiredBufferLen, peError);
		string sResult = pchBuffer;
		delete[] pchBuffer;
		return sResult;
	}

	/// <summary>
	/// Author: OpenVR Documentation,  https://github.com/ValveSoftware/openvr
	/// This is a helper function to get a string form a tracked device type class and turn it into a std::string type. 
	/// </summary>
	string GetTrackedDeviceClassString(vr::ETrackedDeviceClass td_class) {

		string str_td_class = "Unknown class";

		switch (td_class)
		{
		case vr::TrackedDeviceClass_Invalid:			// = 0, the ID was not valid.
			str_td_class = "Invalid";
			break;
		case vr::TrackedDeviceClass_HMD:				// = 1, Head-Mounted Displays
			str_td_class = "HMD";
			break;
		case vr::TrackedDeviceClass_Controller:			// = 2, Tracked controllers
			str_td_class = "Controller";
			break;
		case vr::TrackedDeviceClass_GenericTracker:		// = 3, Generic trackers, similar to controllers
			str_td_class = "Generic Tracker";
			break;
		case vr::TrackedDeviceClass_TrackingReference:	// = 4, Camera and base stations that serve as tracking reference points
			str_td_class = "Tracking Reference";
			break;
		case vr::TrackedDeviceClass_DisplayRedirect:	// = 5, Accessories that aren't necessarily tracked themselves, but may redirect video output from other tracked devices
			str_td_class = "Display Redirecd";
			break;
		}

		return str_td_class;
	}

	/// <summary>
	/// Author: OpenVR Documentation,  https://github.com/ValveSoftware/openvr
	/// This is a helper function to get the digital action state back. 
	/// </summary>
	bool GetDigitalActionState(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* pDevicePath = nullptr)
	{
		vr::InputDigitalActionData_t actionData;
		vr::VRInput()->GetDigitalActionData(action, &actionData, sizeof(actionData), vr::k_ulInvalidInputValueHandle);
		if (pDevicePath)
		{
			*pDevicePath = vr::k_ulInvalidInputValueHandle;
			if (actionData.bActive)
			{
				vr::InputOriginInfo_t originInfo;
				if (vr::VRInputError_None == vr::VRInput()->GetOriginTrackedDeviceInfo(actionData.activeOrigin, &originInfo, sizeof(originInfo)))
				{
					*pDevicePath = originInfo.devicePath;
				}
			}
		}
		return actionData.bActive && actionData.bState;
	}
	
	/// <summary>
	/// Author: Based on parts of the OpenGL exercises in Computer Graphics course at UZH.
	/// This function initialize the needed libraries to render to a window on screen. It creates a window to render to later on. 
	/// </summary>
	bool App::initGL() {
		printf("Init...\n");
		// GLFW initialization
		if (!glfwInit()) {
			std::cout << "Failed to initialize GLFW\n" << std::endl;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		// Window and OpenGL context creation
		_window = glfwCreateWindow(int(_window_size.x), int(_window_size.y), _title.c_str(), NULL, NULL);
		assert(_window);

		glfwSetWindowPos(_window, 64, 64);
		glfwSetWindowUserPointer(_window, this);
		glfwSetInputMode(_window, GLFW_STICKY_KEYS, 1);	

		// Load OpenGL core profile functions using gl3w
		glfwMakeContextCurrent(_window);

		// Initialize GLEW
		if (glewInit() != GLEW_OK) {
			std::cout << "Failed to initialize GLEW\n" << std::endl;
		}

		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;	
		
		return true;
	}

	/// <summary>
	/// Author: Sarah, Jie 
	/// This function creates a shader program for the point cloud, access the reader to read the PTX file and make transformations, access the boundary values, gets the color & create a vertex array. 
	/// </summary>
	bool App::initPointcloud(const std::string& file, int position_scanner, float rotation_scanner, float height_user, float height_room ) {
		
		//create shader program & instance of PTX Reader
		_programForPointcloud = std::make_shared<ShaderProgram>(std::string(SHADER_DIR) + "/pointcloud");
		PTXReader reader;

		//read and transform the point cloud data
		_pointcloud = reader.read_and_transform_pointcloud(file, position_scanner, rotation_scanner, height_room, height_user);

		//get the boundary values
		x_boundary_max = reader.max_value_x_max;
		x_boundary_min = reader.max_value_x_min;

		y_boundary_min = reader.max_value_y_min;
		y_boundary_max = reader.max_value_y_max;

		z_boundary_min = reader.max_value_z_min;
		z_boundary_max = reader.max_value_z_max;
		
		//get color & create vertex array
		std::vector<glm::vec3>& color = _pointcloud->getColors();
		std::cout << color[0][0] << " " << color[0][1] << " " << color[0][2] << std::endl;
		if (!_pointcloud->createVertexArray(0, 1, 2, 3, 4)) return false;

		return true; 
	}

	/// <summary>
	/// Author: Sarah, Jie (render models) 
	/// This function is the main render function to render to the screen. We bind the framebuffer, draw the controllers and point cloud.
	/// </summary>
	bool App::render(glm::mat4 vpmatrix) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_DEPTH_TEST);

		// Clear the color and depth buffers
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		
		for (int rm_id = 0; rm_id < vr::k_unMaxTrackedDeviceCount; rm_id++) {
			if (_render_models[rm_id] != NULL) {
				 //printf("render model %d on screen\n", rm_id);
				_programForRendermodel->bind();
				glm::mat4 device_mvp = vpmatrix * _device_pose_matrix[rm_id];
				glUniformMatrix4fv(_programForRendermodel->getUniformLocation("mvpMatrix"), 1, GL_FALSE, glm::value_ptr(device_mvp));
				_render_models[rm_id]->draw();
				_programForRendermodel->unbind();
			}
		}
		
		_programForPointcloud->bind();		
		
		_mvpMatrix = vpmatrix * _pointcloud->getModelMatrix();
		
		glUniformMatrix4fv(_programForPointcloud->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &_mvpMatrix[0][0]);
		_pointcloud->draw();
		_programForPointcloud->unbind();
		glfwSwapBuffers(_window);
		return true;
	}

	/// <summary>
	/// Author: Sarah
	/// Frees the used space & shut down VR System, destroys the window, cleans up
	/// </summary>
	bool App::end() {

		glfwDestroyWindow(_window);
		glfwTerminate();
		_programForPointcloud->deleteShaderProgramFromGPU();

		glDeleteBuffers(1, &framebufferID_right);
		glDeleteBuffers(1, &framebufferID_left);
		glDeleteBuffers(1, &depthbufferID_right);
		glDeleteBuffers(1, &depthbufferID_left);

		glDeleteBuffers(1, &depthbufferID_right);
		glDeleteBuffers(1, &depthbufferID_left);
		_programForPointcloudVR->deleteShaderProgramFromGPU();

		vr::VR_Shutdown();
		vr_environment = NULL;
		
		return true;
	}

	/// <summary>
	/// Author: Sarah, Jie (render model)
	/// set the render target size & the shader program
	/// </summary>
	bool App::SetupRenderVR()
	{
		if (!vr_environment)
			return false;

		vr_environment->GetRecommendedRenderTargetSize(&RenderWidth, &RenderHeight);

		//init shader program for Pointcloud to render in the vr headset
		_programForPointcloudVR = std::make_shared<ShaderProgram>(std::string(SHADER_DIR) + "/vr_view");
		
		//init shader program for Rendermodel to render in the vr headset
		_programForRendermodel = std::make_shared<ShaderProgram>(std::string(SHADER_DIR) + "/render_model");
		return true;
	};

	/// <summary>
	/// Author: Sarah
	///render functionality for one eye 
	/// </summary>
	void App::RenderVRPointcloud(int nreye)
	{
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		//bind, get vmp-Matrix, submit the shader, draw and unbind 
		_programForPointcloudVR->bind();
		if (nreye == 1) {
			_mvpMatrix_left = _vpmatrix_eye_left * _pointcloud->getModelMatrix();
			glUniformMatrix4fv(_programForPointcloudVR->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &_mvpMatrix_left[0][0]);
		}
		else {
			_mvpMatrix_right = _vpmatrix_eye_right * _pointcloud->getModelMatrix();
			glUniformMatrix4fv(_programForPointcloudVR->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &_mvpMatrix_right[0][0]);
		}

		_pointcloud->draw();
		_programForPointcloudVR->unbind();

		return;

	};

	/// <summary>
	/// Author: Sarah
	///set up the buffers used to render to texture to display the scene in HMD-device
	/// </summary>
	bool App::SetupFramebuffers()
	{
		//setting up the framebuffer
		glGenFramebuffers(1, &framebufferID_left);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID_left);

		//create a texture 
		glGenTextures(1, &renderedTexture_left);
		glBindTexture(GL_TEXTURE_2D, renderedTexture_left);

		//allocate the space for texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, RenderWidth, RenderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		//set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//depth buffer
		glGenRenderbuffers(1, &depthbufferID_left);
		glBindRenderbuffer(GL_RENDERBUFFER, depthbufferID_left);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RenderWidth, RenderHeight);

		//put everything into a render target
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID_left);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbufferID_left);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture_left, 0);
		GLenum drawBuffers_left[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers_left);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			printf("Error in creation of frame buffer for render to texture on left eye, check again.");
			return false;
		}
		printf("Framebuffer for left eye successfully created and ready to use");

		//for the right eye
		//setting up the framebuffer
		glGenFramebuffers(1, &framebufferID_right);
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID_right);

		//create a texture 
		glGenTextures(1, &renderedTexture_right);
		glBindTexture(GL_TEXTURE_2D, renderedTexture_right);

		//allocate the space for texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, RenderWidth, RenderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		//set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//depth buffer

		glGenRenderbuffers(1, &depthbufferID_right);
		glBindRenderbuffer(GL_RENDERBUFFER, depthbufferID_right);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, RenderWidth, RenderHeight);

		//put everything into a render target
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID_right);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbufferID_right);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture_right, 0);
		GLenum drawBuffers_right[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers_right);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			printf("Error in creation of frame buffer for render to texture on right eye, check again.");
			return false;
		}
		printf("Framebuffer for right eye successfully created and ready to use");


		return true;

	}

	/// <summary>
	/// Author: Sarah, Jie (render models)
	//render to texture for HMD-device for both eyes
	/// </summary>
	bool App::RenderToTexture() {

		//bind framebuffer for left eye
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID_left);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferID_left);
		glViewport(0, 0, RenderWidth, RenderHeight);

		//render point cloud to texture for left eye 
		RenderVRPointcloud(1);
		
		//render render model to texture for left eye
		for (int rm_id = 0; rm_id < vr::k_unMaxTrackedDeviceCount; rm_id++) {
			if (_render_models[rm_id] != NULL) {
				//printf("render model %d on screen\n", rm_id);
				_programForRendermodel->bind();
				glm::mat4 device_mvp = _vpmatrix_eye_left * _device_pose_matrix[rm_id];
				glUniformMatrix4fv(_programForRendermodel->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &device_mvp[0][0]);
				_render_models[rm_id]->draw();
				_programForRendermodel->unbind();
			}
		}
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//get texture for left eye and submit it to the VR headset
		vr::Texture_t leftEyeTexture = { (void*)(uintptr_t)renderedTexture_left, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);

		//bind framebuffer for right eye
		glBindFramebuffer(GL_FRAMEBUFFER, framebufferID_right);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferID_right);
		glViewport(0, 0, RenderWidth, RenderHeight);

		//render point cloud to texture for right eye 
		RenderVRPointcloud(2);

		//render render model to texture for right eye
		for (int rm_id = 0; rm_id < vr::k_unMaxTrackedDeviceCount; rm_id++) {
			if (_render_models[rm_id] != NULL) {
				//printf("render model %d on screen\n", rm_id);
				_programForRendermodel->bind();
				glm::mat4 device_mvp = _vpmatrix_eye_right * _device_pose_matrix[rm_id];
				glUniformMatrix4fv(_programForRendermodel->getUniformLocation("mvpMatrix"), 1, GL_FALSE, &device_mvp[0][0]);
				_render_models[rm_id]->draw();
				_programForRendermodel->unbind();
			}
		}
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//get texture for right eye and submit it to the VR headset
		vr::Texture_t rightEyeTexture = { (void*)(uintptr_t)renderedTexture_right, vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
		vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);

		return true;

	}
	
	/// <summary>
	///Author: OpenVR Documentation,  https://github.com/ValveSoftware/openvr
	///helper function convert a 3x4 matrix into a 4x4 matrix 
	/// </summary>
	glm::mat4 App::steamvrtoglm(const vr::HmdMatrix34_t& matPose) {

		glm::mat4 pose_matrix = glm::mat4(matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0f,
			matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0f,
			matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0f,
			matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f);

		return pose_matrix;
	}

	/// <summary>
	///Author: OpenVR Documentation,  https://github.com/ValveSoftware/openvr
	///updates the poses for movement
	/// </summary>
	void App::update_pose_matrix() {
		if (!vr_environment)
			return;

		vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);

		ValidPoseCount = 0;
		strPoseClasses = "";
		for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice) {
			if (m_rTrackedDevicePose[nDevice].bPoseIsValid) {
				ValidPoseCount++;
				_device_pose_matrix[nDevice] = steamvrtoglm(m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
				if (DeviceClassChar[nDevice] == 0) {
					switch (vr_environment->GetTrackedDeviceClass(nDevice)) {
						case vr::TrackedDeviceClass_Controller:        DeviceClassChar[nDevice] = 'C'; break;
						case vr::TrackedDeviceClass_HMD:               DeviceClassChar[nDevice] = 'H'; break;
						case vr::TrackedDeviceClass_Invalid:           DeviceClassChar[nDevice] = 'I'; break;
						case vr::TrackedDeviceClass_GenericTracker:    DeviceClassChar[nDevice] = 'G'; break;
						case vr::TrackedDeviceClass_TrackingReference: DeviceClassChar[nDevice] = 'T'; break;
						default:                                       DeviceClassChar[nDevice] = '?'; break;
					}
				}
				strPoseClasses += DeviceClassChar[nDevice];
			}
		}

		if (m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid) {
			_poseHMDmatrix = _device_pose_matrix[vr::k_unTrackedDeviceIndex_Hmd];

		}
		return;
	};

	/// <summary>
	///Author:Mostly based on OpenVR Documentation,  https://github.com/ValveSoftware/openvr
	///returns the projection matrix for one eye
	/// </summary>
	glm::mat4 App::GetHMDMatrixProjectionEye(vr::Hmd_Eye nEye) {
		if (!vr_environment)
			return glm::mat4();

		vr::HmdMatrix44_t matrixproj = vr_environment->GetProjectionMatrix(nEye, m_fNearClip, m_fFarClip);

		return glm::mat4(
			matrixproj.m[0][0], matrixproj.m[1][0], matrixproj.m[2][0], matrixproj.m[3][0],
			matrixproj.m[0][1], matrixproj.m[1][1], matrixproj.m[2][1], matrixproj.m[3][1],
			matrixproj.m[0][2], matrixproj.m[1][2], matrixproj.m[2][2], matrixproj.m[3][2],
			matrixproj.m[0][3], matrixproj.m[1][3], matrixproj.m[2][3], matrixproj.m[3][3]
		);
	};


	/// <summary>
	///Author: Mostly based on OpenVR Documentation,  https://github.com/ValveSoftware/openvr
	///returns the pose matrix for one eye
	/// </summary>
	glm::mat4 App::GetHMDMatrixPoseEye(vr::Hmd_Eye nEye)
	{
		if (!vr_environment)
			return glm::mat4();

		vr::HmdMatrix34_t matrixEye = vr_environment->GetEyeToHeadTransform(nEye);
		glm::mat4 finalmatrix(
			matrixEye.m[0][0], matrixEye.m[1][0], matrixEye.m[2][0], 0.0,
			matrixEye.m[0][1], matrixEye.m[1][1], matrixEye.m[2][1], 0.0,
			matrixEye.m[0][2], matrixEye.m[1][2], matrixEye.m[2][2], 0.0,
			matrixEye.m[0][3], matrixEye.m[1][3], matrixEye.m[2][3], 1.0f
		);

		finalmatrix = glm::inverse(finalmatrix);
		return finalmatrix;
	
	};


	
	/// <summary>
	///Author: Sarah
	/// get the digital action state for jumping, update the pose matrices
	/// If we have a jumping action, we translate the model matrix in the direction of view 
	/// then we get the new projection and pose matrix for each eye & calculate the vpm matrix
	/// Update the action state & check if we want to stop the rendering
	/// </summary>
	void App::update() {


		//get the state of jumping action 
		jumping = GetDigitalActionState(actionJump);

		//update pose matrix
		update_pose_matrix();

		//if jumping action is triggered, translate the model matrix to the view 
		if (jumping)
		{
			printf("jumping is true \n");
			update_pose_matrix();
			glm::vec4 vector_dir = glm::vec4(0.0, 0.0, -0.1,0.0);

			direction = glm::inverse(_poseHMDmatrix) * vector_dir;

			_pointcloud->translateModelMatrix(direction, x_boundary_max, x_boundary_min, y_boundary_max, y_boundary_min, z_boundary_max,z_boundary_min);
			jumping = false;
		};

		//update pose matrix
		update_pose_matrix();

		//update projection and HMD-pose matrix
		_projectionmatrix_eye_left = GetHMDMatrixProjectionEye(vr::Eye_Left);
		_posematrix_eye_left = GetHMDMatrixPoseEye(vr::Eye_Left);

		_projectionmatrix_eye_right = GetHMDMatrixProjectionEye(vr::Eye_Right);
		_posematrix_eye_right = GetHMDMatrixPoseEye(vr::Eye_Right);

		_vpmatrix_eye_left = _projectionmatrix_eye_left * _posematrix_eye_left * glm::inverse(_poseHMDmatrix);
		_vpmatrix_eye_right = _projectionmatrix_eye_right * _posematrix_eye_right * glm::inverse(_poseHMDmatrix);

		//update action state and check if the action to stop the main loop is triggered. 
		vr::VRActiveActionSet_t actionSet = { 0 };
		actionSet.ulActionSet = actionsetMasterProject;
		vr::VRInput()->UpdateActionState(&actionSet, sizeof(actionSet), 1);

		StopMainLoop = !GetDigitalActionState(actionStopMainLoop);
		
		return;
	};

	/// <summary>
	///Author: Sarah
	///Action function to stop the main loop
	/// </summary>
	bool App::getStopMainLoopVal()
	{
		if (StopMainLoop == false)
		{
			printf("Stop the main loop \n");
		}
		return StopMainLoop;
	}


	/// <summary>
	///Author: Sarah
	///helper function to get the vp-matrix of left eye
	/// </summary>
	glm::mat4 App::getvpmatrix_left()
	{
		return _vpmatrix_eye_left;
	};

	/// <summary>
	///Author: Sarah
	///helper function to get the vp-matrix of right eye
	/// </summary>
	glm::mat4 App::getvpmatrix_right()
	{
		return _vpmatrix_eye_right;
	};

	/// <summary>
	///Author: Jie
	///track the device and init the render model
	/// </summary>
	bool App::setup_render_model(vr::TrackedDeviceIndex_t tracked_device) {

		if (tracked_device >=vr::k_unMaxTrackedDeviceCount)
			return false;

		string render_model_name = GetTrackedDeviceString(vr_environment, tracked_device, vr::Prop_RenderModelName_String);
		//std::cout << "Render model name:" << render_model_name << std::endl;

		_render_models[tracked_device] = new RenderModel(render_model_name.c_str());

		cout << "Starting initialization of render model data for device " << tracked_device << endl;
		if (_render_models[tracked_device]->init()) {
			//cout << "Render model for device " << tracked_device << " correctly initialized" << endl;
			return true;
		}

		cout << "Unable to generate render model data for device " << tracked_device << endl;
		return false;
	}
	
	/// <summary>
	///Author: Sarah, Jie (render model) 
	///Some parts are based on the OpenVR Documentation, https://github.com/ValveSoftware/openvr
	/// 
	///looks if HMD-device is present and init VR System, VR Compositor used to display image in HMD-device, computes first vp-matrix of both eyes, prepare for render function (get recommended target size), set up buffer to render
	/// </summary>
	int App::initVR()
	{
		for (int nDevice = 0; nDevice < k_unMaxTrackedDeviceCount; nDevice++) {
			_device_pose_matrix[nDevice] = glm::mat4(1.0);
			_render_models[nDevice] = NULL;
		}
		
		//check if the HMD is present
		if (vr::VR_IsHmdPresent()) {
			printf("HMD is present\n");
		}
		else {
			printf("HMD is not present \n");
		};

		//initialize the vr system
		vr::EVRInitError errorinit = vr::VRInitError_None;
		vr_environment = vr::VR_Init(&errorinit, vr::VRApplication_Scene);
		if (errorinit != vr::VRInitError_None) {
			vr_environment = NULL;
			printf("not able to initalize the VR System \n");
			return -1;
		}
		else {
			printf("Initialization of VR System successful \n");
		};

		//set up the render models
		for (uint32_t td = vr::k_unTrackedDeviceIndex_Hmd; td < vr::k_unMaxTrackedDeviceCount; td++) {

			if (vr_environment->IsTrackedDeviceConnected(td)) {
				vr::ETrackedDeviceClass tracked_device_class = vr_environment->GetTrackedDeviceClass(td);

				std::cout << "Tracking device " << td << " is connected " << std::endl;
				std::cout << "  Device type: " << GetTrackedDeviceClassString(tracked_device_class) << ". Name: " << GetTrackedDeviceString(vr_environment, td, vr::Prop_TrackingSystemName_String) << endl;

				if (tracked_device_class == vr::ETrackedDeviceClass::TrackedDeviceClass_TrackingReference) _base_stations_count++;
				else if (tracked_device_class == vr::TrackedDeviceClass_Controller) {
					if (!setup_render_model(td))
						return -1;
				}				
				
				if (td == vr::k_unTrackedDeviceIndex_Hmd) {
					// Fill variables used for obtaining the device name and serial ID (used later for naming the SDL window)
					_strDriver = GetTrackedDeviceString(vr_environment, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String);
					_strDisplay = GetTrackedDeviceString(vr_environment, vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SerialNumber_String);
				}	
			}
			else
				std::cout << "Tracking device " << td << " not connected" << std::endl;
		}

		// Check whether both base stations are found
		if (_base_stations_count < 1) {
			cout << "There was a problem indentifying the base stations, please check they are powered on" << endl;
			return -1;
		}
		
		//initialize VR Compositor
		if (!vr::VRCompositor()) {
			printf("VR Compositor initialization failed\n");
			return -1;
		}
		else {
			printf("VR Compositor initialization was successful \n");
		};

		//update projection and pose matrix, calculate the vp-matrix
		_projectionmatrix_eye_left = GetHMDMatrixProjectionEye(vr::Eye_Left);
		_projectionmatrix_eye_right = GetHMDMatrixProjectionEye(vr::Eye_Right);
		_posematrix_eye_left = GetHMDMatrixPoseEye(vr::Eye_Left);
		_posematrix_eye_right = GetHMDMatrixPoseEye(vr::Eye_Right);
		_vpmatrix_eye_left = _projectionmatrix_eye_left * _posematrix_eye_left;
		_vpmatrix_eye_right = _projectionmatrix_eye_right * _posematrix_eye_right;

		if (_projectionmatrix_eye_left != glm::mat4() && _posematrix_eye_left != glm::mat4() && _projectionmatrix_eye_right != glm::mat4() && _posematrix_eye_right != glm::mat4()) {
			printf("the projection matrix and the pose matrix of both eyes has value in it.\n");
		}
		else {
			printf("the projection matrix or the pose matrix of the eye is empty");
			printf("first value of projection matrix left eye: %f \n", _projectionmatrix_eye_left[0][0]);
			printf("first value of projection matrix right eye: %f \n", _projectionmatrix_eye_right[0][0]);
			printf("first value of pose matrix left eye: %f \n", _posematrix_eye_left[0][0]);
			printf("first value of pose matrix right eye: %f \n", _posematrix_eye_right[0][0]);
		};
		
		//setup framebuffer and shader program
		SetupRenderVR();
		SetupFramebuffers();
		/*
		//define actions and the path to them
		vr::VRInput()->SetActionManifestPath("C:/Users/feuzs/OneDrive/Desktop/bereinigt/vr/actions.json");
		vr::VRInput()->GetActionHandle("/actions/masterproject/in/Stop", &actionStopMainLoop);
		vr::VRInput()->GetActionHandle("/actions/masterproject/in/Jump", &actionJump);
		vr::VRInput()->GetActionSetHandle("/actions/masterproject", &actionsetMasterProject);
		*/
		
		vr::VRInput()->SetActionManifestPath("D:/study/UZH/MasterProject/vr_pointcloud_viewer/vr/actions.json");
		//vr::VRInput()->SetActionManifestPath("C:/Users/Sarah/Desktop/project/vr_pointcloud_viewer/vr/actions.json");
		vr::VRInput()->GetActionHandle("/actions/masterproject/in/Stop", &actionStopMainLoop);
		vr::VRInput()->GetActionHandle("/actions/masterproject/in/Jump", &actionJump);
		vr::VRInput()->GetActionSetHandle("/actions/masterproject", &actionsetMasterProject);
		
		return 1;
	};


};