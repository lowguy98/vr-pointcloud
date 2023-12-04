#ifndef VR_POINTCLOUD_H
#define VR_POINTCLOUD_H

//#include <shape.h>

#include <vector>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace MasterProject {
	class Pointcloud {
	public:
		Pointcloud(int n_points);
		~Pointcloud();
		void draw();
		void translateModelMatrix(glm::vec4 direction, float x_boundary_max, float x_boundary_min, float  y_boundary_max, float y_boundary_min, float  z_boundary_max, float  z_boundary_min);
		float maxvalue_x, minvalue_x;
		float maxvalue_y, minvalue_y;
		float maxvalue_z, minvalue_z;

		float remaining_x_value_max; 
		float remaining_x_value_min; 

		float remaining_y_value_max; 
		float remaining_y_value_min; 

		std::vector<glm::vec3>& getPositions();
		std::vector<glm::vec3>& getColors();
		const glm::mat4& getModelMatrix() const;
		//accessor
		int get_n_points();
		GLuint VRViewVAOID; 

		glm::mat4 rotationMatrix = glm::mat4(1);
		glm::mat4 translationMatrix = glm::mat4(1);

		bool createVertexArray(GLuint posAttribLoc, GLuint colAttribLoc, GLuint normAttribLoc, GLuint texAttribLoc, GLuint tangentsAttribLoc);
		void initIndexBuffer(); 

		GLuint vaoID = 0;
		GLuint posBufferID = 0;
		GLuint colorBufferID = 0;
		GLuint normalBufferID = 0;
		GLuint texCoordsBufferID = 0;
		GLuint tangentBufferID = 0;
		GLuint indexBufferID = 0;

		float buffer_x = 0.0; 
		float buffer_y = 0.0; 



		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> tangents;
		glm::vec3 objectPosition;

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> colors;
		std::vector<glm::vec3> normals;
		std::vector<glm::uvec3> faces;
		std::vector<glm::vec3> faceNormals;

		glm::mat4 modelMatrix = glm::mat4(1);

	private:
		int _n_points;
	};

};
#endif // VR_POINTCLOUD_H