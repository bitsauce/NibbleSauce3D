#include <Sauce/Sauce.h>

using namespace sauce;

class GameControllerGame : public Game
{
public:
	Keybind m_keybindLeft;
	Keybind m_keybindRight;
	Keybind m_keybindUp;
	Keybind m_keybindDown;

	Vector2F m_playerPos;

	Resource<Font> m_font;
	SpriteBatch *m_spriteBatch;
	string m_btn;

	GameControllerGame() :
		Game("GameController", SAUCE_WINDOW_RESIZABLE)
	{
	}

	void onStart(GameEvent *e)
	{
		m_spriteBatch = new SpriteBatch;
		m_font = Resource<Font>("Arial");

		// TODO:
		// o Implement keybind modes (3rd parameter)
		// o Consider more closely how an input configuration file can be used with a controller
		// o Add controller support to InputContexts
		m_keybindLeft = Keybind(SAUCE_CONTROLLER_BUTTON_DPAD_LEFT, bind(&GameControllerGame::moveLeft, this, placeholders::_1), Keybind::TRIGGER_WHILE_PRESSED);
		m_keybindRight = Keybind(SAUCE_CONTROLLER_BUTTON_DPAD_RIGHT, bind(&GameControllerGame::moveRight, this, placeholders::_1), Keybind::TRIGGER_WHILE_PRESSED);
		m_keybindUp = Keybind(SAUCE_CONTROLLER_BUTTON_DPAD_UP, bind(&GameControllerGame::moveUp, this, placeholders::_1), Keybind::TRIGGER_WHILE_PRESSED);
		m_keybindDown = Keybind(SAUCE_CONTROLLER_BUTTON_DPAD_DOWN, bind(&GameControllerGame::moveDown, this, placeholders::_1), Keybind::TRIGGER_WHILE_PRESSED);

		InputManager *input = getInputManager();
		input->addKeybind(&m_keybindLeft);
		input->addKeybind(&m_keybindRight);
		input->addKeybind(&m_keybindUp);
		input->addKeybind(&m_keybindDown);

		Game::onStart(e);
	}

	void moveLeft(InputEvent *e) { m_playerPos.x -= 5; }
	void moveRight(InputEvent *e) { m_playerPos.x += 5; }
	void moveUp(InputEvent *e) { m_playerPos.y -= 5; }
	void moveDown(InputEvent *e) { m_playerPos.y += 5; }

	void onEnd(GameEvent *e)
	{
		LOG("onEnd");
		Game::onEnd(e);
	}

	void onTick(TickEvent *e)
	{
		Game::onTick(e);
	}

	void onControllerButton(ControllerButtonEvent *e)
	{
		if(e->getInputButton() == SAUCE_CONTROLLER_BUTTON_RIGHT_TRIGGER)
		{
			switch(e->getType())
			{
			case EVENT_CONTROLLER_BUTTON_DOWN: m_btn = "RT down"; break;
			case EVENT_CONTROLLER_BUTTON_UP: m_btn = "RT up"; break;
			case EVENT_CONTROLLER_BUTTON_REPEAT: m_btn = "RT repeat"; break;
			}
		}
	}

	void onControllerAxis(ControllerAxisEvent *e)
	{
		if(abs(e->getValue()) > 30000)
		{
			switch(e->getAxis())
			{
			case SAUCE_CONTROLLER_AXIS_LEFTX: m_btn = "LEFTX: " + util::intToStr(e->getValue()); break;
			case SAUCE_CONTROLLER_AXIS_LEFTY: m_btn = "LEFTY: " + util::intToStr(e->getValue()); break;
			}
		}
	}

	void onDraw(DrawEvent *e)
	{
		stringstream ss;
		ss << "Button A: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_A) ? "true" : "false") << endl;
		ss << "Button B: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_B) ? "true" : "false") << endl;
		ss << "Button X: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_X) ? "true" : "false") << endl;
		ss << "Button Y: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_Y) ? "true" : "false") << endl;
		ss << "Button BACK: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_BACK) ? "true" : "false") << endl;
		ss << "Button GUIDE: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_GUIDE) ? "true" : "false") << endl;
		ss << "Button START: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_START) ? "true" : "false") << endl;
		ss << "Button LEFT_STICK: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_LEFT_STICK) ? "true" : "false") << endl;
		ss << "Button RIGHT_STICK: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_RIGHT_STICK) ? "true" : "false") << endl;
		ss << "Button LEFT_SHOULDER: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_LEFT_SHOULDER) ? "true" : "false") << endl;
		ss << "Button RIGHT_SHOULDER: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_RIGHT_SHOULDER) ? "true" : "false") << endl;
		ss << "Button DPAD_UP: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_DPAD_UP) ? "true" : "false") << endl;
		ss << "Button DPAD_DOWN: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_DPAD_DOWN) ? "true" : "false") << endl;
		ss << "Button DPAD_LEFT: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_DPAD_LEFT) ? "true" : "false") << endl;
		ss << "Button DPAD_RIGHT: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_DPAD_RIGHT) ? "true" : "false") << endl;
		ss << "Button LEFT_TRIGGER: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_LEFT_TRIGGER) ? "true" : "false") << endl;
		ss << "Button RIGHT_TRIGGER: " << (getInputManager()->getButtonState(SAUCE_CONTROLLER_BUTTON_RIGHT_TRIGGER) ? "true" : "false") << endl;

		stringstream rss;
		rss << "Axis LEFTX:" << getInputManager()->getAxisValue(SAUCE_CONTROLLER_AXIS_LEFTX) << endl;
		rss << "Axis LEFTY:" << getInputManager()->getAxisValue(SAUCE_CONTROLLER_AXIS_LEFTY) << endl;
		rss << "Axis RIGHTX:" << getInputManager()->getAxisValue(SAUCE_CONTROLLER_AXIS_RIGHTX) << endl;
		rss << "Axis RIGHTY:" << getInputManager()->getAxisValue(SAUCE_CONTROLLER_AXIS_RIGHTY) << endl;
		rss << "Axis TRIGGER_LEFT:" << getInputManager()->getAxisValue(SAUCE_CONTROLLER_AXIS_TRIGGER_LEFT) << endl;
		rss << "Axis TRIGGER_RIGHT:" << getInputManager()->getAxisValue(SAUCE_CONTROLLER_AXIS_TRIGGER_RIGHT) << endl;

		m_spriteBatch->begin(e->getGraphicsContext());
		m_font->setColor(Color(255, 255, 255, 255));
		m_font->setHeight(16.0f);
		m_font->draw(m_spriteBatch, 10, 10, ss.str());
		m_font->draw(m_spriteBatch, e->getGraphicsContext()->getWidth() - 10, 10, rss.str(), FONT_ALIGN_RIGHT);
		m_font->draw(m_spriteBatch, e->getGraphicsContext()->getSize() / 2, m_btn);
		m_spriteBatch->end();

		float x = getInputManager()->getAxisValue(SAUCE_CONTROLLER_AXIS_RIGHTX);
		float y = getInputManager()->getAxisValue(SAUCE_CONTROLLER_AXIS_RIGHTY);
		e->getGraphicsContext()->drawRectangle(300 + 100 * x, 110 + 100 * y, 1, 1);

		Game::onDraw(e);
	}
};

/* Main entry point. This is where our program first starts executing. */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	/* To start our game we simply create our game object
	   and call run() */
	GameControllerGame game;
	return game.run();
}