//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
	#pragma once
    #include <iostream>
    #include <stack>
    #include <SFML/Graphics.hpp>
    #include <resource_manager.hpp>
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class StateManager;
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class State {
    //-------------//
    public:
        State(StateManager& state_man, ResourceManager& res_man , sf::RenderWindow& window, bool replace = true);
        virtual ~State() = default;
    //-------------//
        State(const State&) = delete;
        State& operator=(const State&) = delete;
    //-------------//
        virtual void pause() = 0;
        virtual void resume() = 0;
    //-------------//
        virtual void handleEvents(sf::Event& event) = 0;
        virtual void update(float dt) = 0;
        virtual void draw() = 0;
    //-------------//
        std::unique_ptr<State> next();
        [[nodiscard]] bool isReplacing() const;
    //-------------//
    protected:
        StateManager& m_state_man;
        ResourceManager& m_res_man;
        sf::RenderWindow& m_window;
        std::unique_ptr<State> m_next;
        bool m_replacing;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
    class StateManager {
    //-------------//
    public:
        StateManager();
        void run(std::unique_ptr<State> state);
    //-------------//
        void nextState();
        void lastState();
    //-------------//
        void handleEvents(sf::Event& event);
        void update(float dt);
        void draw();
    //-------------//
        [[nodiscard]] bool running() const;
        void quit();
    //-------------//
        template <typename T>
        static std::unique_ptr<T> build(StateManager& state_man, ResourceManager& res_man , sf::RenderWindow& window, bool replace = true);
    //-------------//
    private:
        std::stack<std::unique_ptr<State>> m_states;
        bool m_resume, m_running;
    //-------------//
    };
//~~~~~~~~~~~~~~~~~~~~~~~~//
    template <typename T>
    std::unique_ptr<T> StateManager::build(StateManager& state_man, ResourceManager& res_man , sf::RenderWindow& window, bool replace) {
    //-------------//
        auto new_state = std::unique_ptr<T>{ nullptr };
    //-------------//
        try {
            new_state = std::make_unique<T>(state_man, res_man, window, replace);
        }
        catch(std::runtime_error& exception) {
            std::cout << "Creation of new state was unsuccessful\n";
            std::cout << exception.what() << std::endl;
        } return new_state;
    //-------------//
    }
//~~~~~~~~~~~~~~~~~~~~~~~~//
//==============================================================//