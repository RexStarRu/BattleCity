#pragma once

#include <memory>
#include <string>
#include <map>

namespace Renderer
{
	class ShaderProgram;
}

namespace Resorce
{
	class ResorceManager
	{
	public:
		ResorceManager(const std::string& executablePath);
		~ResorceManager() = default;

		ResorceManager(const ResorceManager&) = delete;
		ResorceManager(ResorceManager&&) = delete;
		ResorceManager& operator=(const ResorceManager&) = delete;
		ResorceManager& operator=(ResorceManager&&) = delete;

		std::shared_ptr<Renderer::ShaderProgram> loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath);
		std::shared_ptr<Renderer::ShaderProgram> getShaderProgram(const std::string& shaderName);

	private:
		std::string getFileString(const std::string& relativeFilePath) const;

		typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
		ShaderProgramsMap m_shaderPrograms;

		std::string m_path;
	};
}