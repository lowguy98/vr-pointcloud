#include <shader_program.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <iostream>
#include <fstream>

namespace MasterProject {

	/// <summary>
	///Author: Based on the OpenGL exercise of the Computer Graphics course at UZH.
	///create and init shader program
	/// </summary>
	ShaderProgram::ShaderProgram(std::string _name)
	{
		program = 0;

		// load shader source code
		std::string vertShaderString = loadFile(_name + ".vert");
		std::string fragShaderString = loadFile(_name + ".frag");

		// Build and compile our shader program consisting of
		// a vertex and fragment shader
		GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

		loadAndCompileShaderToGPU(vertShader, vertShaderString);
		loadAndCompileShaderToGPU(fragShader, fragShaderString);

		// Link shaders
		program = glCreateProgram();
		// check if it exists
		assert(program);
		// attach the shaders to the shader program
		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);
		// link the program to make it ready to use by the GPU
		glLinkProgram(program);
		// check the link status of the program and show linking errors
		// if there are any
		checkLinkStatusOfProgram(program);

		// shader programs can be marked as to delete. We don't have
		// to do this here if it is done later anywhen in the program. An explanation is here:
		// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glDeleteShader.xhtml
		//
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
	}

	/// <summary>
	///Author: Based on the OpenGL exercise of the Computer Graphics course at UZH.
	///load the shader files
	/// </summary>
	std::string ShaderProgram::loadFile(const std::string& _filename) const
	{
		std::string result;
		std::ifstream stream(_filename.c_str());

		if (!stream.is_open())
			return result;

		stream.seekg(0, std::ios::end);
		result.reserve(stream.tellg());
		stream.seekg(0, std::ios::beg);

		result.assign(std::istreambuf_iterator<char>(stream),
			std::istreambuf_iterator<char>()
		);

		return result;
	}

	/// <summary>
	///Author: Based on the OpenGL exercise of the Computer Graphics course at UZH.
	///load and compile the shader files to the GPU
	/// </summary>
	bool ShaderProgram::loadAndCompileShaderToGPU(GLuint _program, const std::string& _source)
	{
		GLint status;
		GLchar errorMessage[512];

		char* shadersource = (char*)_source.c_str();
		int shaderstringlength = _source.length();
		glShaderSource(_program, 1, (const GLchar**)&shadersource, &shaderstringlength);
		glCompileShader(_program);
		glGetShaderiv(_program, GL_COMPILE_STATUS, &status);
		if (!status)
		{
			glGetShaderInfoLog(_program, 512, 0, errorMessage);
			std::cerr << "Fragment Shader compilation error:" << std::endl;
			std::cerr << errorMessage << std::endl;
			return false;
		}
		return true;
	}

	/// <summary>
	///Author: Based on the OpenGL exercise of the Computer Graphics course at UZH.
	///check link status of the shader program
	/// </summary>
	bool ShaderProgram::checkLinkStatusOfProgram(GLuint _program)
	{
		GLint status;
		GLchar errorMessage[512];
		glGetProgramiv(_program, GL_LINK_STATUS, &status);
		if (!status)
		{
			glGetProgramInfoLog(_program, 512, 0, errorMessage);
			std::cerr << "Linking error:" << std::endl;
			std::cerr << errorMessage << std::endl;
			return false;
		}
		return true;
	}

	/// <summary>
	///Author: Based on the OpenGL exercise of the Computer Graphics course at UZH.
	///get back uniform location
	/// </summary>
	GLuint ShaderProgram::getUniformLocation(const std::string& _uniformName)
	{
		return glGetUniformLocation(program, _uniformName.c_str());
	}

	/// <summary>
	///Author: Based on the OpenGL exercise of the Computer Graphics course at UZH.
	///delete the shader program
	/// </summary>
	void ShaderProgram::deleteShaderProgramFromGPU()
	{
		glDeleteProgram(program);
	}

	/// <summary>
	///Author: Based on the OpenGL exercise of the Computer Graphics course at UZH.
	///bind the program
	/// </summary>
	void ShaderProgram::bind() const
	{
		glUseProgram(program);
	}

	/// <summary>
	///Author: Based on the OpenGL exercise of the Computer Graphics course at UZH.
	///unbind the program
	/// </summary>
	void ShaderProgram::unbind() const
	{
		glUseProgram(0);
	}

};

