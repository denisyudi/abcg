#include "openglwindow.hpp"

#include <imgui.h>

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

#include <glm/gtc/matrix_inverse.hpp>

void OpenGLWindow::handleEvent(SDL_Event& ev) {
  if (ev.type == SDL_KEYDOWN) {
    if (ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (ev.key.keysym.sym == SDLK_q) m_truckSpeed = -1.0f;
    if (ev.key.keysym.sym == SDLK_e) m_truckSpeed = 1.0f;
  }
  if (ev.type == SDL_KEYUP) {
    if ((ev.key.keysym.sym == SDLK_UP || ev.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_DOWN || ev.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_LEFT || ev.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((ev.key.keysym.sym == SDLK_RIGHT || ev.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_q && m_truckSpeed < 0) m_truckSpeed = 0.0f;
    if (ev.key.keysym.sym == SDLK_e && m_truckSpeed > 0) m_truckSpeed = 0.0f;
  }
}

void OpenGLWindow::initializeGL() {
  glClearColor(0, 0, 0, 1);

  glEnable(GL_DEPTH_TEST);

  m_program = createProgramFromFile(getAssetsPath() + "shaders/texture.vert",
                                    getAssetsPath() + "shaders/texture.frag");

  m_model.loadFromFile(getAssetsPath() + "snowflake.obj");
  m_model.setupVAO(m_program);

  m_terrain.loadDiffuseTexture(getAssetsPath() + "maps/686.jpg");
  m_terrain.loadFromFile(getAssetsPath() + "snowterrain.obj");
  m_terrain.setupVAO(m_program);
  m_Ka = m_terrain.getKa();
  m_Kd = m_terrain.getKd();
  m_Ks = m_terrain.getKs();
  m_shininess = m_terrain.getShininess();

  m_snowman.loadDiffuseTexture(getAssetsPath() + "maps/snowman1_diffuse.png");
  m_snowman.loadFromFile(getAssetsPath() + "snowman_finish.obj");
  m_snowman.setupVAO(m_program);
  m_KaSnowman = m_snowman.getKa();
  m_KdSnowman = m_snowman.getKd();
  m_KsSnowman = m_snowman.getKs();
  m_shininessSnowman = m_snowman.getShininess();

  m_tree.loadFromFile(getAssetsPath() + "DeadTree.obj");
  m_tree.setupVAO(m_program);

  m_wood.loadDiffuseTexture(getAssetsPath() + "maps/WoodTexture.png");
  m_wood.loadFromFile(getAssetsPath() + "Wood.obj");
  m_KaWood = m_wood.getKa();
  m_KdWood = m_wood.getKd();
  m_KsWood = m_wood.getKs();
  m_wood.setupVAO(m_program);

  for (const auto index : iter::range(m_numSnowflake)) {
    auto &position{m_snowflakePositions.at(index)};
    auto &rotation{m_snowflakeRotations.at(index)};

    randomizeSnowflake(position, rotation);
  }

  resizeGL(getWindowSettings().width, getWindowSettings().height);
}

void OpenGLWindow::randomizeSnowflake(glm::vec3 &position, glm::vec3 &rotation) {
  std::uniform_real_distribution<float> distPosXY(-30.0f, 20.0f);
  std::uniform_real_distribution<float> distPosZ(-100.0f, 20.0f);

  position = glm::vec3(distPosXY(m_randomEngine), distPosXY(m_randomEngine),
                       distPosZ(m_randomEngine));

  std::uniform_real_distribution<float> distRotAxis(-1.0f, 1.0f);

  rotation = glm::normalize(glm::vec3(distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine),
                                      distRotAxis(m_randomEngine)));
}

void OpenGLWindow::paintGL() {
  update();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, m_viewportWidth, m_viewportHeight);

  glUseProgram(m_program);

  GLint viewMatrixLoc{glGetUniformLocation(m_program, "viewMatrix")};
  GLint projMatrixLoc{glGetUniformLocation(m_program, "projMatrix")};
  GLint modelMatrixLoc{glGetUniformLocation(m_program, "modelMatrix")};
  GLint colorLoc{glGetUniformLocation(m_program, "color")};
  GLint normalMatrixLoc{glGetUniformLocation(m_program, "normalMatrix")};
  GLint lightDirLoc{glGetUniformLocation(m_program, "lightDirWorldSpace")};
  GLint shininessLoc{glGetUniformLocation(m_program, "shininess")};
  GLint IaLoc{glGetUniformLocation(m_program, "Ia")};
  GLint IdLoc{glGetUniformLocation(m_program, "Id")};
  GLint IsLoc{glGetUniformLocation(m_program, "Is")};
  GLint KaLoc{glGetUniformLocation(m_program, "Ka")};
  GLint KdLoc{glGetUniformLocation(m_program, "Kd")};
  GLint KsLoc{glGetUniformLocation(m_program, "Ks")};
  GLint diffuseTexLoc{glGetUniformLocation(m_program, "diffuseTex")};
  GLint mappingModeLoc{glGetUniformLocation(m_program, "mappingMode")};

  glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_camera.m_viewMatrix[0][0]);
  glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_camera.m_projMatrix[0][0]);
  glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
  glUniform1i(diffuseTexLoc, 0);
  glUniform1i(mappingModeLoc, m_mappingMode);

  auto lightDirRotated{m_camera.m_projMatrix * m_lightDir};
  glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  glUniform4fv(IaLoc, 1, &m_Ia.x);
  glUniform4fv(IdLoc, 1, &m_Id.x);
  glUniform4fv(IsLoc, 1, &m_Is.x);

  auto modelViewMatrix{glm::mat3(m_viewMatrix * m_modelMatrix)};
  glm::mat3 normalMatrix{glm::inverseTranspose(modelViewMatrix)};
  glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, &normalMatrix[0][0]);

  for (const auto index : iter::range(m_numSnowflake)) {
    auto &position{m_snowflakePositions.at(index)};
    auto &rotation{m_snowflakeRotations.at(index)};

    glm::mat4 modelMatrix{1.0f};
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.7f));
    modelMatrix = glm::rotate(modelMatrix, m_angle, rotation);

    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &modelMatrix[0][0]);

    m_model.render();
  }

  glm::mat4 tree{1.0f};
  tree = glm::translate(tree, glm::vec3(5.0f, 1.0f, 0.0f));
  tree = glm::rotate(tree, glm::radians(45.0f), glm::vec3(0, 1, 0));
  tree = glm::scale(tree, glm::vec3(4.0f));
  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &tree[0][0]);
  glUniform4f(colorLoc, 0, 0.34f, 0.67f, 0.60f);
  m_tree.render();

  glm::mat4 terrain{1.0f};
  terrain = glm::translate(terrain, glm::vec3(0.0f, 5.0f, 0.0f));
  terrain = glm::rotate(terrain, glm::radians(90.0f), glm::vec3(0, 1, 0));
  terrain = glm::scale(terrain, glm::vec3(20.0f));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &terrain[0][0]);

  glUniform1f(shininessLoc, m_shininess);
  glUniform4fv(KaLoc, 1, &m_Ka.x);
  glUniform4fv(KdLoc, 1, &m_Kd.x);
  glUniform4fv(KsLoc, 1, &m_Ks.x);
  m_terrain.render();

  glm::mat4 snowman{1.0f};
  snowman = glm::translate(snowman, glm::vec3(2.0f, -0.2f, 0.0f));
  snowman = glm::rotate(snowman, glm::radians(90.0f), glm::vec3(0, 1, 0));
  snowman = glm::scale(snowman, glm::vec3(1.0f));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &snowman[0][0]);
  glUniform1f(shininessLoc, m_shininessSnowman);
  glUniform4fv(KaLoc, 1, &m_KaSnowman.x);
  glUniform4fv(KdLoc, 1, &m_KdSnowman.x);
  glUniform4fv(KsLoc, 1, &m_KsSnowman.x);
  m_snowman.render();

  glm::mat4 wood{1.0f};
  wood = glm::translate(wood, glm::vec3(-1.0f, -1.0f, 0.0f));
  wood = glm::rotate(wood, glm::radians(90.0f), glm::vec3(0, 1, 0));
  wood = glm::scale(wood, glm::vec3(5.0f));

  glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &wood[0][0]);
  glUniform1f(shininessLoc, m_shininessWood);
  glUniform4fv(KaLoc, 1, &m_KaWood.x);
  glUniform4fv(KdLoc, 1, &m_KdWood.x);
  glUniform4fv(KsLoc, 1, &m_KsWood.x);
  m_wood.render();

  glUseProgram(0);
}

void OpenGLWindow::paintUI() {
  abcg::OpenGLWindow::paintUI();
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  m_camera.computeProjectionMatrix(width, height);
}

void OpenGLWindow::terminateGL() { glDeleteProgram(m_program); }

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};
  m_angle = glm::wrapAngle(m_angle + glm::radians(90.0f) * deltaTime);

  for (const auto index : iter::range(m_numSnowflake)) {
    auto &position{m_snowflakePositions.at(index)};
    auto &rotation{m_snowflakeRotations.at(index)};

    position.y -= deltaTime * 7.5f;

    if (position.y < -0.5f) {
      randomizeSnowflake(position, rotation);
      position.y = 20.0f;
    }
  }

  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}