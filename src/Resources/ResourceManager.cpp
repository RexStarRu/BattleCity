#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>

namespace Resorce
{
	ResorceManager::ResorceManager(const std::string& executablePath)
	{
		size_t found = executablePath.find_last_of("/\\");
		m_path = executablePath.substr(0, found);
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	std::shared_ptr<Renderer::ShaderProgram> ResorceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
	{
		std::string vertexString = getFileString(vertexPath);
		if (vertexString.empty())
		{
			std::cerr << "ERROR: no vertex shader!" << std::endl;
			return nullptr;
		}

		std::string fragmentString = getFileString(fragmentPath);
		if (fragmentString.empty())
		{
			std::cerr << "ERROR: no fragment shader!" << std::endl;
			return nullptr;
		}

		std::shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;

		if (newShader->isCompiled())
		{
			return newShader;
		}

		std::cerr << "ERROR: Can't load shader program:\n"
			<< "Vertex: " << vertexPath << "\n"
			<< "Fragment: " << fragmentPath << std::endl;

		return nullptr;
	}
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
	std::shared_ptr<Renderer::ShaderProgram> ResorceManager::getShaderProgram(const std::string& shaderName)
	{
		ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
		if (it != m_shaderPrograms.end())
		{
			return it->second;
		}

		std::cerr << "ERROR: Can't find the shader program: " << shaderName << std::endl;
		return nullptr;
	}

	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	std::string ResorceManager::getFileString(const std::string& relativeFilePath) const
	{
		std::ifstream fin;
		fin.open(m_path + '/' + relativeFilePath.c_str(), std::ios::in | std::ios::binary);

		if (!fin.is_open())
		{
			std::cerr << "ERROR: failed to open file: " << relativeFilePath << std::endl;
			return std::string{};
		}

		std::stringstream buffer;
		buffer << fin.rdbuf();
		return buffer.str();
	}
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
}