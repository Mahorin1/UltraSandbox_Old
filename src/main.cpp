//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
	#include <globals.hpp>
	#include <state.hpp>
	#include <states/menu.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
	using namespace sf;
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
	int main() {
	//''''''''''''''''''//
		ContextSettings GL_Config;
		Event event; Image icon;
		StateManager State_Man;
		ResourceManager Res_Man;
		RenderWindow window;
		Clock DT_Timer;
	//''''''''''''''''''//
		GL_Config.antialiasingLevel = 16; icon.loadFromFile("res/gcc/icon.png");
		// Window Mode
		window.create({ WIN_W, WIN_H }, GameTitle, Style::Titlebar | Style::Close, GL_Config);
		window.setPosition(Vector2i(110, 35));
		// FullScreen Mode
		//window.create(VideoMode::getDesktopMode(), GameTitle, Style::Fullscreen | Style::Close, GL_Config);
		//SetWinDims((Vector2f)window.getSize());
		window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
		State_Man.run(StateManager::build<MenuState>(State_Man, Res_Man, window, true));
		window.setKeyRepeatEnabled(false);
		float DeltaTime = 0.f; window.setFramerateLimit(400);
	//''''''''''''''''''//
		while (State_Man.running()) {
			DeltaTime = DT_Timer.restart().asSeconds();
			DeltaTime = (DeltaTime > 0.1f) ? 0.1f : DeltaTime;
			State_Man.nextState();
			while (window.pollEvent(event))
			{ State_Man.handleEvents(event); }
			State_Man.update(DeltaTime);
			State_Man.draw();
		} return 0;
	//''''''''''''''''''//
	}
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//