#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include "abcg.hpp"
#include "player.hpp"
#include "ground.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
    protected:
        void handleEvent(SDL_Event& event) override;
        void initializeGL() override;
        void paintGL() override;
        void paintUI() override;
        void resizeGL(int width, int height) override;
        void terminateGL() override;

    private:
        GLuint m_objectsProgram{};

        int m_viewportWidth{};
        int m_viewportHeight{};

        Data m_data;

        Player m_player;
        Ground m_ground;

        void restart();
        void update();
};

#endif