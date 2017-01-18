#pragma once



#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glfw3.lib")
#pragma comment(lib, "glew32s.lib")

#include <Windows.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <list>


/* Helpers */
// TODO: Refactor into helper class etc
inline void CheckCompiledShader(GLuint shader)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		char buffer[512];
		glGetShaderInfoLog(shader, 512, nullptr, buffer);
		printf("Error: Shader %d is not compiled correctly! \n %s", shader, buffer);
#if DEBUG || _DEBUG
		abort();
#endif
		// Exit our program
	}
}
inline const char* glErrorToString(GLenum error)
{
	char* str = nullptr;
	switch (error)
	{
	case GL_INVALID_ENUM:
		str = "GL_INVALID_ENUM";
		break;
	case GL_INVALID_VALUE:
		str = "GL_INVALID_VALUE";
		break;
	case GL_INVALID_OPERATION:
		str = "GL_INVALID_OPERATION";
		break;
	case GL_STACK_OVERFLOW:
		str = "GL_STACK_OVERFLOW";
		break;
	case GL_STACK_UNDERFLOW:
		str = "GL_STACK_UNDERFLOW";
		break;
	case GL_OUT_OF_MEMORY:
		str = "GL_OUT_OF_MEMORY";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		str = "GL_INVALID_FRAMEBUFFER_OPERATION";
		break;
	case GL_CONTEXT_LOST:
		str = "GL_CONTEXT_LOST";
		break;
	case GL_TABLE_TOO_LARGE:
		str = "GL_TABLE_TOO_LARGE";
		break;
	default:
		str = "NON_SPECIFIED";
		break;
	}
	return str;
}
inline void CheckError()
{
	bool hadErrors = false;
	for (GLenum err; (err = glGetError()) != GL_NO_ERROR;)
	{
		hadErrors = true;
		printf("OpenGL Error %d: %s \n", err, glErrorToString(err));
	}
#if DEBUG | _DEBUG
	if (hadErrors)
		abort();
#endif
}
inline const char* ReadFile(const std::string& file)
{
	std::ifstream ifstream(file, std::ios::ate | std::ios::binary);
	int size = (int)ifstream.tellg();
	ifstream.seekg(0);
	if (ifstream.fail())
	{
		return nullptr;
	}
	char * data = new char[size + 1];
	ifstream.read(data, size);
	ifstream.close();
	data[size] = '\0';
	return data;

}

class Logger
{
public:
	static Logger& GetInstance()
	{
		static Logger l;
		return l;
	}
	static void LogInfo(const std::string& str)
	{
		GetInstance().write("[INFO]  " + str);
	}
	static void LogWarning(const std::string& str)
	{
		GetInstance().write("[WARNING]  " + str);
	}
	static void LogOpCode(unsigned short opCode)
	{
		if(!GetInstance().m_JustLogged)
			LogOpCode(opCode, "",false);
	}
	static void LogOpCode(unsigned short opCode, const std::string& info, bool abortP = true)
	{
		std::stringstream ss;
		ss << "[OPCODE]  " << "0x" <<std::hex << opCode << ((!info.empty())?" - ":"") << info;
		GetInstance().write(ss.str());
		GetInstance().m_JustLogged = true;

#if defined(DEBUG) | defined(_DEBUG)
		if(abortP)
		{
			std::wstringstream ss2;
			ss2 << L"Unkown Operator code: " << std::hex << opCode;
			MessageBoxW(NULL, ss2.str().c_str(), L"UNKOWN OPCODE", MB_OK);
			abort();
		}
#endif
	}
	
	static void NewFrame()
	{
		GetInstance().m_JustLogged = false;
	}

	const std::list<std::string> GetLog()
	{
		return log;
	}
private:
	void write(const std::string& str)
	{
#if defined(DEBUG)
		std::cout << str << std::endl;
#endif
		if (outputFile.is_open())
			outputFile << str << std::endl;

		log.push_back(str);
		if (log.size() > 200)
			log.pop_front();
	}
	Logger()
	{
		outputFile.open("log.txt");
		while(outputFile.fail())
		{
			outputFile.open("log.txt");
		}
	}
	~Logger()
	{
		outputFile.close();
	}
	std::ofstream outputFile;
	std::list<std::string> log;
	bool m_JustLogged = false;
};