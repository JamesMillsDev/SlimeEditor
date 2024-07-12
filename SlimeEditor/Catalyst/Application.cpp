#include "Application.h"

#include <glfw/glfw3.h>

#include "GameInstance.h"
#include "Input.h"
#include "Screen.h"
#include "Levels/LevelManager.h"
#include "Utilities/Config.h"
#include "Utilities/GameTime.h"

namespace Catalyst
{
	using Utilities::GameTime;

	Application* Application::m_instance = nullptr;

	Application::Application()
		: m_game{ nullptr }, m_levelManager{ new LevelManager }, m_screen{ nullptr }
	{
		m_config = new Config(new char[] { "config" });
		m_instance = this;
	}

	Application::~Application()
	{
		delete m_game;
		m_game = nullptr;

		delete m_levelManager;
		m_levelManager = nullptr;

		delete m_config;
		m_config = nullptr;

		delete m_screen;
		m_screen = nullptr;
	}

	Config* Application::GetConfig()
	{
		return m_instance->m_config;
	}

	void Application::Quit()
	{
		m_instance->m_screen->m_shouldClose = true;
	}

	void Application::Run(GameInstance* _game)
	{
		m_config->Load();
		m_screen = new Screen(m_config);

		m_screen->Open();

		Input::Create();

		InitTime();

		m_game = _game;
		m_game->m_screen = m_screen;
		m_game->m_levelManager = m_levelManager;
		m_game->BeginPlay();

		while (!m_screen->ShouldClose())
		{
			PollTime();

			Input::GetInstance()->ClearStatus();

			if(!m_screen->NewFrame())
				continue;

			m_game->Tick();
			m_levelManager->Tick();

			m_game->Render();
			m_levelManager->Render();

			m_screen->EndFrame();
		}

		m_game->EndPlay();
		Input::Destroy();

		m_screen->Close();
	}

	void Application::InitTime()
	{
		GameTime::m_prevTime = glfwGetTime();
	}

	void Application::PollTime()
	{
		GameTime::m_currTime = glfwGetTime();
		GameTime::m_deltaTime = GameTime::m_currTime - GameTime::m_prevTime;

		if (GameTime::m_deltaTime > 0.1)
			GameTime::m_deltaTime = 0.1;

		GameTime::m_prevTime = GameTime::m_currTime;

		// Update fps every second
		GameTime::m_frames++;
		GameTime::m_fpsInterval += GameTime::m_deltaTime;
		if (GameTime::m_fpsInterval >= 1.0)
		{
			GameTime::m_fps = GameTime::m_frames;
			GameTime::m_frames = 0;
			GameTime::m_fpsInterval -= 1.0;
		}
	}
}
