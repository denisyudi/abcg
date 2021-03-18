#include "player.hpp"

#include <glm/gtx/fast_trigonometry.hpp>

void Player::initializeGL(GLuint program) {
    terminateGL();

    m_program = program;
    m_colorLoc = glGetUniformLocation(m_program, "color");
    m_scaleLoc = glGetUniformLocation(m_program, "scale");
    m_translationLoc = glGetUniformLocation(m_program, "translation");

    m_color = glm::vec4(0.75f, 0.1f, 0, 1);
    m_translation = glm::vec2(0, 0);
    m_velocity = glm::vec2(1.0f, 0);

    std::array<glm::vec2, 46> positions {
        //body
        glm::vec2{0, 5.0f},
        glm::vec2{-7.5f, 0},
        glm::vec2{0, -3.5f},
        glm::vec2{7.5f, 0},

        //claws
        glm::vec2{-8.5f, 7.5f},
        glm::vec2{-11.0f, 5.0f},
        glm::vec2{-9.0f, 4.5f},
        glm::vec2{-7.5f, 7.0f},
        glm::vec2{-7.0f, 6.0f},

        glm::vec2{5.0f, 8.5f},
        glm::vec2{10.0f, 7.5f},
        glm::vec2{6.0f, 6.0f},
        glm::vec2{3.5f, 8.0f},
        glm::vec2{3.5f, 6.0f},

        //legs
        glm::vec2{-8.5f, -3.0f},
        glm::vec2{-8.5f, -5.5f},
        glm::vec2{-10.0f, -7.5f},
        glm::vec2{-8.5f, -10.0f},

        glm::vec2{8.5f, -3.0f},
        glm::vec2{8.5f, -5.5f},
        glm::vec2{10.0f, -7.5f},
        glm::vec2{8.5f, -10.0f},

        glm::vec2{-2.5f, -2.5f},
        glm::vec2{-2.5f, -7.0f},
        glm::vec2{-2.5f, -5.0f},
        glm::vec2{-2.5f, -10.0f},
        glm::vec2{-4.0f, -5.5f},

        glm::vec2{2.5f, -2.5f},
        glm::vec2{2.5f, -7.0f},
        glm::vec2{2.5f, -5.0f},
        glm::vec2{2.5f, -10.0f},
        glm::vec2{4.0f, -5.5f},

        //eyes
        glm::vec2{-2.0f, 4.5f},
        glm::vec2{-2.0f, 2.5f},
        glm::vec2{-4.0f, 4.5f},
        glm::vec2{-4.0f, 2.5f},

        glm::vec2{2.0f, 4.5f},
        glm::vec2{2.0f, 2.5f},
        glm::vec2{4.0f, 4.5f},
        glm::vec2{4.0f, 2.5f},

        //move
        glm::vec2{15.0f, -9.5f},
        glm::vec2{15.0f, -8.5f},
        glm::vec2{8.5f, -9.5f},

        glm::vec2{-15.0f, -9.5f},
        glm::vec2{-15.0f, -8.5f},
        glm::vec2{-8.5f, -9.5f},
    };

    for (auto &position :positions) {
        position /= glm::vec2{45.0f, 30.0f};
    }

    std::array indices { 0, 1, 2,
                        0, 2, 3,
                        1, 4, 5,
                        6, 7, 8,
                        3, 9, 10,
                        11, 12, 13,
                        2, 14, 15,
                        14, 16, 17,
                        2, 18, 19,
                        18, 20, 21,
                        2, 22, 23,
                        24, 25, 26,
                        2, 27, 28,
                        29, 30, 31,
                        32, 33, 34,
                        33, 34, 35,
                        36, 37, 38,
                        37, 38, 39,

                        40, 41, 42,
                        43, 44, 45,

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

void Player::paintGL(const Data &data) {
    glUseProgram(m_program);

    glBindVertexArray(m_vao);

    if (data.m_input[static_cast<size_t>(Input::Right)] || data.m_input[static_cast<size_t>(Input::Left)]) {
        glUniform1f(m_scaleLoc, m_scale);
        glUniform2fv(m_translationLoc, 1, &m_translation.x);
        glUniform4f(m_colorLoc, 0.5, 0.5, 0.5, 0.5);

        glDrawElements(GL_TRIANGLES, 20 * 3, GL_UNSIGNED_INT, nullptr);
    }
    glUniform1f(m_scaleLoc, m_scale);
    glUniform2fv(m_translationLoc, 1, &m_translation.x);
    glUniform4fv(m_colorLoc, 1, &m_color.r);
    glDrawElements(GL_TRIANGLES, 18 * 3, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);

    glUseProgram(0);
}

void Player::terminateGL() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

void Player::update(const Data &data, float deltaTime) {
    if (data.m_input[static_cast<size_t>(Input::Left)] && m_translationLoc > -25.0f)
        m_translation.x -= m_velocity.x * deltaTime;
    if (data.m_input[static_cast<size_t>(Input::Right)] && m_translationLoc < 25.0f)
        m_translation.x += m_velocity.x * deltaTime;
}