#ifndef OPENGLSHAPE_H
#define OPENGLSHAPE_H

#include "GL/glew.h"

#include <memory>
#include <vector>

#include "engine/graphics/VBO.h"
#include "engine/graphics/VBOAttribMarker.h"

class VAO;

class OpenGLShape {
public:
    OpenGLShape();

    ~OpenGLShape();

    /** Initialize the VBO with the given vertex data. */
    void setVertexData(GLfloat *data, int size, VBO::GEOMETRY_LAYOUT drawMode, int numVertices);

    /** Enables the specified attribute and calls glVertexAttribPointer with the given arguments. */
    void setAttribute(GLuint index, GLuint numElementsPerVertex, int offset, VBOAttribMarker::DATA_TYPE type,
                      bool normalize);

    /** Build the VAO given the specified vertex data and atrributes */
    void buildVAO();

    /** Draw the initialized geometry. */
    void draw();

private:
    GLfloat *m_data;                            /// vector of floats containing the vertex data.
    GLsizeiptr m_size;                          /// size of the data array, in bytes.
    VBO::GEOMETRY_LAYOUT m_drawMode;            /// drawing mode
    int m_numVertices;                          /// number of vertices to be rendered
    std::vector<VBOAttribMarker> m_markers;     /// list of VBOAttribMarkers that describe how the data is laid out.
    std::unique_ptr<VAO> m_VAO;      /// a wrapper for the vertex array object (VAO)

};

#endif // OPENGLSHAPE_H
