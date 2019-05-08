#include "networksystem.h"
#include <iostream>
#include "raknet/MessageIdentifiers.h"
#include "raknet/BitStream.h"
#include "raknet/RakNetTypes.h"
#include "engine/components/CTransform.h"
#include "engine/objectManagement/GameWorld.h"
#include "engine/components/CRenderable.h"
#include "engine/components/CAnimatedMesh.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

enum GameMessages
{
    ID_NEW_PLAYER = ID_USER_PACKET_ENUM+1,
    ID_CREATE_PLAYER = ID_USER_PACKET_ENUM+2,
    // client transform is the transform sent by the client
    ID_CLIENT_TRANSFORM = ID_USER_PACKET_ENUM+3,
    // server transform is the aggregation of all client transforms in a span of time
    ID_SERVER_TRANSFORM = ID_USER_PACKET_ENUM+4,
    ID_NETWORKID_INITIALIZED = ID_USER_PACKET_ENUM+5
};

NetworkSystem::NetworkSystem(int priority, GameWorld *gameworld, bool isServer) : System(priority),
    m_isServer(isServer),
    m_gw(gameworld),
    m_isInitialized(false)
{
    m_peer = RakPeerInterface::GetInstance();

    if (isServer) {
        SocketDescriptor sd(SERVER_PORT, 0);
        m_peer->Startup(MAX_CLIENTS, &sd, 1);
        std::cout<<"Starting the server"<<std::endl;
        m_peer->SetMaximumIncomingConnections(MAX_CLIENTS);
    } else {
        SocketDescriptor sd;
        m_peer->Startup(1, &sd, 1);
        m_peer->Ping("255.255.255.255", SERVER_PORT, true);
        //m_peer->Connect("255.255.255.255", SERVER_PORT, 0, 0);
    }
}

NetworkSystem::~NetworkSystem() {
    RakPeerInterface::DestroyInstance(m_peer);
}

QString NetworkSystem::getComponentType() const
{
    return typeid(NetworkComponent).name();
}

inline uint qHash(const std::shared_ptr<NetworkComponent> &key) {
    return key->getId();
}


void NetworkSystem::addComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<NetworkComponent> netComp = std::dynamic_pointer_cast<NetworkComponent>(c);
    m_networkObjects.insert(netComp);
}

void NetworkSystem::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<NetworkComponent> netComp = std::dynamic_pointer_cast<NetworkComponent>(c);
    m_networkObjects.remove(netComp);
}

void NetworkSystem::tick(float seconds)
{
    Packet *packet;
    for (packet = m_peer->Receive(); packet; m_peer->DeallocatePacket(packet), packet = m_peer->Receive()) {
        switch (packet->data[0]) {
            case ID_REMOTE_DISCONNECTION_NOTIFICATION:
                std::cout<<"Another client has disconnected"<<std::endl;
                break;
            case ID_REMOTE_CONNECTION_LOST:
                std::cout<<"Another client has lost the connection"<<std::endl;
                break;
            case ID_REMOTE_NEW_INCOMING_CONNECTION:
                std::cout<<"Another client has connected"<<std::endl;
                break;
            case ID_CONNECTION_REQUEST_ACCEPTED:
                {
                std::cout<<"Our connection request has been accepted"<<std::endl;

                BitStream bsOut;
                bsOut.Write((MessageID)ID_NEW_PLAYER);
                m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED,0,packet->systemAddress,false);

                }
                break;
            case ID_NEW_INCOMING_CONNECTION:
                std::cout<<"A connection is incoming"<<std::endl;
                break;
            case ID_NO_FREE_INCOMING_CONNECTIONS:
                std::cout<<"The server is full"<<std::endl;
                break;
            case ID_DISCONNECTION_NOTIFICATION:
                if (m_isServer) {
                    std::cout<<"A client has disconnected"<<std::endl;
                } else {
                    std::cout<<"We have been disconnected"<<std::endl;
                }
                break;
            case ID_CONNECTION_LOST:
                if (m_isServer) {
                    std::cout<<"A client lost the connection"<<std::endl;
                } else {
                    std::cout<<"Connection lost"<<std::endl;
                }
                break;
//            case ID_UNCONNECTED_PING_OPEN_CONNECTIONS:
//                m_peer->AdvertiseSystem(packet->systemAddress.ToString(true), SERVER_PORT, 0, 0);
//                break;
//            case ID_ADVERTISE_SYSTEM:
//                {
//                    std::cout<<"it worked"<<std::endl;
//                }
//                break;
            case ID_UNCONNECTED_PONG:
                {
                    m_serverAddress = packet->systemAddress;
                    m_peer->Connect(packet->systemAddress.ToString(false), SERVER_PORT, 0, 0);
                }
                break;
            case ID_NEW_PLAYER:
                {
                    // This is for the server to send out a message telling all servers to create a new player and gameobject
                    if (m_isServer) {
                        PlayerObject *player = new PlayerObject;
                        player->SetNetworkIDManager(&m_networkIDManager);
                        BitStream bsOut;
                        bsOut.Write((MessageID)ID_CREATE_PLAYER);
                        bsOut.Write(player->GetNetworkID());
                        bsOut.Write(packet->guid);
                        m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);
                        addPlayer(player, "Player" + QString::number(m_networkObjects.size()), player->GetNetworkID());
                        //note: calling addPlayer changes the size of m_networkObjects
                        bsOut.Write(m_activeNetworkIDs.size());
                        for (auto id : m_activeNetworkIDs) {
                            bsOut.Write(id);
                        }
                        m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
                    }
                }
                break;
            case ID_CREATE_PLAYER:
                {
                    //This will sync all player network ids across the server
                    BitStream bsIn(packet->data, packet->length, false);
                    bsIn.IgnoreBytes(sizeof(MessageID));
                    NetworkID playerID;
                    bsIn.Read(playerID);
                    RakNetGUID guid;
                    bsIn.Read(guid);
                    PlayerObject *playerObject = new PlayerObject;
                    playerObject->SetNetworkIDManager(&m_networkIDManager);
                    playerObject->SetNetworkID(playerID);
                    m_activeNetworkIDs.insert(playerID);
                    if (guid == m_peer->GetMyGUID()) {
                        // Need to add all previous players to new player gameworld
                        int len;
                        bsIn.Read(len);
                        for (int i = 0; i < len; i++) {
                            NetworkID netID;
                            bsIn.Read(netID);
                            QString name = "Player" + QString::number(i);
                            PlayerObject *oldPlayerObject = new PlayerObject;
                            oldPlayerObject->SetNetworkIDManager(&m_networkIDManager);
                            oldPlayerObject->SetNetworkID(netID);
                            addPlayer(oldPlayerObject, name, netID);
                            m_activeNetworkIDs.insert(netID);
                            //std::cout<<netID<<std::endl;
                        }
                        //std::cout<< "playerID should be:" << std::endl;
                        m_player->setNetworkID(playerID);
                        BitStream bsOut;
                        bsOut.Write((MessageID)ID_NETWORKID_INITIALIZED);
//                        std::cout<<"[";
//                        for (NetworkID id : m_activeNetworkIDs) {
//                            std::cout<< id << ","<< std::endl;
//                        }
//                        std::cout<<"]"<<std::endl;
                        bsOut.Write(playerID);
                        m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverAddress, false);
                    } else {
                        QString name = "Player" + QString::number(m_networkObjects.size());
                        addPlayer(playerObject, name, playerID);
                    }
                    m_isInitialized = true;
                }
                break;
            case ID_CLIENT_TRANSFORM:
                {
                    if (m_isServer) {
                        BitStream bsIn(packet->data, packet->length, false);
                        bsIn.IgnoreBytes(sizeof(MessageID));
                        float posX,posY,posZ,rotX,rotY,rotZ;
                        NetworkID netID;
                        bsIn.Read(netID);

                        bsIn.ReadVector(posX,posY,posZ);
                        bsIn.ReadVector(rotX,rotY,rotZ);
                        glm::vec3 pos(posX,posY,posZ);
                        glm::vec3 rot(rotX,rotY,rotZ);
                        auto playerObj = m_networkIDManager.GET_OBJECT_FROM_ID<PlayerObject*>(netID);
                        auto trans = playerObj->networkComponent->getSibling<CTransform>();
                        trans->pos = pos;
                        trans->rot = rot;

                        BitStream bsOut;
                        bsOut.Write((MessageID) ID_SERVER_TRANSFORM);
                        bsOut.Write(m_activeNetworkIDs.size());
//                        std::cout<<"active Net IDs: " <<m_activeNetworkIDs.size()<<std::endl;
//                        std::cout<<"netobjs: "<<m_networkObjects.contains(m_player)<<std::endl;
                        for (auto id : m_activeNetworkIDs) {
                            bsOut.Write(id);
                            auto netObj = m_networkIDManager.GET_OBJECT_FROM_ID<PlayerObject*>(id)->networkComponent;
                            std::shared_ptr<CTransform> trans = netObj->getSibling<CTransform>();
                            bsOut.WriteVector(trans->pos.x, trans->pos.y, trans->pos.z);
                            bsOut.WriteVector(trans->rot.x, trans->rot.y, trans->rot.z);
                            m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
                    }
                }
                break;
            case ID_SERVER_TRANSFORM:
                {
//                    if (!m_isInitialized) {
//                        break;
//                    }
//                    BitStream bsIn(packet->data, packet->length, false);
//                    bsIn.IgnoreBytes(sizeof(MessageID));
//                    int len;
//                    bsIn.Read(len);
//                    for (int i = 0; i < len; i++) {
//                        NetworkID netID;
//                        bsIn.Read(netID);
//                        float posX,posY,posZ,rotX,rotY,rotZ;
//                        bsIn.ReadVector(posX,posY,posZ);
//                        bsIn.ReadVector(rotX,rotY,rotZ);
//                        glm::vec3 pos(posX,posY,posZ);
//                        glm::vec3 rot(rotX,rotY,rotZ);
//                        auto playerObj = m_networkIDManager.GET_OBJECT_FROM_ID<PlayerObject*>(netID);
//                        auto trans = playerObj->networkComponent->getSibling<CTransform>();
//                        trans->pos = pos;
//                        trans->rot = rot;
//                    }
                        BitStream bsIn(packet->data, packet->length, false);
                        bsIn.IgnoreBytes(sizeof(MessageID));
                        int len;
                        bsIn.Read(len);
                        //std::cout<<"len: "<<len<<std::endl;
                        for (int i = 0; i < len; i++) {
                            float posX,posY,posZ,rotX,rotY,rotZ;
                            NetworkID netID;
                            bsIn.Read(netID);
                            bsIn.ReadVector(posX,posY,posZ);
                            bsIn.ReadVector(rotX,rotY,rotZ);
                            glm::vec3 pos(posX,posY,posZ);
                            glm::vec3 rot(rotX,rotY,rotZ);
                            if (netID == m_player->getNetworkID()) {
//                                auto trans = m_player->getSibling<CTransform>();
//                                trans->pos = pos;
//                                trans->rot = rot;
                                break;
                            } else {
                                auto playerObj = m_networkIDManager.GET_OBJECT_FROM_ID<PlayerObject*>(netID);
                                auto trans = playerObj->networkComponent->getSibling<CTransform>();
                                trans->pos = pos;
                                trans->rot = rot;
                            }
                       }
                }
                break;
            case ID_NETWORKID_INITIALIZED:
                {
                    if (m_isServer) {
                        BitStream bsIn(packet->data, packet->length, false);
                        bsIn.IgnoreBytes(sizeof(MessageID));
                        NetworkID netID;
                        bsIn.Read(netID);
                        m_activeNetworkIDs.insert(netID);
                    }
                }
                break;
            default:
                std::cout<<"Message with identifier "<<packet->data[0]<<" has arrived"<<std::endl;
                break;
            }
        }
    }
    if (m_isInitialized && !m_isServer) {
        std::shared_ptr<CTransform> trans = m_player->getSibling<CTransform>();
        MessageID typeID = ID_CLIENT_TRANSFORM;
        BitStream bsOut;
        bsOut.Write(typeID);
        bsOut.Write(m_player->getNetworkID());
        bsOut.WriteVector(trans->pos.x, trans->pos.y, trans->pos.z);
        bsOut.WriteVector(trans->rot.x, trans->rot.y, trans->rot.z);
        m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverAddress, false);
    }
}

void NetworkSystem::setPlayer(std::shared_ptr<NetworkComponent> comp) {
    m_player = comp;
    if (m_isServer) {
        PlayerObject *player = new PlayerObject;
        player->SetNetworkIDManager(&m_networkIDManager);
        m_player->setNetworkID(player->GetNetworkID());
        player->networkComponent = m_player;
        m_activeNetworkIDs.insert(player->GetNetworkID());
        //std::cout<<player->GetNetworkID()<<std::endl;
    }
}

void NetworkSystem::addPlayer(PlayerObject *playerObject, QString name, NetworkID netID) {
    std::shared_ptr<GameObject> player = std::make_shared<GameObject>(name, m_gw->getNewObjID());
    auto netComp = std::make_shared<NetworkComponent>(player, netID);
    player->addComponent(netComp);
    playerObject->networkComponent = netComp;
    player->addComponent(std::make_shared<CTransform>(player, false, glm::vec3(0.f, 22.0f, 50.0f), glm::vec3(0.0f), glm::vec3(0.2f)));
    player->addComponent(std::make_shared<CAnimatedMesh>(player, "/course/cs1950u/.archive/2019/student/vulpecula/fox.fbx", "PureWhite"));
//    player->addComponent(std::make_shared<CRenderable>(player, "cube", "Star"));
//                    auto coll = std::make_shared<CollCylinder>(glm::vec3(0.f, -0.375f, 0.f), 0.75f, 0.8f);
//                    auto comp = std::make_shared<CCollider>(player, coll, false);
//                    player->addComponent(comp);
//                    player->addComponent(std::make_shared<CInputReceiver>(player));
//                    player->addComponent(std::make_shared<ColEllipsoid>(player, glm::vec3(.75f, .5f, 1.25f)));
//                    player->addComponent(std::make_shared<CPhysics>(player, glm::vec3(0.f, -.2f, 0.f)));
    m_gw->addGameObject(player);
}

