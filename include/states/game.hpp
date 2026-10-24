//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
	#pragma once
	#include <state.hpp>
	#include <ragdoll.hpp>
    #include <entity.hpp>
	#include <camera.hpp>
	#include <gui.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
	class SideToolUI final : public Panel {
	public:
	//-------------//
		SideToolUI(Every& e, ResourceManager& rm);
		~SideToolUI();
	//-------------//
		void draw(sf::RenderWindow& w);
		void update(sf::Vector2f mpos);
		void click(bool b);
		int getMode();
	//-------------//
	private:
	//-------------//
		int m_mode;
		int m_nbBtn;
		bool m_showBtn;
		Button* m_btn[10];
		Button* m_lastPressed;
		float m_posX1, m_posX2;
		Tweener<float> m_tween;
		sf::Clock m_sliderTimer;
		Tweener<float> m_btnTweens[10];
		sf::SoundBuffer* m_bufferPtr[3];
	//-------------//
	};
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
	class GameState final : public State {
	public:
	//-------------//
		GameState(StateManager& state_man, ResourceManager& res_man , sf::RenderWindow& window, bool replace = true);
		~GameState();
	//-------------//
		void pause() override;
		void resume() override;
	//-------------//
		void handleEvents(sf::Event& event) override;
		void update(float dt) override;
		void reset(bool anim = false);
		void unload(); void draw() override;
	//-------------//
		void addBarrel(sf::Vector2f pos, float size = Rdm(0.65f, 0.8f));
		void addBox(sf::Vector2f pos, float size = Rdm(0.5f, 1.5f));
		void addSaw(sf::Vector2f pos, float size = Rdm(0.65f, 0.8f));
	//-------------//
	private:
	//-------------//
		sf::Text m_creditText;
		sf::Text m_runtimeText;
		sf::Clock m_runtimeClock;
		sf::Color m_background_color;
	//-------------//
		sf::RectangleShape m_blackScreen;
		Tweener<float> m_blackScreenTween;
		sf::Color m_blackScreenCol;
	//-------------//
		ShapedCollider* m_ground;
		sf::FloatRect m_queryRect;
		UserQuery_CB m_queryCB;
        DragJoint m_dragJoint;
		float m_mouseAngleSpeed;
		Ragdoll* m_player;
		ShapedCollider* m_paddle;
		ShapedCollider* m_weight;
		std::vector<SpawnEntity*> m_entityLst;
		std::vector<Ragdoll*> m_ragdollLst;
		EntityName m_lastEntity;
	//-------------//
		Robot* m_robot;
		sf::Shader TestShader;
		sf::Shader TestShader2;
		std::array<sf::Glsl::Vec4, 4> ShaderColors;
		sf::RectangleShape TestBox;
		sf::CircleShape TestCirc;
		sf::Clock ShaderTimer;
	//-------------//
		b2DistanceJoint* J[2];
		sw::Line m_line[2];
		SideToolUI* m_sideToolUI;
	//-------------//
		Camera m_camera;
		Tweener<float> m_camZoomTween;
		Tweener<float> m_slowMotionTween;
		float m_gameSpeed;
		Every m_gameEvery;
		Every m_entityEvery;
	//-------------//
		sf::Sound m_popSound;
		sf::Sound m_bubbleSound;
		sf::Sprite m_viseur;
	//-------------//
	};
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//