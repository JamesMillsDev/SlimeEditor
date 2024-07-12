#pragma once

#include "Catalyst/Types.h"
#include "Catalyst/Actors/ActorComponent.h"
#include "Catalyst/Utilities/Color.h"

namespace Catalyst::Graphics
{
	using Actors::ActorComponent;
	using Utilities::Color;

	class Sprite;
	class Texture;

	constexpr int VERTEX_COUNT = 4;

	class SpriteRenderer final : public ActorComponent
	{
	public:
		SpriteRenderer();
		~SpriteRenderer() override;

	public:
		SpriteRenderer(const SpriteRenderer&) = delete;
		SpriteRenderer(SpriteRenderer&&) = delete;

	public:
		void SetSprite(Sprite* _sprite);

	public:
		SpriteRenderer& operator=(const SpriteRenderer&) = delete;
		SpriteRenderer& operator=(SpriteRenderer&&) = delete;

	protected:
		void Render() override;

	private:
		struct SpriteRenderData
		{
		public:
			float pos[4];
			float color[4];
			float texCoord[2];
		};

	private:
		Sprite* m_sprite;
		Color m_tint;

		SpriteRenderData m_vertices[VERTEX_COUNT];
		ushort m_indices[VERTEX_COUNT + 2];

		uint m_shader;
		uint m_vao;
		uint m_vbo;
		uint m_ibo;

		short m_currentVertex;
		int m_currentIndex;

	private:
		static void PushTexture(const Texture* _texture);
		static void RotateAround(float _inX, float _inY, float& _outX, float& _outY, float _sin, float _cos);

	private:
		void BuildShader();
		void BuildVertices();

		void BeginRender() const;
		void DrawSprite();
		void EndRender();

		void Flush();

	};
}
