#pragma once

namespace Catalyst
{
	namespace Levels
	{
		class LevelManager;
	}

	class Screen;

	using Levels::LevelManager;

	class GameInstance
	{
		friend class Application;

	public:
		GameInstance(const GameInstance&) = delete;
		GameInstance(GameInstance&&) = delete;

	public:
		virtual void BeginPlay();

		virtual void Tick();
		virtual void Render();

		virtual void EndPlay();

	public:
		GameInstance& operator=(const GameInstance&) = delete;
		GameInstance& operator=(GameInstance&&) = delete;

	protected:
		Screen* m_screen;
		LevelManager* m_levelManager;

	protected:
		GameInstance();
		virtual ~GameInstance();

	};
}
