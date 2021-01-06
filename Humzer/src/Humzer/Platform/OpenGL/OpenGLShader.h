#pragma once

#include "../../Renderer/Shader.h"

namespace Humzer {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

		virtual ~OpenGLShader();

		void Bind() const override;
		void Unbind() const override;

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);


		void SetInt(const std::string& name, int value) override;
		void SetIntArray(const std::string& name, int* values, uint32_t count) override;
		void SetFloat(const std::string& name, const float value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;


		void SetMat4(const std::string& name, const glm::mat4& value) override;


		const std::string& GetName() const override;

	private:
		std::string ReadShader(std::string path);
	private:
		uint32_t m_ID;
		std::string m_Name;
	};

}
