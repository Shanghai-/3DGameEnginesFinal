#include "PlayerMovementSys.h"

#include "engine/components/CInputReceiver.h"
#include "engine/components/CCamera.h"
#include "engine/graphics/Camera.h"
#include "engine/components/ColEllipsoid.h"
#include "warmup1/CustomComponents/CPhysics.h"
#include "engine/graphics/Shape.h"
#include "engine/components/CRenderable.h"

#include <iostream>
#include <math.h>

PlayerMovementSys::PlayerMovementSys(int priority) :
    System(priority)
{
    m_input = InputManager::getGlobalInstance();
    m_graphics = Graphics::getGlobalInstance();
    m_grounded = false;
    m_curPos = glm::ivec2(4, 2);
    m_vel = glm::vec3(0.f);
}

PlayerMovementSys::~PlayerMovementSys()
{
}

void PlayerMovementSys::addToVector(std::vector<float> &vector, int index, std::vector<float> values) {
    for(size_t i = 0; i < values.size(); i++) {
        vector[index + i] = values[i];
    }
}

void PlayerMovementSys::addToVector(std::vector<int> &vector, int index, std::vector<int> values) {
    for(size_t i = 0; i < values.size(); i++) {
        vector[index + i] = values[i];
    }
}

QString PlayerMovementSys::getComponentType() const
{
    return typeid(CInputReceiver).name();
}

inline uint qHash(const std::shared_ptr<CTransform> &key) {
    return key->getId();
}

void PlayerMovementSys::addComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CTransform> t = c->getSibling<CTransform>();
    m_transforms.insert(t);

    /* if(m_transforms.size() == 1) {
        addMesh(std::make_unique<CMeshCol>(c->getParent()));
        std::vector<float> vertices(8 * 4);
        std::vector<int> faces(3 * 2);
        addToVector(vertices, 0, {-10, 10, -10, 0, 1, 0, 0, 0});
        addToVector(vertices, 8, {-10, 10, 10, 0, 1, 0, 0, 0});
        addToVector(vertices, 16, {10, 15, -10, 0, 1, 0, 0, 0});
        addToVector(vertices, 24, {10, 15, 10, 0, 1, 0, 0, 0});
        addToVector(faces, 0, {0, 1, 2});
        addToVector(faces, 3, {3, 2, 1});
        m_shape = std::make_shared<Shape>(vertices, faces);
    } */
}

void PlayerMovementSys::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<CTransform> t = c->getSibling<CTransform>();
    m_transforms.remove(t);
}

void PlayerMovementSys::addGlobalMesh(std::shared_ptr<CMeshCol> mesh, glm::ivec2 coord)
{
    m_globalMeshMap[glmToStd(coord)] = mesh;
}

void PlayerMovementSys::addMesh(glm::ivec2 coord)
{
    std::vector<int> vec = glmToStd(coord);
    std::shared_ptr<CMeshCol> coll = m_globalMeshMap[vec];
    m_meshMap[vec] = coll;
    //coll->getSibling<CRenderable>()->setMaterialName("Cave");
}

void PlayerMovementSys::removeMesh(glm::ivec2 coord)
{
    auto res = m_meshMap.find(glmToStd(coord));
    if (res != m_meshMap.end()) {
        m_meshMap.erase(res);
        //std::shared_ptr<CMeshCol> coll = m_globalMeshMap[glmToStd(coord)];
        //coll->getSibling<CRenderable>()->setMaterialName("Ground");
    }

}

std::vector<int> PlayerMovementSys::glmToStd(glm::ivec2 vec)
{
    std::vector<int> retVec;
    retVec.push_back(vec[0]);
    retVec.push_back(vec[1]);
    return retVec;
}

glm::ivec2 PlayerMovementSys::stdToGlm(std::vector<int> vec)
{
    return glm::ivec2(vec[0], vec[1]);
}

void PlayerMovementSys::tick(float seconds)
{
    std::shared_ptr<CCamera> cam = m_graphics->getActiveCamera();
    std::shared_ptr<Camera> graphicsCam = cam->getCameraObject();


    glm::vec3 forward = graphicsCam->getLook();
    forward.y = 0;
    forward = glm::normalize(forward) * 3.0f * 2.f;

    glm::vec3 left = glm::cross(graphicsCam->getUp(), graphicsCam->getLook());
    left.y = 0;
    left = glm::normalize(left) * 3.0f * 2.f;

    QSetIterator<std::shared_ptr<CTransform>> it(m_transforms);

    while (it.hasNext()) {
        std::shared_ptr<CTransform> transform = it.next();

        glm::vec3 dir = glm::vec3(0);
        if (m_input->isPressed(Qt::Key_W)) {
            dir += forward * seconds;
        }
        if (m_input->isPressed(Qt::Key_A)) {
            dir += left * seconds;
        }
        if (m_input->isPressed(Qt::Key_S)) {
            dir -= forward * seconds;
        }
        if (m_input->isPressed(Qt::Key_D)) {
            dir -= left * seconds;
        }
        if(m_grounded && m_input->isPressed(Qt::Key_Space)) {
            std::shared_ptr<CPhysics> phys = transform->getSibling<CPhysics>();
            phys->vel += glm::vec3(0.f, 2.f, 0.f);
            m_grounded = false;
        }

        // This makes the fox point in the direction that it is moving
        if (dir.x != 0.0f || dir.z != 0.0f) {
            // We take the dot product between straight forward and the target direction to get the
            // cosine of the angle between the two, then use arccos to resolve that to an angle
            float angle = glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), glm::normalize(dir));
            angle = (dir.x > 0.0f) ? glm::acos(angle) : -glm::acos(angle);
            transform->rot.y = angle;
        }

//        returnType values = checkCollision(transform->pos, transform->pos + dir, transform->getSibling<ColEllipsoid>()->getRadii());
//        glm::vec3 hit = transform->pos + (dir * values.time);
//        transform->pos = hit + (0.01f * values.normal);

        std::shared_ptr<CPhysics> phys = transform->getSibling<CPhysics>();
        dir += phys->vel;

        int repetitions = 3;
        float timeTraveled = 0.f;
        for(int i = 0; i < repetitions; i++) {
            returnType values = checkCollision(transform->pos, transform->pos + dir, transform->getSibling<ColEllipsoid>()->getRadii());
            glm::vec3 hit = transform->pos + (dir * values.time);
            //std::cout << glm::to_string(values.normal) << std::endl;
            transform->pos = hit + (0.01f * values.normal);

            if(values.time < 1.f) {
                phys->vel = glm::vec3(0.f, 0.f, 0.f);
                if(values.normal.y > 0.f) {
                    m_grounded = true;
                }
            }
            else {
                phys->vel += (phys->acc * seconds);
                m_grounded = false;
            }
            timeTraveled += values.time;
            if(timeTraveled >= 1.f) {
                break;
            }
        }

        //UNCOMMENT THIS ONCE WE ACTUALLY HAVE THE MESH CHUNKS IN
        updateMeshMap(glm::vec2(transform->pos[0], transform->pos[2]));
    }
}

void PlayerMovementSys::updateMeshMap(glm::vec2 pos2D)
{
    glm::ivec2 posCoords;
    posCoords[0] = glm::clamp((int)glm::floor((pos2D[0] - 8.f) / 16.f) + 5, 0, 9);
    posCoords[1] = 9 - glm::clamp((int)glm::floor((pos2D[1] + 8.f) / 16.f) + 5, 0, 9);
    if(m_curPos != posCoords) {
        glm::ivec2 dir = posCoords - m_curPos;
        if(dir[0] != 0) {
            int backX = m_curPos[0] - dir[0];
            if(backX >= 0 && backX <= 9) {
                for(int y = glm::max(m_curPos[1] - 1, 0); y <= glm::min(m_curPos[1] + 1, 9); y++) {
                    removeMesh(glm::ivec2(backX, y));
                }
            }
            int frontX = posCoords[0] + dir[0];
            if(frontX >= 0 && frontX <= 9) {
                for(int y = glm::max(m_curPos[1] - 1, 0); y <= glm::min(m_curPos[1] + 1, 9); y++) {
                    addMesh(glm::ivec2(frontX, y));
                }
            }
        }

        if(dir[1] != 0) {
            int backY = m_curPos[1] - dir[1];
            if(backY >= 0 && backY <= 9) {
                for(int x = glm::max(m_curPos[0] - 1, 0); x <= glm::min(m_curPos[0] + 1, 9); x++) {
                    removeMesh(glm::ivec2(x, backY));
                }
            }
            int frontY = posCoords[1] + dir[1];
            if(frontY >= 0 && frontY <= 9) {
                for(int x = glm::max(m_curPos[0] - 1, 0); x <= glm::min(m_curPos[0] + 1, 9); x++) {
                    addMesh(glm::ivec2(x, frontY));
                }
            }
        }

        m_curPos = posCoords;
    }
}

glm::vec3 PlayerMovementSys::scaleVector(glm::vec3 vector, glm::mat4x4 scale) {
    glm::vec4 vec = glm::inverse(scale) * glm::vec4(vector, 1.f);
    return glm::vec3(vec);
}

PlayerMovementSys::returnType PlayerMovementSys::checkCollision(glm::vec3 start, glm::vec3 end, glm::vec3 rad)
{
    returnType values;
    values.contactPoint = end;
    values.normal = glm::vec3(0.f, 0.f, 0.f);
    values.time = 1.f;

    glm::mat4x4 model = glm::translate(start) * glm::scale(rad);

    start = scaleVector(start, model);
    end = scaleVector(end, model);

    for(std::map<std::vector<int>, std::shared_ptr<CMeshCol>>::iterator it = m_meshMap.begin(); it != m_meshMap.end(); ++it) {
        std::shared_ptr<CMeshCol> mesh = it->second;
        glm::vec3 transform = mesh->getSibling<CTransform>()->pos;
        std::vector<glm::vec3> faces = mesh->getFaces();
        std::vector<glm::vec3> vertexList = mesh->getVertices();
        std::vector<glm::vec3> normals = mesh->getNormals();
        for(int i = 0; i < faces.size(); i++) {
            glm::vec3 face = faces[i];
            glm::vec3 v0 = scaleVector(vertexList[face[0]] + transform, model);
            glm::vec3 v1 = scaleVector(vertexList[face[1]] + transform, model);
            glm::vec3 v2 = scaleVector(vertexList[face[2]] + transform, model);

            glm::vec4 norm = glm::normalize(model * glm::vec4(normals[i], 0.f));
            //if(glm::length2(normals[i]) < 0.001f) {
            if (isnan(norm.x) || isnan(norm.y) || isnan(norm.z)) {
                //std::cout << glm::to_string(normals[i]) << std::endl;
            }
            glm::vec3 N = glm::vec3(norm);

            //Check ellipsoid/interior
            float t = glm::dot(-N, start - N - v0) / glm::dot(N, end - start);
            glm::vec3 P = (start - N) + ((end - start) * t);
            if(glm::dot(glm::cross(v1 - v0, P - v0), N) > 0.f && glm::dot(glm::cross(v2 - v1, P - v1), N) > 0.f && glm::dot(glm::cross(v0 - v2, P - v2), N) > 0.f) {
                //std::cout << t << std::endl;
                if(t > 0.f && t < values.time) {
                    //std::cout << "interior" << std::endl;
                    values.time = t;
                    values.contactPoint = P;
                    values.normal = N;
                }
                continue;
            }
            //Check ellipsoid/edge
            std::vector<glm::vec3> vertices = {v0, v1, v2};
            bool cont = false;
            for(int j = 0; j < vertices.size(); j++) {
                glm::vec3 C = vertices[j];
                glm::vec3 D = vertices[(j + 1) % vertices.size()];
                float aQuad = glm::pow(glm::length(glm::cross(end - start, D - C)), 2.f);
                float bQuad = 2.f * glm::dot(glm::cross(end - start, D - C), glm::cross(start - C, D - C));
                float cQuad = glm::pow(glm::length(glm::cross(start - C, D - C)), 2.f) - glm::pow(glm::length(D - C), 2.f);

                float det = glm::pow(bQuad, 2.f) - (4.f * aQuad * cQuad);
                if(det >= 0.f) {
                    t = (-bQuad + glm::sqrt(det)) / (2.f * aQuad);
                    float t2 = (-bQuad - glm::sqrt(det)) / (2.f * aQuad);
                    if(t2 < t) {
                        t = t2;
                    }
                    P = start + (end - start) * t;
                    float checkVal = glm::dot(P - C, D - C);
                    if(0.f < checkVal && checkVal < glm::pow(glm::length(D - C), 2.f)) {
                        if(t > 0.f && t < values.time) {
                            //std::cout << "edge" << std::endl;
                            values.time = t;
                            values.contactPoint = P;
                            values.normal = N;
                        }
                        cont = true;
                    }
                }
            }
            if(cont) {
                continue;
            }

            //Check ellispoid/vertex
            for(int j = 0; j < vertices.size(); j++) {
                glm::vec3 V = vertices[j];
                float aQuad = glm::pow(glm::length(end - start), 2.f);
                float bQuad = -2.f * glm::dot(end - start, V - start);
                float cQuad = glm::pow(glm::length(V - start), 2.f) - 1.f;

                float det = glm::pow(bQuad, 2.f) - (4.f * aQuad * cQuad);
                if(det >= 0.f) {
                    t = (-bQuad + glm::sqrt(det)) / (2.f * aQuad);
                    float t2 = (-bQuad - glm::sqrt(det)) / (2.f * aQuad);
                    if(t2 < t) {
                        t = t2;
                    }
                    P = V - ((end - start) * t);
                    if(glm::abs(glm::pow(glm::length(P - start), 2.f) - 1.f) < 0.00001f) {
                        if(t > 0.f && t < values.time) {
                            //std::cout << "vertex" << std::endl;
                            values.time = t;
                            values.contactPoint = P;
                            values.normal = N;
                        }
                    }
                }
            }
        }
    }

    if(glm::length2(values.normal) != 0.f) {
        values.normal = glm::normalize(glm::vec3(glm::transpose(glm::inverse(model)) * glm::vec4(values.normal, 0.f)));
    }

    values.contactPoint = glm::vec3(model * glm::vec4(values.contactPoint, 1.f));

    return values;
}

void PlayerMovementSys::draw()
{
    QSetIterator<std::shared_ptr<CTransform>> it(m_transforms);

    while (it.hasNext()) {
        std::shared_ptr<CTransform> transform = it.next();
        transform->getSibling<ColEllipsoid>()->drawWireframe();
    }

    /* Graphics *g = Graphics::getGlobalInstance();

    g->clearTransform();
    g->translate(glm::vec3(0.f));
    g->scale(glm::vec3(1.f));

    g->drawShape(m_shape); */
}
