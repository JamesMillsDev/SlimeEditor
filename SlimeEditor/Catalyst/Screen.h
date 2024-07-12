#pragma once

#include "Utilities/Color.h"

#include <string>

using std::string;

struct GLFWwindow;

namespace Catalyst
{
	namespace Utilities
	{
		class Config;
	}

	using Utilities::Config;
	using Utilities::Color;

	/* A utility class to manage the window of Raylib providing useful functionality like changing the screen size and title. */
	class Screen
	{
		friend class Application;

	public:
		Screen(const Screen&) = delete;
		Screen(Screen&&) = delete;

	public:
		/* Gets the current width of the screen. */
		const int& Width() const;
		/* Gets the current height of the screen. */
		const int& Height() const;
		/* Gets the current title of the window. */
		const string& Title();
		/* Gets the current clear colour of the screen. */
		const Color& ClearColor() const;

		/*
		* Sets the width of the screen to the new value.
		* @param _newWidth: The new width of the screen.
		*/
		void SetWidth(int _newWidth);
		/*
		* Sets the height of the screen to the new value.
		* @param _newHeight: The new height of the screen.
		*/
		void SetHeight(int _newHeight);
		/*
		* Sets the title of the window to the new value.
		* @param _newTitle: The new title of the window shown in the title bar.
		*/
		void SetTitle(const char* _newTitle);
		/*
		* Sets the clear color of the screen to the new value.
		* @param _newColor: The new background colour of the screen.
		*/
		void SetClearColor(Color _newColor);

		bool ShouldClose() const;

	public:
		Screen& operator=(const Screen&) = delete;
		Screen& operator=(Screen&&) = delete;

	private:
		int m_width;
		int m_height;
		string m_title;
		Color m_clearColor;
		bool m_fullscreen;
		bool m_shouldClose;

		GLFWwindow* m_window;

	private:
		Screen(Config* _config);
		~Screen();

	private:
		/* Opens a window and initialises the audio device. */
		bool Open();
		/* Closes down the window and the audio device. */
		void Close() const;

		/* Begins the drawing process and clears the screen background with the clear color. */
		bool NewFrame() const;
		/* Ends the drawing process. */
		void EndFrame();

	};
}
