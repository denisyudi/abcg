#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "abcg.hpp"
#include "input.hpp"

class OpenGLWindow;

class Player {
    public:
        void initializeGL(GLuint program);
        void paintGL(const Data &data);
        void terminateGL();
        void update(const Data &data, float deltaTime);
    
    private:
        friend OpenGLWindow;

        GLuint m_program{};
        GLint m_translationLoc{};
        GLint m_colorLoc{};
        GLint m_scaleLoc{};
    
        GLuint m_vao{};
        GLuint m_vbo{};
        GLuint m_ebo{};

        glm::vec4 m_color{1};
        float m_scale{1};
        glm::vec2 m_translation{glm::vec2(0)};
        glm::vec2 m_velocity{glm::vec2(0)};
};

#endif