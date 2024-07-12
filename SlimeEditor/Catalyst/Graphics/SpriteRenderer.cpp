#include "SpriteRenderer.h"

#include <cstdio>
#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Sprite.h"
#include "Texture.h"
#include "Catalyst/Actors/Actor.h"
#include "Catalyst/Actors/ActorTransform.h"

using glm::mat4;

namespace Catalyst::Graphics
{
	using Actors::ActorTransform;

	SpriteRenderer::SpriteRenderer()
		: m_sprite{ nullptr }, m_tint{ 255, 255, 255, 255 }, m_vertices{}, m_shader{ 0 }, m_vao{ 0 }, m_vbo{ 0 },
		m_ibo{ 0 }, m_currentVertex{ 0 }, m_currentIndex{ 0 }
	{
		BuildShader();
		BuildVertices();
	}

	SpriteRenderer::~SpriteRenderer()
	{
		glDeleteBuffers(1, &m_vbo);
		glDeleteBuffers(1, &m_ibo);
		glDeleteBuffers(1, &m_vao);
		glDeleteProgram(m_shader);

		delete m_sprite;
	}

	void SpriteRenderer::SetSprite(Sprite* _sprite)
	{
		m_sprite = _sprite;
	}

	void SpriteRenderer::Render()
	{
		BeginRender();

		DrawSprite();

		EndRender();
	}

	void SpriteRenderer::PushTexture(const Texture* _texture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture->GlHandle());
		glActiveTexture(GL_TEXTURE0);
	}

	void SpriteRenderer::RotateAround(const float _inX, const float _inY, float& _outX,
		float& _outY, const float _sin, const float _cos)
	{
		_outX = _inX * _cos - _inY * _sin;
		_outY = _inX * _sin + _inY * _cos;
	}

	void SpriteRenderer::BuildShader()
	{
		const char* vertexShader = "\
			#version 150\n \
			in vec4 position; \
			in vec4 colour; \
			in vec2 texcoord; \
			out vec4 vColour; \
			out vec2 vTexCoord; \
			uniform mat4 projectionMatrix; \
			void main() { \
				vColour = colour; \
				vTexCoord = texcoord; \
				gl_Position = projectionMatrix * vec4(position.x, position.y, position.z, 1.0f); \
			}";

		const char* fragmentShader = "#version 150\n \
						in vec4 vColour; \
						in vec2 vTexCoord; \
						in float vTextureID; \
						out vec4 fragColour; \
						uniform sampler2D texture; \
						void main() { \
							vec4 rgba = texture2D(texture, vTexCoord); \
							fragColour = rgba * vColour; \
						if (fragColour.a < 0.001f) discard; }";

		const unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
		const unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vs, 1, &vertexShader, nullptr);
		glCompileShader(vs);

		glShaderSource(fs, 1, &fragmentShader, nullptr);
		glCompileShader(fs);

		m_shader = glCreateProgram();
		glAttachShader(m_shader, vs);
		glAttachShader(m_shader, fs);
		glBindAttribLocation(m_shader, 0, "position");
		glBindAttribLocation(m_shader, 1, "colour");
		glBindAttribLocation(m_shader, 2, "texcoord");
		glLinkProgram(m_shader);

		int success = GL_FALSE;
		glGetProgramiv(m_shader, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			int infoLogLength = 0;
			glGetProgramiv(m_shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* infoLog = new char[infoLogLength];

			glGetProgramInfoLog(m_shader, infoLogLength, nullptr, infoLog);
			printf("Error: Failed to link SpriteBatch shader program!\n%s\n", infoLog);
			delete[] infoLog;
		}

		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	void SpriteRenderer::BuildVertices()
	{
		// create the vao, vio and vbo
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (VERTEX_COUNT + 2) * sizeof(ushort), static_cast<void*>(&m_indices[0]), GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * sizeof(SpriteRenderData), m_vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteRenderData), nullptr);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteRenderData), reinterpret_cast<char*>(16));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteRenderData), reinterpret_cast<char*>(32));
		glBindVertexArray(0);
	}

	void SpriteRenderer::BeginRender() const
	{
		int width = 0;
		int height = 0;
		GLFWwindow* window = glfwGetCurrentContext();
		glfwGetWindowSize(window, &width, &height);

		glUseProgram(m_shader);

		mat4 projection = glm::ortho(0.f, static_cast<float>(width), 0.f, static_cast<float>(height), 1.0f, -101.0f);
		glUniformMatrix4fv(glGetUniformLocation(m_shader, "projectionMatrix"), 1, false, &projection[0][0]);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void SpriteRenderer::DrawSprite()
	{
		if (const ActorTransform* transform = Owner()->Transform())
		{
			PushTexture(m_sprite->source);

			float tlX = (0.0f - transform->origin.x) * (transform->size.x * .5f);
			float tlY = (0.0f - transform->origin.y) * (transform->size.y * .5f);
			float trX = (1.0f - transform->origin.x) * (transform->size.x * .5f);
			float trY = (0.0f - transform->origin.y) * (transform->size.y * .5f);
			float brX = (1.0f - transform->origin.x) * (transform->size.x * .5f);
			float brY = (1.0f - transform->origin.y) * (transform->size.y * .5f);
			float blX = (0.0f - transform->origin.x) * (transform->size.x * .5f);
			float blY = (1.0f - transform->origin.y) * (transform->size.y * .5f);

			if (transform->rotation != 0.0f)
			{
				const float si = glm::sin(glm::radians(transform->rotation));
				const float co = glm::cos(glm::radians(transform->rotation));

				RotateAround(tlX, tlY, tlX, tlY, si, co);
				RotateAround(trX, trY, trX, trY, si, co);
				RotateAround(brX, brY, brX, brY, si, co);
				RotateAround(blX, blY, blX, blY, si, co);
			}

			const short index = m_currentVertex;

			m_vertices[m_currentVertex].pos[0] = transform->position.x + tlX;
			m_vertices[m_currentVertex].pos[1] = transform->position.y + tlY;
			m_vertices[m_currentVertex].pos[2] = 0.f;
			m_vertices[m_currentVertex].pos[3] = 0.f;
			m_vertices[m_currentVertex].color[0] = m_tint.Red();
			m_vertices[m_currentVertex].color[1] = m_tint.Green();
			m_vertices[m_currentVertex].color[2] = m_tint.Blue();
			m_vertices[m_currentVertex].color[3] = m_tint.Alpha();
			m_vertices[m_currentVertex].texCoord[0] = m_sprite->rect.x / static_cast<float>(m_sprite->source->Width());
			m_vertices[m_currentVertex].texCoord[1] = (m_sprite->rect.y + m_sprite->rect.h) / static_cast<float>(m_sprite->source->Height());
			m_currentVertex++;

			m_vertices[m_currentVertex].pos[0] = transform->position.x + trX;
			m_vertices[m_currentVertex].pos[1] = transform->position.y + trY;
			m_vertices[m_currentVertex].pos[2] = 0.f;
			m_vertices[m_currentVertex].pos[3] = 0.f;
			m_vertices[m_currentVertex].color[0] = m_tint.Red();
			m_vertices[m_currentVertex].color[1] = m_tint.Green();
			m_vertices[m_currentVertex].color[2] = m_tint.Blue();
			m_vertices[m_currentVertex].color[3] = m_tint.Alpha();
			m_vertices[m_currentVertex].texCoord[0] = (m_sprite->rect.x + m_sprite->rect.w) / static_cast<float>(m_sprite->source->Width());
			m_vertices[m_currentVertex].texCoord[1] = (m_sprite->rect.y + m_sprite->rect.h) / static_cast<float>(m_sprite->source->Height());
			m_currentVertex++;

			m_vertices[m_currentVertex].pos[0] = transform->position.x + brX;
			m_vertices[m_currentVertex].pos[1] = transform->position.y + brY;
			m_vertices[m_currentVertex].pos[2] = 0.f;
			m_vertices[m_currentVertex].pos[3] = 0.f;
			m_vertices[m_currentVertex].color[0] = m_tint.Red();
			m_vertices[m_currentVertex].color[1] = m_tint.Green();
			m_vertices[m_currentVertex].color[2] = m_tint.Blue();
			m_vertices[m_currentVertex].color[3] = m_tint.Alpha();
			m_vertices[m_currentVertex].texCoord[0] = (m_sprite->rect.x + m_sprite->rect.h) / static_cast<float>(m_sprite->source->Width());
			m_vertices[m_currentVertex].texCoord[1] = m_sprite->rect.y / static_cast<float>(m_sprite->source->Height());
			m_currentVertex++;

			m_vertices[m_currentVertex].pos[0] = transform->position.x + blX;
			m_vertices[m_currentVertex].pos[1] = transform->position.y + blY;
			m_vertices[m_currentVertex].pos[2] = 0.f;
			m_vertices[m_currentVertex].pos[3] = 0.f;
			m_vertices[m_currentVertex].color[0] = m_tint.Red();
			m_vertices[m_currentVertex].color[1] = m_tint.Green();
			m_vertices[m_currentVertex].color[2] = m_tint.Blue();
			m_vertices[m_currentVertex].color[3] = m_tint.Alpha();
			m_vertices[m_currentVertex].texCoord[0] = m_sprite->rect.x / static_cast<float>(m_sprite->source->Width());
			m_vertices[m_currentVertex].texCoord[1] = m_sprite->rect.y / static_cast<float>(m_sprite->source->Height());
			m_currentVertex++;

			m_indices[m_currentIndex++] = index + 0;
			m_indices[m_currentIndex++] = index + 2;
			m_indices[m_currentIndex++] = index + 3;

			m_indices[m_currentIndex++] = index + 0;
			m_indices[m_currentIndex++] = index + 1;
			m_indices[m_currentIndex++] = index + 2;
		}
	}

	void SpriteRenderer::EndRender()
	{
		Flush();

		glUseProgram(0);
	}

	void SpriteRenderer::Flush()
	{
		int depthFunc = GL_LESS;
		glGetIntegerv(GL_DEPTH_FUNC, &depthFunc);
		glDepthFunc(GL_LEQUAL);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(m_currentVertex * sizeof(SpriteRenderData)), m_vertices);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(m_currentIndex * sizeof(ushort)), m_indices);

		glDrawElements(GL_TRIANGLES, m_currentIndex, GL_UNSIGNED_SHORT, nullptr);

		glBindVertexArray(0);

		glDepthFunc(depthFunc);

		// reset vertex, index and texture count
		m_currentIndex = 0;
		m_currentVertex = 0;
	}
}
