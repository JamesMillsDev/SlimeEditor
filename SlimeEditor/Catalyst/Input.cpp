#include "Input.h"

#include <glfw/glfw3.h>

namespace Catalyst
{
	Input* Input::m_instance = nullptr;

	Input::Input()
		: m_oldMouseX{ 0 }, m_oldMouseY{ 0 }, m_firstMouseMove{ false },
		m_lastKeys{ new int[GLFW_KEY_LAST + 1] }, m_currentKeys{ new int[GLFW_KEY_LAST + 1] }
	{
		auto window = glfwGetCurrentContext();

		for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
			m_lastKeys[i] = m_currentKeys[i] = glfwGetKey(window, i);

		for (int i = 0; i < 8; ++i)
			m_lastButtons[i] = m_currentButtons[i] = glfwGetMouseButton(window, i);

		// set up callbacks
		auto keyPressCallback = [](GLFWwindow* _window, const int _key, const int _scancode, const int _action, const int _mods)
			{
				for (auto& f : Input::GetInstance()->m_keyCallbacks)
					f(_window, _key, _scancode, _action, _mods);
			};

		auto characterInputCallback = [](GLFWwindow* _window, const unsigned int _character)
			{
				GetInstance()->m_pressedCharacters.push_back(_character);

				for (auto& f : GetInstance()->m_charCallbacks)
					f(_window, _character);
			};

		auto mouseMoveCallback = [](GLFWwindow* _window, const double _x, const double _y)
			{
				int w = 0, h = 0;
				glfwGetWindowSize(_window, &w, &h);

				GetInstance()->OnMouseMove(static_cast<int>(_x), h - static_cast<int>(_y));

				for (auto& f : Input::GetInstance()->m_mouseMoveCallbacks)
					f(_window, _x, h - _y);
			};

		auto mouseInputCallback = [](GLFWwindow* _window, const int _button, const int _action, const int _mods)
			{
				for (auto& f : GetInstance()->m_mouseButtonCallbacks)
					f(_window, _button, _action, _mods);
			};

		auto mouseScrollCallback = [](GLFWwindow* _window, const double _xOffset, const double _yOffset)
			{
				GetInstance()->m_mouseScroll += _yOffset;

				for (auto& f : GetInstance()->m_mouseScrollCallbacks)
					f(_window, _xOffset, _yOffset);
			};

		auto mouseEnterCallback = [](GLFWwindow*, int)
			{
				// Set flag to prevent large mouse delta on entering screen
				GetInstance()->m_firstMouseMove = true;
			};

		glfwSetKeyCallback(window, keyPressCallback);
		glfwSetCharCallback(window, characterInputCallback);
		glfwSetMouseButtonCallback(window, mouseInputCallback);
		glfwSetCursorPosCallback(window, mouseMoveCallback);
		glfwSetScrollCallback(window, mouseScrollCallback);
		glfwSetCursorEnterCallback(window, mouseEnterCallback);

		m_mouseX = 0;
		m_mouseY = 0;
		m_mouseScroll = 0;
	}

	Input::~Input()
	{
		delete[] m_lastKeys;
		delete[] m_currentKeys;
	}

	void Input::OnMouseMove(const int _newXPos, const int _newYPos)
	{
		m_mouseX = _newXPos;
		m_mouseY = _newYPos;
		if (m_firstMouseMove)
		{
			// On first move after Startup/entering window reset old mouse position
			m_oldMouseX = _newXPos;
			m_oldMouseY = _newYPos;
			m_firstMouseMove = false;
		}
	}

	void Input::Create()
	{
		m_instance = new Input();
	}

	void Input::Destroy()
	{
		delete m_instance;
	}

	void Input::ClearStatus()
	{
		m_pressedCharacters.clear();

		auto window = glfwGetCurrentContext();

		m_pressedKeys.clear();

		// Update keys
		for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
		{
			m_lastKeys[i] = m_currentKeys[i];

			if ((m_currentKeys[i] = glfwGetKey(window, i)) == GLFW_PRESS)
				m_pressedKeys.push_back(m_currentKeys[i]);
		}

		// Update mouse
		for (int i = 0; i < 8; ++i)
		{
			m_lastButtons[i] = m_currentButtons[i];
			m_currentButtons[i] = glfwGetMouseButton(window, i);
		}

		// Update old mouse position
		m_oldMouseX = m_mouseX;
		m_oldMouseY = m_mouseY;
	}

	Input* Input::GetInstance()
	{
		return m_instance;
	}

	bool Input::IsKeyDown(const int _inputKeyId) const
	{
		return m_currentKeys[_inputKeyId] == GLFW_PRESS;
	}

	bool Input::IsKeyUp(const int _inputKeyId) const
	{
		return m_currentKeys[_inputKeyId] == GLFW_RELEASE;
	}

	bool Input::WasKeyPressed(const int _inputKeyId) const
	{
		return m_currentKeys[_inputKeyId] == GLFW_PRESS && m_lastKeys[_inputKeyId] == GLFW_RELEASE;
	}

	bool Input::WasKeyReleased(const int _inputKeyId) const
	{
		return m_currentKeys[_inputKeyId] == GLFW_RELEASE && m_lastKeys[_inputKeyId] == GLFW_PRESS;
	}

	const std::vector<int>& Input::GetPressedKeys() const
	{
		return m_pressedKeys;
	}

	const std::vector<unsigned int>& Input::GetPressedCharacters() const
	{
		return m_pressedCharacters;
	}

	bool Input::IsMouseButtonDown(const int _inputMouseId) const
	{
		return m_currentButtons[_inputMouseId] == GLFW_PRESS;
	}

	bool Input::IsMouseButtonUp(const int _inputMouseId) const
	{
		return m_currentButtons[_inputMouseId] == GLFW_RELEASE;
	}

	bool Input::WasMouseButtonPressed(const int _inputMouseId) const
	{
		return m_currentButtons[_inputMouseId] == GLFW_PRESS &&
			m_lastButtons[_inputMouseId] == GLFW_RELEASE;
	}

	bool Input::WasMouseButtonReleased(const int _inputMouseId) const
	{
		return m_currentButtons[_inputMouseId] == GLFW_RELEASE &&
			m_lastButtons[_inputMouseId] == GLFW_PRESS;
	}

	int Input::GetMouseX() const
	{
		return m_mouseX;
	}

	int Input::GetMouseY() const
	{
		return m_mouseY;
	}

	double Input::GetMouseScroll() const
	{
		return m_mouseScroll;
	}

	void Input::GetMouseXY(int* _x, int* _y) const
	{
		if (_x != nullptr)
			*_x = m_mouseX;

		if (_y != nullptr)
			*_y = m_mouseY;
	}

	int Input::GetMouseDeltaX() const
	{
		return m_mouseX - m_oldMouseX;
	}

	int Input::GetMouseDeltaY() const
	{
		return m_mouseY - m_oldMouseY;
	}

	void Input::GetMouseDelta(int* _x, int* _y) const
	{
		if (_x != nullptr) *_x = m_mouseX - m_oldMouseX;
		if (_y != nullptr) *_y = m_mouseY - m_oldMouseY;
	}
}
