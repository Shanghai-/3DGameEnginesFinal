#include "CAudioSource.h"

CAudioSource::CAudioSource(std::shared_ptr<GameObject> parent, QString path) :
    CAudioSource(parent, path, false)
{
}

CAudioSource::CAudioSource(std::shared_ptr<GameObject> parent, QString path, bool follows_head) :
    Component(parent),
    m_path(path),
    m_ambient(follows_head)
{
}

CAudioSource::~CAudioSource()
{
}
