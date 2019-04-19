#include "TextureCube.h"

#include <QImage>
#include <assert.h>

TextureCube::TextureCube(std::__cxx11::string dir, Cubemap c)
{
    std::string skyboxFiles[6] = {c.front, c.back, c.top, c.bottom, c.left, c.right};
    GLenum faces[6] = {GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                       GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                       GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X};

    bind();
    for (int i = 0; i < 6; i++) {
        QImage im((dir + skyboxFiles[i]).c_str());
        im = im.convertToFormat(QImage::Format_RGBA8888);

        std::cout << skyboxFiles[i] << std::endl;

        assert(im.width() > 0);
        assert(im.height() > 0);
        assert(im.width() == im.height());

        glTexImage2D(faces[i], 0, GL_RGBA, im.width(), im.height(),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, im.bits());
    }
    unbind();
}

void TextureCube::bind() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);
}

void TextureCube::unbind() const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCube::_setFilterMethod(GLenum filter)
{
    this->bind();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter);
    this->unbind();
}

void TextureCube::_setWrapMethod(GLenum wrap)
{
    this->bind();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap);
    this->unbind();
}

void TextureCube::_setTextureParams(GLenum filter, GLenum wrap)
{
    this->bind();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrap);
    this->unbind();
}
