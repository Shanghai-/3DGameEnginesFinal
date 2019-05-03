#ifndef PARTICLESYS_H
#define PARTICLESYS_H
#include "GL/glew.h"
#include <QGLWidget>
#include <QTimer>

#include "glm/glm.hpp"            // glm::vec*, mat*, and basic glm functions
#include "glm/gtx/transform.hpp"  // glm::translate, scale, rotate
#include "glm/gtc/type_ptr.hpp"   // glm::value_ptr

#include <memory>  // std::unique_ptr

#include "engine/graphics/FBO.h"

class OpenGLShape;

class ParticleSys : public QGLWidget {
    Q_OBJECT

public:
    ParticleSys(QGLFormat format, QWidget *parent = 0);
    ~ParticleSys();

protected:
    void initializeGL();
    void paintGL();

private:
    void drawBlur();
    void drawParticles();
    void setParticleViewport();

    void rebuildMatrices();

    int m_width;
    int m_height;

    GLuint m_phongProgram;
    GLuint m_textureProgram;
    GLuint m_horizontalBlurProgram;
    GLuint m_verticalBlurProgram;
    GLuint m_particleUpdateProgram;
    GLuint m_particleDrawProgram;

    std::shared_ptr<OpenGLShape> m_quad;
    std::shared_ptr<OpenGLShape> m_sphere;

    std::unique_ptr<FBO> m_blurFBO1;
    std::unique_ptr<FBO> m_blurFBO2;

    GLuint m_particlesVAO;
    std::shared_ptr<FBO> m_particlesFBO1;
    std::shared_ptr<FBO> m_particlesFBO2;
    bool m_firstPass;
    bool m_evenPass;
    int m_numParticles;

    glm::mat4 m_view, m_projection;

    /** For mouse interaction. */
    float m_angleX, m_angleY, m_zoom;
    QPoint m_prevMousePos;
};

#endif // PARTICLESYS_H
