#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcg.hpp"

class OpenGLWindow;

class Ground {
    public:
        void initializeGL(GLuint program);
        void paintGL();
        void terminateGL();
    
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
        float m_scale{0.25f};
        glm::vec2 m_translation{glm::vec2(0)};
};

#endif