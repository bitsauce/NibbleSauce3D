#ifndef SAUCE_EVENT_H
#define SAUCE_EVENT_H

#include <Sauce/Config.h>

BEGIN_SAUCE_NAMESPACE

enum EventType
{
	EVENT_START,
	EVENT_END,

	EVENT_TICK,
	EVENT_DRAW,
	EVENT_STEP_BEGIN,
	EVENT_STEP_END,
	EVENT_WINDOW_SIZE_CHANGED,
	EVENT_TEXT_INPUT,

	EVENT_KEY_DOWN,
	EVENT_KEY_UP,
	EVENT_KEY_REPEAT,

	EVENT_MOUSE_UP,
	EVENT_MOUSE_DOWN,
	EVENT_MOUSE_MOVE,
	EVENT_MOUSE_WHEEL,

	EVENT_CONTROLLER_BUTTON_DOWN,
	EVENT_CONTROLLER_BUTTON_UP,
	EVENT_CONTROLLER_BUTTON_REPEAT,
	EVENT_CONTROLLER_AXIS,

	EVENT_CUSTOM
};

class SAUCE_API Event
{
protected:
	Event(const uint type) :
		m_type(type),
		m_userData(0)
	{
	}

public:
	uint getType() const
	{
		return m_type;
	}

	void *getUserData() const
	{
		return m_userData;
	}

	void setUserData(void *data)
	{
		m_userData = data;
	}

private:
	const uint m_type;
	void *m_userData;
};

class SAUCE_API TextEvent : public Event
{
public:
	TextEvent(const char c) :
		Event(EVENT_TEXT_INPUT),
		m_char(c)
	{
	}

	char getChar() const
	{
		return m_char;
	}

private:
	const char m_char;
};

class GraphicsContext;
class SpriteBatch;

class SAUCE_API DrawEvent : public Event
{
public:
	DrawEvent(const float alpha, GraphicsContext *graphicsContext) :
		Event(EVENT_DRAW),
		m_alpha(alpha),
		m_graphicsContext(graphicsContext)
	{
	}

	float getAlpha() const
	{
		return m_alpha;
	}

	GraphicsContext *getGraphicsContext() const
	{
		return m_graphicsContext;
	}

private:
	const float m_alpha;
	GraphicsContext *m_graphicsContext;
};

class SAUCE_API TickEvent : public Event
{
public:
	TickEvent(const float delta) :
		Event(EVENT_TICK),
		m_delta(delta)
	{
	}

	float getDelta() const
	{
		return m_delta;
	}

private:
	const float m_delta;
};

class Window;

class SAUCE_API WindowEvent : public Event
{
public:
	enum WindowEventType
	{
		SIZE_CHANGED = EVENT_WINDOW_SIZE_CHANGED
	};

	WindowEvent(const WindowEventType type, Window *window, const Sint32 width, const Sint32 height) :
		Event(type),
		m_window(window),
		m_width(width),
		m_height(height)
	{
	}

	Window *getWindow() const
	{
		return m_window;
	}

	Sint32 getWidth() const
	{
		return m_width;
	}

	Sint32 getHeight() const
	{
		return m_height;
	}

private:
	Window *m_window;
	Sint32 m_width, m_height;
};

class SAUCE_API GameEvent : public Event
{
public:
	enum GameEventType
	{
		START = EVENT_START,
		END = EVENT_END
	};

	GameEvent(const GameEventType type) :
		Event(type)
	{
	}
};

class SAUCE_API StepEvent : public Event
{
public:
	enum StepEventType
	{
		BEGIN = EVENT_STEP_BEGIN,
		END = EVENT_STEP_END
	};

	StepEvent(const StepEventType type) :
		Event(type)
	{
	}
};

class SAUCE_API InputEvent : public Event
{
protected:
	InputEvent(const uint type, InputManager *inputManager, const InputButton button) :
		Event(type),
		m_inputManager(inputManager),
		m_inputButton(button)
	{
	}

	InputEvent(const uint type, InputManager *inputManager) :
		Event(type),
		m_inputManager(inputManager),
		m_inputButton()
	{
	}

public:
	InputManager *getInputManager() const
	{
		return m_inputManager;
	}

	InputButton getInputButton() const
	{
		return m_inputButton;
	}

protected:
	InputManager * const m_inputManager;
	const InputButton m_inputButton;
};

class SAUCE_API KeyEvent : public InputEvent
{
public:
	enum Modifier
	{
		NONE = KMOD_NONE,
		NUMLOCK = KMOD_NUM,
		CAPSLOCK = KMOD_CAPS,
		LCONTROL = KMOD_LCTRL,
		RCONTROL = KMOD_RCTRL,
		RSHIFT = KMOD_RSHIFT,
		LSHIFT = KMOD_LSHIFT,
		RALT = KMOD_RALT,
		LALT = KMOD_LALT,
		CTRL = KMOD_CTRL,
		SHIFT = KMOD_SHIFT,
		ALT = KMOD_ALT
	};

	enum KeyEventType
	{
		DOWN = EVENT_KEY_DOWN,
		UP = EVENT_KEY_UP,
		REPEAT = EVENT_KEY_REPEAT
	};

	KeyEvent(const KeyEventType type, InputManager *inputManager, const InputButton inputButton, const Uint16 modifiers) :
		InputEvent(type, inputManager, inputButton),
		m_modifiers(modifiers)
	{
	}

	Keycode getKeycode() const;

	Scancode getScancode() const
	{
		return (Scancode) m_inputButton.getCode();
	}

	Uint16 getModifiers() const
	{
		return m_modifiers;
	}

private:
	const Uint16 m_modifiers;
};

class SAUCE_API MouseEvent : public InputEvent
{
public:
	enum MouseEventType
	{
		MOVE = EVENT_MOUSE_MOVE,
		DOWN = EVENT_MOUSE_DOWN,
		UP = EVENT_MOUSE_UP,
		WHEEL = EVENT_MOUSE_WHEEL
	};

	MouseEvent(const MouseEventType type, InputManager *inputManager, const Sint32 x, const Sint32 y, const MouseButton button, const Sint32 wheelX, const Sint32 wheelY) :
		InputEvent(type, inputManager, button),
		m_x(x),
		m_y(y),
		m_wheelX(wheelX),
		m_wheelY(wheelY)
	{
	}

	MouseButton getButton() const
	{
		return (MouseButton) getInputButton().getCode();
	}

	Sint32 getX() const
	{
		return m_x;
	}

	Sint32 getY() const
	{
		return m_y;
	}

	Vector2<Sint32> getPosition() const
	{
		return Vector2<Sint32>(m_x, m_y);
	}

	Sint32 getWheelX() const
	{
		return m_wheelX;
	}

	Sint32 getWheelY() const
	{
		return m_wheelY;
	}

private:
	const Sint32 m_x, m_y;
	const Sint32 m_wheelX, m_wheelY;
};

class SAUCE_API ControllerButtonEvent : public InputEvent
{
public:
	enum ControllerButtonEventType
	{
		DOWN = EVENT_CONTROLLER_BUTTON_DOWN,
		UP = EVENT_CONTROLLER_BUTTON_UP,
		REPEAT = EVENT_CONTROLLER_BUTTON_REPEAT
	};

	ControllerButtonEvent(const ControllerButtonEventType type, InputManager *inputManager, const ControllerButton controllerButton) :
		InputEvent(type, inputManager, controllerButton)
	{
	}
};

class SAUCE_API ControllerAxisEvent : public InputEvent
{
public:
	ControllerAxisEvent(InputManager *inputManager, const ControllerAxis axis, const short value) :
		InputEvent(EVENT_CONTROLLER_AXIS, inputManager),
		m_axis(axis),
		m_value(value)
	{
	}

	ControllerAxis getAxis() const { return m_axis; }
	short getValue() const { return m_value; }

private:
	const ControllerAxis m_axis;
	const short m_value;
};

END_SAUCE_NAMESPACE

#endif // SAUCE_EVENT_H
