#include "ground.hpp"

#include <glm/gtx/fast_trigonometry.hpp>

void Ground::initializeGL(GLuint program) {
    terminateGL();

    m_program = program;
    m_colorLoc = glGetUniformLocation(m_program, "color");
    m_scaleLoc = glGetUniformLocation(m_program, "scale");
    m_translationLoc = glGetUniformLocation(m_program, "translation");

    m_color = glm::vec4(0.8784, 0.8784, 0.5804, 1);
    m_translation = glm::vec2(0, -0.365);

    std::array<glm::vec2, 4> positions {
        glm::vec2{-60.0f, 2.5f},
        glm::vec2{60.0f, 2.5f},
        glm::vec2{-60.0f, -40.0f},
        glm::vec2{60.0f, -40.0f},

    };

    for (auto &position :positions) {
        position /= glm::vec2{15.0f, 15.0f};
    }

    std::array indices { 0, 1, 2,
                         1, 2, 3
    };

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLint positionAttribute{glGetAttribLocation(m_program, "inPosition")};

    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);
    
    glEnableVertexAttribArray(positionAttribute);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

 
    glBindVertexArray(0);
}

void Ground::paintGL() {
    glUseProgram(m_program);

    glBindVertexArray(m_vao);

    glUniform1f(m_scaleLoc, m_scale);
    glUniform2fv(m_translationLoc, 1, &m_translation.x);
    glUniform4fv(m_colorLoc, 1, &m_color.r);
    glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);

    glUseProgram(0);
}

void Ground::terminateGL() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}