#include "GameTime.h"

namespace Catalyst::Utilities
{
	double GameTime::m_prevTime = 0;
	double GameTime::m_currTime = 0;
	double GameTime::m_deltaTime = 0;

	byte GameTime::m_fps = 0;
	byte GameTime::m_frames = 0;
	double GameTime::m_fpsInterval = 0;

	float GameTime::DeltaTime()
	{
		return static_cast<float>(m_deltaTime);
	}

	float GameTime::Time()
	{
		return static_cast<float>(m_currTime);
	}

	byte GameTime::Frames()
	{
		return m_frames;
	}

	byte GameTime::Fps()
	{
		return m_fps;
	}
}
