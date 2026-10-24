//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
	#pragma once
	#include <state.hpp>
	#include <utils.hpp>
	#include <gui.hpp>
	#include <audio.hpp>
	#include <tweeny/tweeny.h>
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
	class MenuState final : public State {
	public:
	//-------------//
		MenuState(StateManager& state_man, ResourceManager& res_man , sf::RenderWindow& window, bool replace = true);
		~MenuState();
	//-------------//
		void pause() override;
		void resume() override;
	//-------------//
		void handleEvents(sf::Event& event) override;
		void update(float dt) override;
		void draw() override;
		void unload();
	//-------------//
	private:
	//-------------//
		sf::Text m_text;
		sf::Color m_background_color;
		sf::Music music;
	//-------------//
		sf::RectangleShape m_blackScreen;
		Tweener<float> m_blackScreenTween;
		sf::Color m_blackScreenCol;
		bool m_canUseUI;
	//-------------//
		sf::SoundBuffer* m_bufferPtr1;
		sf::SoundBuffer* m_bufferPtr2;
		sf::SoundBuffer* m_bufferPtr3;
	//-------------//
		Tweener<float> m_titleTween;
		Every m_tweenHandler;
		bool m_titleAnimPhase;
		float m_titlezoom_min;
		float m_titlezoom_max;
		Tweener<float> m_playButtonTween;
		Tweener<float> m_optionButtonTween;
		Tweener<float> m_quitButtonTween;
		Button* m_playButton;
		Button* m_optionButton;
		Button* m_quitButton;
	//-------------//
	};
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//