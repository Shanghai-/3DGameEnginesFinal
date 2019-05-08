#include "particlesys.h"

#include "Particles/ResourceLoader2.h"
//#include "cs123_lib/errorchecker.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <iostream>
//#include "settings.h"

#include "Particles/OpenGLShape.h"
#include "engine/graphics/Texture2D.h"
#include "engine/graphics/ShaderAttribLocations.h"
//#include "sphere.h"

ParticleSys::ParticleSys(int priority)
    : System(priority),
      m_phongProgram(0), m_textureProgram(0), m_horizontalBlurProgram(0), m_verticalBlurProgram(0),
      m_quad(nullptr), m_sphere(nullptr),
      m_blurFBO1(nullptr), m_blurFBO2(nullptr),
      m_particlesFBO1(nullptr), m_particlesFBO2(nullptr),
      m_firstPass(true), m_evenPass(true),
      m_numParticles(10),
      m_angleX(-0.5f), m_angleY(0.5f), m_zoom(4.f)
{

    m_screenWidth = 800;
    m_screenHeight = 600;



    ResourceLoader2::initializeGlew();
    glEnable(GL_DEPTH_TEST);

    // Set the color to set the screen when the color buffer is cleared.
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Create shader programs.
    m_particleUpdateProgram = ResourceLoader2::createShaderProgram(
                ":/shaders/quad.vert", ":/shaders/particles_update.frag");
    m_particleDrawProgram = ResourceLoader2::createShaderProgram(
                ":/shaders/particles_draw.vert", ":/shaders/particles_draw.frag");

    // TODO: [Task 6] Fill in the positions and UV coordinates to draw a fullscreen quad
    // We've already set the vertex attributes for you, so be sure to follow those specifications
    // (triangle strip, 4 vertices, position followed by UVs)
    std::vector<GLfloat> quadData = {-1.f, 1.f, 0.f, 0.f, 0.f,
                                     -1.f, -1.f, 0.f, 0.f, 1.f,
                                     1.f, 1.f, 0.f, 1.f, 0.f,
                                     1.f, -1.f, 0.f, 1.f, 1.f};
    m_quad = std::make_shared<OpenGLShape>();
    m_quad->setVertexData(&quadData[0], quadData.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, 4);
    m_quad->setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->setAttribute(ShaderAttrib::TEXCOORD0, 2, 3*sizeof(GLfloat), VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_quad->buildVAO();

    // We will use this VAO to draw our particles' triangles
    // It doesn't need any data associated with it, so we don't have to make a full VAO instance
    glGenVertexArrays(1, &m_particlesVAO);
    // TODO [Task 12] Create m_particlesFBO1 and 2 with std::make_shared
    m_particlesFBO1 = std::make_shared<FBO>(m_numParticles, 1, 2, 4, FBO::DEPTH_STENCIL_ATTACHMENT::NONE, Texture::FILTER_METHOD::NEAREST, Texture::WRAP_METHOD::CLAMP_TO_EDGE, Texture::DATA_TYPE::FLOAT);
    m_particlesFBO2 = std::make_shared<FBO>(m_numParticles, 1, 2, 4, FBO::DEPTH_STENCIL_ATTACHMENT::NONE, Texture::FILTER_METHOD::NEAREST, Texture::WRAP_METHOD::CLAMP_TO_EDGE, Texture::DATA_TYPE::FLOAT);

    // Print the max FBO dimension.
    GLint maxRenderBufferSize;
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE_EXT, &maxRenderBufferSize);
    std::cout << "Max FBO size: " << maxRenderBufferSize << std::endl;

}

ParticleSys::~ParticleSys()
{
    glDeleteVertexArrays(1, &m_particlesVAO);
}

void ParticleSys::drawParticles() {

    auto prevFBO = m_evenPass ? m_particlesFBO1 : m_particlesFBO2;
    auto nextFBO = m_evenPass ? m_particlesFBO2 : m_particlesFBO1;
    float firstPass = m_firstPass ? 1.0f : 0.0f;

    // TODO [Task 13] Move the particles from prevFBO to nextFBO while updating them
    nextFBO->bind();
    glUseProgram(m_particleUpdateProgram);\
    glActiveTexture(GL_TEXTURE0);
    prevFBO->getColorAttachment(0).bind();
    glActiveTexture(GL_TEXTURE1);
    prevFBO->getColorAttachment(1).bind();
    glUniform1f(glGetUniformLocation(m_particleUpdateProgram, "firstPass"), firstPass);
    glUniform1i(glGetUniformLocation(m_particleUpdateProgram, "numParticles"), m_numParticles);
    glUniform1i(glGetUniformLocation(m_particleUpdateProgram, "prevPos"), 0);
    glUniform1i(glGetUniformLocation(m_particleUpdateProgram, "prevVel"), 1);
    m_quad->draw();
    // TODO [Task 17] Draw the particles from nextFBO
    nextFBO->unbind();
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_particleDrawProgram);
    setParticleViewport();
    glActiveTexture(GL_TEXTURE0);
    nextFBO->getColorAttachment(0).bind();
    glActiveTexture(GL_TEXTURE1);
    nextFBO->getColorAttachment(1).bind();
    glUniform1i(glGetUniformLocation(m_particleDrawProgram, "pos"), 0);
    glUniform1i(glGetUniformLocation(m_particleDrawProgram, "vel"), 1);
    glUniform1i(glGetUniformLocation(m_particleDrawProgram, "numParticles"), m_numParticles);
    glBindVertexArray(m_particlesVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3 * m_numParticles);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);

    m_firstPass = false;
    m_evenPass = !m_evenPass;
}

// Sets the viewport to ensure that {0,0} is always in the center of the viewport
// in clip space, and to ensure that the aspect ratio is 1:1
void ParticleSys::setParticleViewport() {
    int maxDim = std::max(m_screenWidth, m_screenHeight);
    int x = (m_screenWidth - maxDim) / 2.0f;
    int y = (m_screenHeight - maxDim) / 2.0f;
    glViewport(x, y, maxDim, maxDim);
}


void ParticleSys::rebuildMatrices() {
//    m_view = glm::translate(glm::vec3(0, 0, -m_zoom)) *
//             glm::rotate(m_angleY, glm::vec3(1,0,0)) *
//             glm::rotate(m_angleX, glm::vec3(0,1,0));

//    m_projection = glm::perspective(0.8f, (float)width()/height(), 0.1f, 100.f);
//    update();
}

void ParticleSys::draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawParticles();
    glViewport(0, 0, m_screenWidth, m_screenHeight);
//    update();
}

void ParticleSys::resize(int w, int h)
{
    m_screenHeight = h;
    m_screenWidth = w;
}

void ParticleSys::tick(float seconds)
{

}

QString ParticleSys::getComponentType() const {
    return "";
}

void ParticleSys::addComponent(const std::shared_ptr<Component> &c) {

}

void ParticleSys::removeComponent(const std::shared_ptr<Component> &c)
{

}
