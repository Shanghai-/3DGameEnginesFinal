#ifndef TEXTURECUBE_H
#define TEXTURECUBE_H

#include "engine/graphics/Texture.h"

class TextureCube : public Texture
{
public:
    struct Cubemap {
        std::string front;
        std::string back;
        std::string left;
        std::string right;
        std::string top;
        std::string bottom;
    };

    TextureCube(std::string dir, Cubemap c);

    void bind() const;
    void unbind() const;

protected:
    void _setFilterMethod(GLenum filter);
    void _setWrapMethod(GLenum wrap);
    void _setTextureParams(GLenum filter, GLenum wrap);
};

#endif // TEXTURECUBE_H
