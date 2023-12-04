#ifndef VR_SHADERPROGRAM_H
#define VR_SHADERPROGRAM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

namespace MasterProject {

	class ShaderProgram {
	public:
		ShaderProgram(std::string _name);

		void deleteShaderProgramFromGPU();
		void bind() const;
		void unbind() const;

		GLuint getUniformLocation(const std::string& _uniformName);

	private:
		bool loadAndCompileShaderToGPU(GLuint _program, const std::string& _source);
		bool checkLinkStatusOfProgram(GLuint _program);
		std::string loadFile(const std::string& filename) const;

		GLuint program = 0;
	};

};
#endif // VR_SHADERPROGRAM_H

