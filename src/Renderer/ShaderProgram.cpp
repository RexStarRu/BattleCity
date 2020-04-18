#include "ShaderProgram.h"

#include <iostream>

namespace Renderer
{
	//------------------------------------------------------------------------------------------------------------------------------------
	ShaderProgram::ShaderProgram(const std::string & vertexShader, const std::string & fragmentShader)
	{
		GLuint vertexShaderID;
		if (!createShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
		{
			std::cerr << "ERROR: VERTEX::SHADER Compiled time-error!" << std::endl;
			return;
		}

		GLuint fragmentShaderID;
		if (!createShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
		{
			std::cerr << "ERROR: FRAGMENT::SHADER Compiled time-error!" << std::endl;
			glDeleteShader(vertexShaderID);
			return;
		}

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShaderID);                 //аттачим вертексный шейдер к шейдорной программе
		glAttachShader(m_ID, fragmentShaderID);               //аттачим фрагментный шейдер к шейдорной программе
		glLinkProgram(m_ID);								  //линкуем все в шейдерной программе

		GLint success;
		glGetShaderiv(m_ID, GL_LINK_STATUS, &success);

		if (!success)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(m_ID, 1024, nullptr, infoLog);

			std::cerr << "ERROR: LINK::SHADER::PROGRAM time-error!\n" << infoLog << std::endl;
		}
		else
		{
			m_isCompiled = true;
		}

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);

	}

	//--------------------------------------------------------------------------------------------------------------------------------------------

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
	{
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------
	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
	{
		if (this == &shaderProgram)
			return *this;

		glDeleteProgram(m_ID);
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------
	bool ShaderProgram::createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID)
	{
		shaderID = glCreateShader(shaderType);
		const char* code = source.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);

		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			GLchar infoLog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);

			std::cerr << "ERROR: SHADER::Compiled time-error!\n" << infoLog << std::endl;
		
			return false;
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------
	void ShaderProgram::use() const
	{
		glUseProgram(m_ID);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------
	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_ID);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------
}