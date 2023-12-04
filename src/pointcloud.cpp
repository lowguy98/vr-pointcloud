#include <pointcloud.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace MasterProject {

	Pointcloud::Pointcloud(int n_points): _n_points(n_points) {
		assert(_n_points > 0);
		positions.resize(_n_points);
		colors.resize(_n_points);
	};

	Pointcloud::~Pointcloud() {
		glDeleteBuffers(1, &posBufferID);
		glDeleteBuffers(1, &colorBufferID);
		glDeleteBuffers(1, &normalBufferID);
		glDeleteBuffers(1, &indexBufferID);
		glDeleteVertexArrays(1, &vaoID);
	};

	/// <summary>
	///Author: Sarah
	///jumping functionality which considers the boundaries
	/// </summary>
	void Pointcloud::translateModelMatrix(glm::vec4 direction, float x_boundary_max, float x_boundary_min, float  y_boundary_max, float y_boundary_min, float z_boundary_max, float z_boundary_min)
	{
		
		printf("Direction x: \n"); 
		printf("%f\n", direction.x);
		printf("Direction y: \n");
		printf("%f\n", direction.y);
		printf("Direction z: \n");
		printf("%f\n", -direction.z);
		
		//checks if there is still room left to hover and then translate the point cloud with respect to direction of view (only x-axis)
		if (buffer_x > -x_boundary_min*0.7)
		{
			if (direction.x > 0)
			{
				printf("Boundary reached, please turn in other direction \n"); 
			}
			else
			{
				buffer_x = buffer_x + direction.x;
				modelMatrix = glm::translate(modelMatrix, glm::vec3(direction.x, 0, 0));
			}
		}
		else if (buffer_x <= -x_boundary_max*0.7)
		{
			if (direction.x < 0)
			{
				printf("Boundary reached, please turn in other direction \n");
			}
			else
			{
				buffer_x = buffer_x + direction.x;
				modelMatrix = glm::translate(modelMatrix, glm::vec3(direction.x, 0, 0));
			}
		}
		else
		{
			buffer_x = buffer_x + direction.x;
			modelMatrix = glm::translate(modelMatrix, glm::vec3(direction.x, 0, 0));
		}

		//checks if there is still room left to hover and then translate the point cloud with respect to direction of view (only y-axis)
		if (buffer_y > z_boundary_max*0.7)
		{
			if (-direction.z > 0)
			{
				printf("Boundary reached, please turn in other direction \n");
			}
			else
			{
				buffer_y = buffer_y + (-direction.z);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -direction.z));
			}
		}
		else if (buffer_y < z_boundary_min*0.7)
		{
			if (-direction.z < 0)
			{
				printf("Boundary reached, please turn in other direction \n");
			}
			else
			{
				buffer_y = buffer_y + (-direction.z);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -direction.z));
			}
		}
		else
		{
			buffer_y = buffer_y + (-direction.z);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -direction.z));
		}
		

		printf("buffer_x: %f\n", buffer_x);
		printf("Boundaries x_max, x_min: %f, %f \n", -x_boundary_max, -x_boundary_min);

		printf("buffer_y; %f \n", buffer_y);
		printf("Boundaries y_max, y_min: %f, %f \n", z_boundary_max, z_boundary_min);


		printf("modelMatrix \n");
		printf("%f, %f, %f\n", modelMatrix[0][0], modelMatrix[1][0], modelMatrix[2][0]);
		printf("%f, %f, %f \n", modelMatrix[0][1], modelMatrix[1][1], modelMatrix[2][1]);
		printf("%f, %f, %f\n", modelMatrix[0][2], modelMatrix[1][2], modelMatrix[2][2]);
		printf("\n");
		

		return;
	}

	const glm::mat4& Pointcloud::getModelMatrix() const {
		return modelMatrix;
	};

	std::vector<glm::vec3>& Pointcloud::getColors() {
		return colors;
	}
	std::vector<glm::vec3>& Pointcloud::getPositions() {
		return positions;
	}

	/// <summary>
	///Author: Jie
	///draw points
	/// </summary>
	void Pointcloud::draw()  {
		glBindVertexArray(vaoID);
		glDrawArrays(GL_POINTS, 0, _n_points);
		glBindVertexArray(0);
	};

	/// <summary>
	///Author: Based on OpenGL Exercise of Computer Graphics Course at UZH
	///init index buffer
	/// </summary>
	void Pointcloud::initIndexBuffer() {
		glGenBuffers(1, &indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(glm::uvec3), faces.data(), GL_STATIC_DRAW);
	}

	/// <summary>
	///Author: Based on OpenGL Exercise of Computer Graphics Course at UZH
	///creates a vertex array
	/// </summary>
	bool Pointcloud::createVertexArray(GLuint posAttribLoc, GLuint colAttribLoc, GLuint normAttribLoc, GLuint texAttribLoc, GLuint tangentsAttribLoc) {
		// check if all buffer locations are somehow defined
		if ((posAttribLoc == GLuint(-1)) ||
			(colAttribLoc == GLuint(-1)) ||
			(normAttribLoc == GLuint(-1)) ||
			(texAttribLoc == GLuint(-1)) ||
			(tangentsAttribLoc == GLuint(-1))) {
			return false;
		}

		// Initialize Vertex Array Object
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);

		// Initialize buffer objects with geometry data
		// for positions
		glGenBuffers(1, &posBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, posBufferID);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(posAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(posAttribLoc);

		// for colors
		glGenBuffers(1, &colorBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(colAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(colAttribLoc);

		// for normals
		if (normals.size() != 0) {
			glGenBuffers(1, &normalBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(normAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(normAttribLoc);
		}

		if (texCoords.size() != 0) {
			glGenBuffers(1, &texCoordsBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordsBufferID);
			glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(texAttribLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(texAttribLoc);
		}

		// for texture coordinates
		if (tangents.size() != 0) {
			glGenBuffers(1, &tangentBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, tangentBufferID);
			glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), tangents.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(tangentsAttribLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(tangentsAttribLoc);
		}

		initIndexBuffer();

		// Reset state
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return true;
	}


	int Pointcloud::get_n_points() { return _n_points; }

};
