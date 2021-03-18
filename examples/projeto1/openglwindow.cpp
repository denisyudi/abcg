#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {
            m_data.m_input.set(static_cast<size_t>(Input::Left));
        }
        if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
            m_data.m_input.set(static_cast<size_t>(Input::Right));
        }
        if (event.key.keysym.sym == SDLK_SPACE) {
            m_data.m_input.set(static_cast<size_t>(Input::Grab));
        }
    }
    if (event.type == SDL_KEYUP) {
        if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) {
            m_data.m_input.reset(static_cast<size_t>(Input::Left));
        }
        if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
            m_data.m_input.reset(static_cast<size_t>(Input::Right));
        }
        if (event.key.keysym.sym == SDLK_SPACE) {
            m_data.m_input.reset(static_cast<size_t>(Input::Grab));
        }
    }
}

void OpenGLWindow::initializeGL() {
    m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                             getAssetsPath() + "objects.frag");
    glClearColor(0.5f, 0.7f, 1, 1);


    m_player.initializeGL(m_objectsProgram);
    m_ground.initializeGL(m_objectsProgram);
}


void OpenGLWindow::update() {
    float deltaTime{static_cast<float>(getDeltaTime())};

    m_player.update(m_data, deltaTime);
}

void OpenGLWindow::paintGL() {
    update();

    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, m_viewportWidth, m_viewportHeight);

    m_player.paintGL(m_data);
    m_ground.paintGL();
}

void OpenGLWindow::paintUI() {
    abcg::OpenGLWindow::paintUI();

    {
        auto size{ImVec2(0, 0)};
        auto position{ImVec2((m_viewportWidth - size.x) / 2.0f, (m_viewportHeight - size.y) / 2.0f)};
        ImGui::SetNextWindowPos(position);
        ImGui::SetNextWindowSize(size);
        ImGuiWindowFlags flags {ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs};
        ImGui::Begin(" ", nullptr, flags);
        ImGui::End();
    }
}

void OpenGLWindow::resizeGL(int width, int height) {
    m_viewportWidth = width;
    m_viewportHeight = height;

    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
    glDeleteProgram(m_objectsProgram);

    m_player.terminateGL();
    m_ground.terminateGL();
}