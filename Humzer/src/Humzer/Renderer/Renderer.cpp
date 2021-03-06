#include "humpch.h"
#include "Renderer.h"
#include "PrimitiveData.h"
#include "glm/ext/matrix_transform.inl"
#include "Texture.h"


namespace Humzer {

	Ref<ShaderLibrary> Renderer::s_ShaderLibrary = CreateRef<ShaderLibrary>();

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

		void Renderer::Init()
		{
			s_ShaderLibrary = CreateRef<ShaderLibrary>();
		}

		//Ref<ShaderLibrary> s_ShaderLibrary = CreateRef<ShaderLibrary>();

		void Renderer::BeginScene(Camera& camera, const glm::mat4& transform)
		{
			glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

			s_SceneData->ViewProjectionMatrix = viewProj;
		}

		void Renderer::EndScene()
		{
		}

		void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, glm::mat4 transform)
		{
			shader->Bind();
			shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
			shader->SetMat4("u_Transform", transform);

			vertexArray->Bind();
			RenderCommand::DrawIndexed(vertexArray);
		}

		Humzer::Ref<Humzer::ShaderLibrary> Renderer::GetShaderLibrary()
		{
			return s_ShaderLibrary;
		}

		void Renderer::OnWindowResize(uint32_t width, uint32_t height)
		{
			RenderCommand::SetViewport(0, 0, width, height);
		}

		// RENDERER 3D

		struct Renderer3DStorage {
			glm::mat4 m_CameraTransform;
			glm::vec3 m_CameraTranslation;

			Ref<VertexArray> PlaneVAO;
			Ref<VertexArray> CubeVAO;
			Ref<VertexArray> SkyboxVAO;

			
			Ref<Shader> m_FlatColorShader;
			Ref<Shader> m_TexturedShader;
			Ref<Shader> m_SkyboxShader;
			Ref<Shader> m_MeshShader;
		};

		static Renderer3DStorage* s_Data;

		void Renderer3D::Init()
		{
			s_Data = new Renderer3DStorage();

			BufferLayout base_layout = {
				{ ShaderDataType::Float3, "a_Position"},
				{ ShaderDataType::Float3, "a_Normals"},
				{ ShaderDataType::Float4, "a_Color"},
				{ ShaderDataType::Float2, "a_TexCoord"},
			};

			BufferLayout skybox_layout = {
				{ ShaderDataType::Float3, "a_Position"},
			};

			std::vector<float> vertices = {};
			std::vector<uint32_t> indices = {};

			// --- PLANE SET UP ---
			s_Data->PlaneVAO = VertexArray::Create();

			Primitive::getQuadData(vertices, indices);
			Ref<VertexBuffer> PlaneVBO = VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float));
			PlaneVBO->SetLayout(base_layout);
			s_Data->PlaneVAO->AddVertexBuffer(PlaneVBO);

			Ref<IndexBuffer> PlaneEBO = IndexBuffer::Create(&indices[0], indices.size() * sizeof(uint32_t));
			s_Data->PlaneVAO->SetIndexBuffer(PlaneEBO);

			// --- CUBE SET UP ---
			s_Data->CubeVAO = VertexArray::Create();

			Primitive::getCubeData(vertices, indices);
			Ref<VertexBuffer> CubeVBO = VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float));
			CubeVBO->SetLayout(base_layout);
			s_Data->CubeVAO->AddVertexBuffer(CubeVBO);

			Ref<IndexBuffer> CubeEBO = IndexBuffer::Create(&indices[0], indices.size() * sizeof(uint32_t));
			s_Data->CubeVAO->SetIndexBuffer(CubeEBO);

			// --- SKYBOX SET UP ---
			s_Data->SkyboxVAO = VertexArray::Create();

			Primitive::getSkyboxData(vertices);
			Ref<VertexBuffer> SkyboxVBO = VertexBuffer::Create(&vertices[0], vertices.size() * sizeof(float));
			SkyboxVBO->SetLayout(skybox_layout);
			s_Data->SkyboxVAO->AddVertexBuffer(SkyboxVBO);

			// --- SHADERS SET UP ---
			s_Data->m_FlatColorShader = Renderer::GetShaderLibrary()->Load("flat_color", "Resources/shaders/flat_colored.vs", "Resources/shaders/flat_colored.fs");
			s_Data->m_TexturedShader = Renderer::GetShaderLibrary()->Load("textured", "Resources/shaders/textured_shader.vs", "Resources/shaders/textured_shader.fs");
			s_Data->m_SkyboxShader = Renderer::GetShaderLibrary()->Load("skybox", "Resources/shaders/skybox.vs", "Resources/shaders/skybox.fs");
			s_Data->m_MeshShader = Renderer::GetShaderLibrary()->Load("mesh_base", "Resources/shaders/mesh_base_shader.vs", "Resources/shaders/mesh_base_shader.fs");

			s_Data->m_TexturedShader->Bind();
			s_Data->m_TexturedShader->SetInt("u_Texture", 0); // BIND TEXTURE SLOT

			s_Data->m_SkyboxShader->Bind();
			s_Data->m_SkyboxShader->SetInt("u_Skybox", 0); // BIND TEXTURE SLOT

			s_Data->m_MeshShader->Bind();
			s_Data->m_MeshShader->SetInt("u_Material.diffuse", 0);
		}

		void Renderer3D::Shutdown()
		{
			delete s_Data;
		}

		Humzer::Camera* Renderer3D::s_SceneCamera = nullptr;

		void Renderer3D::BeginScene(Camera& camera, const glm::mat4& transform, const glm::vec3 cameraPos)
		{
			s_SceneCamera = &camera;

			glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

			s_Data->m_CameraTranslation = cameraPos;
			s_Data->m_CameraTransform = transform;

			s_Data->m_FlatColorShader->Bind();
			s_Data->m_FlatColorShader->SetMat4("u_ViewProjection", viewProj);
			s_Data->m_FlatColorShader->SetMat4("u_Transform", glm::mat4(1.0));
			s_Data->m_FlatColorShader->SetFloat3("u_ViewPos", cameraPos);

			// TEMPORARY (BEFORE MATERIAL):
			s_Data->m_FlatColorShader->SetFloat3("u_Material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
			s_Data->m_FlatColorShader->SetFloat3("u_Material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
			s_Data->m_FlatColorShader->SetFloat3("u_Material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			s_Data->m_FlatColorShader->SetInt("u_Material.shininess", 32.0f);

			s_Data->m_TexturedShader->Bind();
			s_Data->m_TexturedShader->SetMat4("u_ViewProjection", viewProj);
			s_Data->m_TexturedShader->SetMat4("u_Transform", glm::mat4(1.0));

			// MESH SHADER
			s_Data->m_MeshShader->Bind();
			s_Data->m_MeshShader->SetMat4("u_ViewProjection", viewProj);
			s_Data->m_MeshShader->SetMat4("u_Transform", glm::mat4(1.0));
			s_Data->m_MeshShader->SetFloat3("u_ViewPos", cameraPos);

			// SKYBOX SHADER
			s_Data->m_SkyboxShader->Bind();
			glm::mat4 view = glm::mat4(glm::mat3(glm::inverse(transform)));
			s_Data->m_SkyboxShader->SetMat4("u_ViewProjection", camera.GetProjection() * view);
			
		}

		void Renderer3D::BeginScene(const EditorCamera& camera)
		{
			s_SceneCamera = &(Camera)camera;

			glm::mat4 viewProj = camera.GetViewProjection();

			s_Data->m_CameraTranslation = camera.GetPosition();
			s_Data->m_CameraTransform = glm::translate(glm::mat4(1.0), camera.GetPosition());

			s_Data->m_FlatColorShader->Bind();
			s_Data->m_FlatColorShader->SetMat4("u_ViewProjection", viewProj);
			s_Data->m_FlatColorShader->SetMat4("u_Transform", glm::mat4(1.0));
			s_Data->m_FlatColorShader->SetFloat3("u_ViewPos", camera.GetPosition());

			// TEMPORARY (BEFORE MATERIAL):
			s_Data->m_FlatColorShader->SetFloat3("u_Material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
			s_Data->m_FlatColorShader->SetFloat3("u_Material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
			s_Data->m_FlatColorShader->SetFloat3("u_Material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			s_Data->m_FlatColorShader->SetInt("u_Material.shininess", 32.0f);

			s_Data->m_TexturedShader->Bind();
			s_Data->m_TexturedShader->SetMat4("u_ViewProjection", viewProj);
			s_Data->m_TexturedShader->SetMat4("u_Transform", glm::mat4(1.0));

			// MESH SHADER
			s_Data->m_MeshShader->Bind();
			s_Data->m_MeshShader->SetMat4("u_ViewProjection", viewProj);
			s_Data->m_MeshShader->SetMat4("u_Transform", glm::mat4(1.0));
			s_Data->m_MeshShader->SetFloat3("u_ViewPos", camera.GetPosition());

			// SKYBOX SHADER
			s_Data->m_SkyboxShader->Bind();
			glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
			s_Data->m_SkyboxShader->SetMat4("u_ViewProjection", camera.GetProjection() * view);
		}

		void Renderer3D::EndScene()
		{

		}

		void Renderer3D::DrawPlane(const glm::mat4 transform, const glm::vec4& color, uint32_t entityID)
		{
			s_Data->m_FlatColorShader->Bind();
			s_Data->m_FlatColorShader->SetFloat4("u_Color", color);
			s_Data->m_FlatColorShader->SetInt("u_ObjectID", entityID);

			s_Data->m_FlatColorShader->SetMat4("u_Transform", transform);

			s_Data->PlaneVAO->Bind();
			RenderCommand::DrawIndexed(s_Data->PlaneVAO);
		}

		void Renderer3D::DrawPlane(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
		{
			s_Data->m_FlatColorShader->Bind();
			s_Data->m_FlatColorShader->SetFloat4("u_Color", color);
			
			glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * glm::scale(glm::mat4(1.0), {size.x, size.y, 1.0f});
			s_Data->m_FlatColorShader->SetMat4("u_Transform", transform);

			s_Data->PlaneVAO->Bind();
			RenderCommand::DrawIndexed(s_Data->PlaneVAO);
		}

		void Renderer3D::DrawPlane(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
		{
			s_Data->m_TexturedShader->Bind();
			
			glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0f });
			s_Data->m_TexturedShader->SetMat4("u_Transform", transform);

			texture->Bind();

			s_Data->PlaneVAO->Bind();
			RenderCommand::DrawIndexed(s_Data->PlaneVAO);
		}

		void Renderer3D::DrawCube(const glm::mat4 transform, const glm::vec4& color, uint32_t entityID)
		{
			s_Data->m_FlatColorShader->Bind();
			s_Data->m_FlatColorShader->SetFloat4("u_Color", color);
			s_Data->m_FlatColorShader->SetInt("u_ObjectID", entityID);

			s_Data->m_FlatColorShader->SetMat4("u_Transform", transform);

			s_Data->CubeVAO->Bind();
			RenderCommand::DrawIndexed(s_Data->CubeVAO);
		}

		void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color)
		{
			s_Data->m_FlatColorShader->Bind();
			s_Data->m_FlatColorShader->SetFloat4("u_Color", color);

			glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * glm::scale(glm::mat4(1.0), size);
			s_Data->m_FlatColorShader->SetMat4("u_Transform", transform);

			s_Data->CubeVAO->Bind();
			RenderCommand::DrawIndexed(s_Data->CubeVAO);
		}

		void Renderer3D::DrawCube(const glm::vec3& position, const glm::vec3& size, const Ref<Texture2D>& texture)
		{
			s_Data->m_TexturedShader->Bind();

			glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * glm::scale(glm::mat4(1.0), size);
			s_Data->m_TexturedShader->SetMat4("u_Transform", transform);

			texture->Bind();

			s_Data->CubeVAO->Bind();
			RenderCommand::DrawIndexed(s_Data->CubeVAO);
		}

		void Renderer3D::DrawSkybox(const Ref<TextureCube>& texture)
		{
			RenderCommand::DisableDepthMask();
			s_Data->m_SkyboxShader->Bind();

			s_Data->SkyboxVAO->Bind();
			texture->Bind();
			RenderCommand::DrawArrays(36);

			RenderCommand::EnableDepthMask();
		}

		void Renderer3D::DrawMesh(const glm::mat4& transform, Ref<Mesh> mesh, uint32_t entityID)
		{
			// SHADER SET UP
			s_Data->m_MeshShader->Bind();
			s_Data->m_MeshShader->SetMat4("u_Transform", transform);
			s_Data->m_MeshShader->SetInt("u_ObjectID", entityID);

			// BEFORE MATERIAL SYSTEM
			s_Data->m_MeshShader->SetFloat3("u_Material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			s_Data->m_MeshShader->SetInt("u_Material.shininess", 32.0f);

			for (size_t i = 0; i < mesh->m_Textures.size(); i++) {
				if(mesh->m_Textures[i])
					mesh->m_Textures[i]->Bind();
			}

			// DRAWING
			mesh->m_VertexArray->Bind();
			RenderCommand::DrawIndexed(mesh->m_VertexArray);
		}

		
		void Renderer3D::DrawMesh(Ref<Mesh> mesh, const glm::vec3& position, const glm::vec3& scale)
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * glm::scale(glm::mat4(1.0), scale);

			// SHADER SET UP
			s_Data->m_MeshShader->Bind();
			s_Data->m_MeshShader->SetMat4("u_Transform", transform);

			// BEFORE MATERIAL SYSTEM
			s_Data->m_MeshShader->SetFloat3("u_Material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			s_Data->m_MeshShader->SetInt("u_Material.shininess", 32.0f);

			for (size_t i = 0; i < mesh->m_Textures.size(); i++) {
				if (mesh->m_Textures[i])
					mesh->m_Textures[i]->Bind();
			}

			// DRAWING
			mesh->m_VertexArray->Bind();
			RenderCommand::DrawIndexed(mesh->m_VertexArray);
		}

		// 2D RENDERER

		struct QuadVertex {
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexIndex;
			float TilingFactor;
			uint32_t ObjectID;
		};

		struct Renderer2DStorage
		{
			static const uint32_t MaxQuads = 10000;
			static const uint32_t MaxVertices = MaxQuads * 4;
			static const uint32_t MaxIndices = MaxQuads * 6;
			static const uint32_t MaxTextureSlots = 32; // #TODO: Render Caps

			uint32_t QuadIndexCount = 0;
			QuadVertex* QuadVertexBufferBase = nullptr;
			QuadVertex* QuadVertexBufferPtr = nullptr;

			glm::vec4 QuadVertexPositions[4];

			std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
			uint32_t TextureSlotIndex = 1; // Since 0 will always be white texture

			Ref<VertexArray> QuadVertexArray;
			Ref<VertexBuffer> QuadVertexBuffer;
			Ref<Shader> TextureShader;
			Ref<Texture2D> WhiteTexture;

			Renderer2D::Statistics Stats;
		};

		static Renderer2DStorage* s_Data2D;


		void Renderer2D::Init()
		{
			s_Data2D = new Renderer2DStorage();

			// VAO Creation
			s_Data2D->QuadVertexArray = VertexArray::Create();

			// VBO Creation (allocate size)
			s_Data2D->QuadVertexBuffer = VertexBuffer::Create(s_Data2D->MaxVertices * sizeof(QuadVertex));
			s_Data2D->QuadVertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float4, "a_Color"},
				{ ShaderDataType::Float2, "a_TexCoord" },
				{ ShaderDataType::Float, "a_TexIndex"},
				{ ShaderDataType::Float, "a_TilingFactor"},
				{ ShaderDataType::Int, "a_ObjectID" },
			});
			s_Data2D->QuadVertexArray->AddVertexBuffer(s_Data2D->QuadVertexBuffer);

			s_Data2D->QuadVertexBufferBase = new QuadVertex[s_Data2D->MaxVertices];

			uint32_t* quadIndices = new uint32_t[s_Data2D->MaxIndices];

			uint32_t offset = 0;
			for (uint32_t i = 0; i < s_Data2D->MaxIndices; i += 6)
			{
				quadIndices[i + 0] = offset + 0;
				quadIndices[i + 1] = offset + 1;
				quadIndices[i + 2] = offset + 2;

				quadIndices[i + 3] = offset + 2;
				quadIndices[i + 4] = offset + 3;
				quadIndices[i + 5] = offset + 0;

				offset += 4;
			}

			Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data2D->MaxIndices * sizeof(uint32_t));
			s_Data2D->QuadVertexArray->SetIndexBuffer(quadIB);
			delete[] quadIndices;

			// TEXTURE INIT
			s_Data2D->WhiteTexture = Texture2D::Create(1, 1); // 1x1 Texture (1 pixel)
			uint32_t whiteTextureData = 0xffffffff; // RGBA White
			s_Data2D->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

			int32_t samplers[s_Data2D->MaxTextureSlots];
			for (uint32_t i = 0; i < s_Data2D->MaxTextureSlots; i++)
				samplers[i] = i;
			

			// SHADERS INIT
			s_Data2D->TextureShader = Renderer::GetShaderLibrary()->Load("texture_2d", "Resources/shaders/Texture2D.vs", "Resources/shaders/Texture2D.fs");
			s_Data2D->TextureShader->Bind();
			s_Data2D->TextureShader->SetIntArray("u_Textures", samplers, s_Data2D->MaxTextureSlots); // Sampler

			// Set quad vertex positions
			s_Data2D->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
			s_Data2D->QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
			s_Data2D->QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
			s_Data2D->QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
			
			s_Data2D->TextureSlots[0] = s_Data2D->WhiteTexture;
		}

		void Renderer2D::Shutdown()
		{
			delete s_Data2D;
		}


		void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& camTransform)
		{
			glm::mat4 viewProj = camera.GetProjection() * glm::inverse(camTransform);

			s_Data2D->TextureShader->Bind();
			s_Data2D->TextureShader->SetMat4("u_ViewProjection", viewProj);

			// Start Batch
			s_Data2D->QuadVertexBufferPtr = s_Data2D->QuadVertexBufferBase;
			s_Data2D->QuadIndexCount = 0;
			s_Data2D->TextureSlotIndex = 1; // Since 0 will always be white texture
		}

		void Renderer2D::BeginScene(const EditorCamera& camera)
		{
			glm::mat4 viewProj = camera.GetViewProjection();

			s_Data2D->TextureShader->Bind();
			s_Data2D->TextureShader->SetMat4("u_ViewProjection", viewProj);

			// Start Batch
			s_Data2D->QuadVertexBufferPtr = s_Data2D->QuadVertexBufferBase;
			s_Data2D->QuadIndexCount = 0;
			s_Data2D->TextureSlotIndex = 1; // Since 0 will always be white texture
		}

		void Renderer2D::EndScene()
		{
			Flush();
		}

		void Renderer2D::Flush()
		{
			if (s_Data2D->QuadIndexCount == 0)
				return; // Nothing to draw

			uint32_t totalDataSize = (uint32_t)((uint8_t*)s_Data2D->QuadVertexBufferPtr - (uint8_t*)s_Data2D->QuadVertexBufferBase); // (uint8_t because we have to cast to a 1 byte type to make the subtraction)
			s_Data2D->QuadVertexBuffer->SetData(s_Data2D->QuadVertexBufferBase, totalDataSize);

			// Bind Textures
			for (uint32_t i = 0; i < s_Data2D->TextureSlotIndex; i++) {
				s_Data2D->TextureSlots[i]->Bind(i);
			}

			s_Data2D->QuadVertexArray->Bind();
			RenderCommand::DrawIndexed(s_Data2D->QuadVertexArray, s_Data2D->QuadIndexCount);

#ifdef HUM_DEBUG
			s_Data2D->Stats.DrawCalls++;
#endif
		}


		void Renderer2D::FlushAndReset() {
			EndScene();

			// Start Batch
			s_Data2D->QuadVertexBufferPtr = s_Data2D->QuadVertexBufferBase;
			s_Data2D->QuadIndexCount = 0;
			s_Data2D->TextureSlotIndex = 1; // Since 0 will always be white texture
		}

		// DRAW COLORED QUAD WITH TRANSFORM
		void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, uint32_t entityID)
		{
			constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
			constexpr size_t quadVertexCount = 4;
			const float texIndex = 0.0f; // White Texture

			if (s_Data2D->QuadIndexCount >= Renderer2DStorage::MaxIndices) {
				FlushAndReset();
			}

			for (size_t i = 0; i < quadVertexCount; i++) // FOR EACH QUAD VERTEX
			{
				s_Data2D->QuadVertexBufferPtr->Position = transform * s_Data2D->QuadVertexPositions[i];
				s_Data2D->QuadVertexBufferPtr->Color = color;
				s_Data2D->QuadVertexBufferPtr->TexCoord = textureCoords[i];
				s_Data2D->QuadVertexBufferPtr->TexIndex = texIndex;

				s_Data2D->QuadVertexBufferPtr->TilingFactor = 1.0f;
				s_Data2D->QuadVertexBufferPtr->ObjectID = entityID;

				s_Data2D->QuadVertexBufferPtr++;
			}

			s_Data2D->QuadIndexCount += 6;

#ifdef HUM_DEBUG
			s_Data2D->Stats.QuadCount++;
#endif

			/*s_Data2D->TextureShader->SetFloat("u_TilingFactor", 1.0f);
			s_Data2D->WhiteTexture->Bind();

			s_Data2D->TextureShader->SetMat4("u_Transform", transform);
			s_Data2D->QuadVertexArray->Bind();
			RenderCommand::DrawIndexed(s_Data2D->QuadVertexArray);*/
		}

		void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
		{
			DrawQuad({ position.x, position.y, 0.0f }, size, color); // #TEMP
		}

		void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
			DrawQuad(transform, color, 0); // #TEMP
;		}

		// DRAW TEXTURED QUAD WITH TRANSFORM
		void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, uint32_t entityID, float tilingFactor /*= 1.0f*/)
		{
			constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

			constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
			constexpr size_t quadVertexCount = 4;
			float texIndex = 0.0f;

			if (s_Data2D->QuadIndexCount >= Renderer2DStorage::MaxIndices) {
				FlushAndReset();
			}

			for (uint32_t i = 1; i < s_Data2D->TextureSlotIndex; i++) { // Iterate to last TextureSlot in use
				if (*s_Data2D->TextureSlots[i].get() == *texture.get()) { // Compare textures id
					texIndex = (float)i;
					break;
				}
			}

			if (texIndex == 0.0f) {
				texIndex = (float)s_Data2D->TextureSlotIndex;
				s_Data2D->TextureSlots[s_Data2D->TextureSlotIndex] = texture;
				s_Data2D->TextureSlotIndex++;
			}

			for (size_t i = 0; i < quadVertexCount; i++) // FOR EACH QUAD VERTEX
			{
				s_Data2D->QuadVertexBufferPtr->Position = transform * s_Data2D->QuadVertexPositions[i];
				s_Data2D->QuadVertexBufferPtr->Color = color;
				s_Data2D->QuadVertexBufferPtr->TexCoord = textureCoords[i];
				s_Data2D->QuadVertexBufferPtr->TexIndex = texIndex;

				s_Data2D->QuadVertexBufferPtr->TilingFactor = tilingFactor;
				s_Data2D->QuadVertexBufferPtr->ObjectID = entityID;

				s_Data2D->QuadVertexBufferPtr++;
			}

			s_Data2D->QuadIndexCount += 6;

#ifdef HUM_DEBUG
			s_Data2D->Stats.QuadCount++;
#endif

			/*s_Data2D->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
			s_Data2D->TextureShader->SetFloat("u_TilingFactor", tilingFactor);
			texture->Bind();

			s_Data2D->TextureShader->SetMat4("u_Transform", transform);

			s_Data2D->QuadVertexArray->Bind();
			RenderCommand::DrawIndexed(s_Data2D->QuadVertexArray);*/
		}

		void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor)
		{
			DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor);
		}

		void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor)
		{
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
			DrawQuad(transform, texture, tilingFactor);
		}

		Humzer::Renderer2D::Statistics Renderer2D::GetStats()
		{
			return s_Data2D->Stats;
		}

		void Renderer2D::ResetStats()
		{
			memset(&s_Data2D->Stats, 0, sizeof(Statistics));
		}

}
