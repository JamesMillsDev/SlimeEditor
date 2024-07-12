#pragma once

namespace Catalyst
{
	namespace Utilities
	{
		class Config;
		class GameTime;
	}

	namespace Levels
	{
		class LevelManager;
	}

	class Screen;
	class GameInstance;

	using Utilities::Config;
	using Levels::LevelManager;

	class Application
	{
		friend class Utilities::GameTime;

	public:
		Application();
		~Application();

		Application(const Application&) = delete;
		Application(Application&&) = delete;

	public:
		static Config* GetConfig();
		static void Quit();

	public:
		void Run(GameInstance* _game);

	public:
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

	private:
		static Application* m_instance;
		Config* m_config;

	private:
		GameInstance* m_game;
		LevelManager* m_levelManager;
		Screen* m_screen;

	private:
		static void InitTime();
		static void PollTime();

	};
}
