#include "Screen.h"

#include <glew/glew.h>
#include <glfw/glfw3.h>

#include "Utilities/Config.h"

namespace Catalyst
{
	const int& Screen::Width() const
	{
		return m_width;
	}

	const int& Screen::Height() const
	{
		return m_height;
	}

	const string& Screen::Title()
	{
		return m_title;
	}

	const Color& Screen::ClearColor() const
	{
		return m_clearColor;
	}

	void Screen::SetWidth(int _newWidth)
	{
		m_width = _newWidth;
		glfwSetWindowSize(m_window, m_width, m_height);
	}

	void Screen::SetHeight(int _newHeight)
	{
		m_height = _newHeight;
		glfwSetWindowSize(m_window, m_width, m_height);
	}

	void Screen::SetTitle(const char* _newTitle)
	{
		m_title = _newTitle;

		glfwSetWindowTitle(m_window, m_title.c_str());
	}

	void Screen::SetClearColor(Color _newColor)
	{
		m_clearColor = _newColor;
	}

	bool Screen::ShouldClose() const
	{
		return m_shouldClose;
	}

	Screen::Screen(Config* _config)
		: m_shouldClose{ false }, m_window{ nullptr }
	{
		m_width = _config->GetInt("screen", "width");
		m_height = _config->GetInt("screen", "height");
		m_title = _config->GetString("application", "title");
		m_clearColor = _config->GetColor("screen", "clrCol");
		m_fullscreen = _config->GetBool("screen", "fullscreen");
	}

	Screen::~Screen()
	{
		m_width = 0;
		m_height = 0;

		m_title = "";
		m_clearColor = {  };

		m_window = nullptr;
	}

	bool Screen::Open()
	{
		if (glfwInit() == GL_FALSE)
			return false;

		m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), m_fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
		if (m_window == nullptr)
		{
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_window);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			glfwDestroyWindow(m_window);
			glfwTerminate();
			return false;
		}

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow*, const int _w, const int _h) { glViewport(0, 0, _w, _h); });

		glClearColor(m_clearColor.Red(), m_clearColor.Green(), m_clearColor.Blue(), m_clearColor.Alpha());

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return true;
	}

	void Screen::Close() const
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	bool Screen::NewFrame() const
	{
		glfwPollEvents();

		if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0)
			return false;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		return true;
	}

	void Screen::EndFrame()
	{
		//present back buffer to the monitor
		glfwSwapBuffers(m_window);

		// should the game exit?
		m_shouldClose = m_shouldClose || glfwWindowShouldClose(m_window) == GLFW_TRUE;
	}
}
