#include "networksystem.h"
#include <iostream>
#include "raknet/MessageIdentifiers.h"
#include "raknet/BitStream.h"
#include "raknet/RakNetTypes.h"
#include "engine/components/CTransform.h"
#include "engine/objectManagement/GameWorld.h"

#define MAX_CLIENTS 10
#define SERVER_PORT 60000

enum GameMessages
{
    ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM+1,
    ID_CREATE_PLAYER = ID_USER_PACKET_ENUM+2,
    ID_TRANSFORM = ID_USER_PACKET_ENUM+3
};

NetworkSystem::NetworkSystem(int priority, GameWorld *gameworld, bool isServer) : System(priority),
    m_isServer(isServer),
    m_gw(gameworld)
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

        m_peer->Connect("10.116.72.56", SERVER_PORT, 0, 0);
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

    if (m_isServer) {
        PlayerObject *player = new PlayerObject;
        player->SetNetworkIDManager(&m_networkIDManager);
        BitStream bsOut;
        bsOut.Write((MessageID)ID_CREATE_PLAYER);
        bsOut.Write(player->GetNetworkID());
        m_peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
    }

    m_network.insert(netComp);
}

void NetworkSystem::removeComponent(const std::shared_ptr<Component> &c)
{
    std::shared_ptr<NetworkComponent> netComp = std::dynamic_pointer_cast<NetworkComponent>(c);
    m_network.remove(netComp);
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
                bsOut.Write((MessageID)ID_GAME_MESSAGE_1);
                bsOut.Write("Hello world");
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
            case ID_GAME_MESSAGE_1:
                {
                    RakString rs;
                    BitStream bsIn(packet->data,packet->length,false);
                    bsIn.IgnoreBytes(sizeof(MessageID));
                    bsIn.Read(rs);
                    std::cout<<rs.C_String()<<std::endl;
                }
                break;
            case ID_CREATE_PLAYER:
                {
                    BitStream bsIn(packet->data, packet->length, false);
                    bsIn.IgnoreBytes(sizeof(MessageID));
                    NetworkID playerID;
                    bsIn.Read(playerID);
                    PlayerObject *player = new PlayerObject;
                    player->SetNetworkIDManager(&m_networkIDManager);
                    player->SetNetworkID(playerID);
                }
                break;
            case ID_TRANSFORM:
                {
//                    BitStream bsIn(packet->data, packet->length, false);
//                    bsIn.IgnoreBytes(sizeof(MessageID));
                    float posX,posY,posZ,rotX,rotY,rotZ;

                }
                break;
            default:
                std::cout<<"Message with identifier "<<packet->data[0]<<" has arrived"<<std::endl;
                break;
            }
    }

//    if (m_isServer) {
//        // send packets to clients
//    } else {
//        //send transformation to server
//        std::shared_ptr<CTransform> trans = m_player->getSibling<CTransform>();
//        MessageID typeId = ID_TRANSFORM;
//        BitStream bsOut;
//        bsOut.Write(typeID);
//        bsOut.Write(trans->pos.x);
//        bsOut.Write(trans->pos.y);
//        bsOut.Write(trans->pos.z);
//        bsOut.Write(trans->rot.x);
//        bsOut.Write(trans->rot.y);
//        bsOut.Write(trans->rot.z);
//    }

}

void NetworkSystem::SetPlayer(std::shared_ptr<NetworkComponent> comp) {
    m_player = comp;
}

