#pragma once

#include "Catalyst/Application.h"
#include "Catalyst/Types.h"

namespace Catalyst::Utilities
{
	class GameTime
	{
	public:
		static float DeltaTime();
		static float Time();
		static byte Frames();
		static byte Fps();

	private:
		static friend void Application::InitTime();
		static friend void Application::PollTime();

	private:
		static double m_prevTime;
		static double m_currTime;
		static double m_deltaTime;

		static byte m_fps;
		static byte m_frames;

		static double m_fpsInterval;

	};
}
