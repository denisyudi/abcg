#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <random>

#include "abcg.hpp"
#include "model.hpp"
#include "camera.hpp"

class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& ev) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;

 private:
  static const int m_numSnowflake{1000};

  GLuint m_program{};

  int m_viewportWidth{};
  int m_viewportHeight{};

  Camera m_camera;
  float m_dollySpeed{0.0f};
  float m_truckSpeed{0.0f};
  float m_panSpeed{0.0f};

  std::default_random_engine m_randomEngine;

  Model m_model;
  Model m_terrain;
  Model m_snowman;
  Model m_tree;
  Model m_wood;

  int m_mappingMode{};

  std::array<glm::vec3, m_numSnowflake> m_snowflakePositions;
  std::array<glm::vec3, m_numSnowflake> m_snowflakeRotations;
  float m_angle{};

  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  float m_FOV{30.0f};

  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};

  glm::vec4 m_Ka;
  glm::vec4 m_Kd;
  glm::vec4 m_Ks;
  float m_shininess{};

  glm::vec4 m_KaWood;
  glm::vec4 m_KdWood;
  glm::vec4 m_KsWood;
  float m_shininessWood{};
 
  glm::vec4 m_KaSnowman;
  glm::vec4 m_KdSnowman;
  glm::vec4 m_KsSnowman;
  float m_shininessSnowman{};

  void randomizeSnowflake(glm::vec3 &position, glm::vec3 &rotation);
  void update();
};

#endif