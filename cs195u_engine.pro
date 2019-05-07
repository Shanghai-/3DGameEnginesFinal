QT += core gui opengl

TARGET = cs195u_engine
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++14
CONFIG += c++14

unix:!macx {
    LIBS += -lGLU
    INCLUDEPATH += libs/fmod/
    LIBS += -L"$$PWD/libs/fmod/" -lfmodL
    LIBS += -lassimp
}
macx {
    QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
    CONFIG += c++11
}
win32 {
    DEFINES += GLEW_STATIC
    LIBS += -lopengl32 -lglu32
}

SOURCES += \
    libs/glew-1.10.0/src/glew.c \
    src/main.cpp \
    src/mainwindow.cpp \
    src/view.cpp \
    src/viewformat.cpp \
    src/engine/graphics/ResourceLoader.cpp \
    src/engine/graphics/FBO.cpp \
    src/engine/graphics/IBO.cpp \
    src/engine/graphics/Texture.cpp \
    src/engine/graphics/Texture1D.cpp \
    src/engine/graphics/Texture2D.cpp \
    src/engine/graphics/Texture3D.cpp \
    src/engine/graphics/VAO.cpp \
    src/engine/graphics/VBO.cpp \
    src/engine/graphics/VBOAttribMarker.cpp \
    src/engine/graphics/Font.cpp \
    src/engine/graphics/DepthBuffer.cpp \
    src/engine/graphics/RenderBuffer.cpp \
    src/engine/graphics/GraphicsDebug.cpp \
    src/engine/graphics/Shader.cpp \
    src/engine/graphics/Camera.cpp \
    src/engine/graphics/Shape.cpp \
    src/engine/graphics/Graphics.cpp \
    src/engine/graphics/Material.cpp \
    src/engine/graphics/Light.cpp \
    src/engine/frame/Application.cpp \
    src/engine/frame/Screen.cpp \
    src/warmup1/WarmupApp.cpp \
    src/engine/input/InputManager.cpp \
    src/engine/objectManagement/GameWorld.cpp \
    src/engine/objectManagement/GameObject.cpp \
    src/engine/components/Component.cpp \
    src/engine/components/CCollider.cpp \
    src/engine/systems/CollisionSystem.cpp \
    src/engine/components/CTransform.cpp \
    src/warmup1/CustomComponents/CPhysics.cpp \
    src/engine/systems/RenderSystem.cpp \
    src/warmup1/CustomSystems/PhysicsSystem.cpp \
    src/engine/gen/SpacePartMap.cpp \
    src/dungeon_crawler/DungeonCrawlerApp.cpp \
    src/dungeon_crawler/GameplayScreen.cpp \
    src/dungeon_crawler/ChunkFactory.cpp \
    src/dungeon_crawler/ChunkLoader.cpp \
    src/engine/graphics/AABB.cpp \
    src/engine/components/volumes/CollBox.cpp \
    src/engine/components/volumes/CollSphere.cpp \
    src/engine/components/volumes/CollCylinder.cpp \
    src/engine/components/CRenderable.cpp \
    src/engine/systems/CameraSystem.cpp \
    src/engine/components/CCamera.cpp \
    src/engine/systems/FPSCameraSystem.cpp \
    src/warmup1/CustomSystems/InputSystem.cpp \
    src/dungeon_crawler/MovementSys.cpp \
    src/engine/components/CInputReceiver.cpp \
    src/engine/components/volumes/CollComposite.cpp \
    src/engine/systems/RaycastSystem.cpp \
    src/engine/systems/AISystem.cpp \
    src/engine/components/AIComponent.cpp \
    src/engine/ai/Sequence.cpp \
    src/engine/ai/Selector.cpp \
    src/engine/ai/AStarNode.cpp \
    src/dungeon_crawler/NavmeshDrawer.cpp \
    src/engine/components/CNavMesh.cpp \
    src/dungeon_crawler/ai/RadiusCondition.cpp \
    src/engine/ai/Blackboard.cpp \
    src/dungeon_crawler/ai/MoveToPlayer.cpp \
    src/dungeon_crawler/ai/EnemyAISystem.cpp \
    src/dungeon_crawler/CChunkInfo.cpp \
    src/dungeon_crawler/ai/BackAway.cpp \
    src/dungeon_crawler/ai/Strafe.cpp \
    src/dungeon_crawler/ai/FaceAndShoot.cpp \
    src/dungeon_crawler/CHealth.cpp \
    src/dungeon_crawler/CooldownSys.cpp \
    src/dungeon_crawler/CCooldown.cpp \
    src/dungeon_crawler/ShootSystem.cpp \
    src/dungeon_crawler/CBullet.cpp \
    src/dungeon_crawler/PlayerCollision.cpp \
    src/engine/ai/RandomSelector.cpp \
    src/engine/ai/Inverter.cpp \
    src/engine/ai/RepeatUntilDone.cpp \
    src/engine/ui/UIObject.cpp \
    src/engine/ui/UIDrawer.cpp \
    src/engine/ui/UIRenderable.cpp \
    src/engine/ui/UITransform.cpp \
    src/engine/ui/UIText.cpp \
    src/engine/ui/TextDrawer.cpp \
    src/dungeon_crawler/HealthScript.cpp \
    src/engine/ui/UIScriptSys.cpp \
    src/dungeon_crawler/LandingScreen.cpp \
    src/engine/ui/UIClickSystem.cpp \
    src/dungeon_crawler/StartButtonResp.cpp \
    src/dungeon_crawler/PauseScreen.cpp \
    src/engine/components/CAudioSource.cpp \
    src/engine/systems/AudioSystem.cpp \
    src/vulpecula/MainScreen.cpp \
    src/engine/systems/ThirdPersonCamSys.cpp \
    src/vulpecula/VulpeculaApp.cpp \
    src/warmup1/WarmupScreen.cpp \
    src/vulpecula/components/RandomAudioSource.cpp \
    src/vulpecula/systems/RandomAudioManager.cpp \
    src/vulpecula/systems/PlayerMovementSys.cpp \
    src/vulpecula/responders/GuitarZoneResp.cpp \
    src/vulpecula/responders/GuitarStarResp.cpp \
    src/engine/graphics/RiggedMesh.cpp \
    src/engine/systems/AnimationSystem.cpp \
    src/engine/components/CAnimatedMesh.cpp \
    src/engine/graphics/Animation.cpp \
    src/engine/graphics/Armature.cpp \
    src/engine/graphics/TextureCube.cpp \
    src/engine/components/cmeshcol.cpp \
    src/engine/components/ColEllipsoid.cpp \
    libs/raknet/WSAStartupSingleton.cpp \
    libs/raknet/VitaIncludes.cpp \
    libs/raknet/VariadicSQLParser.cpp \
    libs/raknet/VariableListDeltaTracker.cpp \
    libs/raknet/VariableDeltaSerializer.cpp \
    libs/raknet/UDPProxyServer.cpp \
    libs/raknet/UDPProxyCoordinator.cpp \
    libs/raknet/UDPProxyClient.cpp \
    libs/raknet/UDPForwarder.cpp \
    libs/raknet/TwoWayAuthentication.cpp \
    libs/raknet/ThreadsafePacketLogger.cpp \
    libs/raknet/TelnetTransport.cpp \
    libs/raknet/TeamManager.cpp \
    libs/raknet/TeamBalancer.cpp \
    libs/raknet/TCPInterface.cpp \
    libs/raknet/TableSerializer.cpp \
    libs/raknet/SuperFastHash.cpp \
    libs/raknet/StringTable.cpp \
    libs/raknet/StringCompressor.cpp \
    libs/raknet/StatisticsHistory.cpp \
    libs/raknet/SocketLayer.cpp \
    libs/raknet/SimpleMutex.cpp \
    libs/raknet/SignaledEvent.cpp \
    libs/raknet/SendToThread.cpp \
    libs/raknet/SecureHandshake.cpp \
    libs/raknet/RPC4Plugin.cpp \
    libs/raknet/Router2.cpp \
    libs/raknet/ReplicaManager3.cpp \
    libs/raknet/ReliabilityLayer.cpp \
    libs/raknet/RelayPlugin.cpp \
    libs/raknet/ReadyEvent.cpp \
    libs/raknet/RandSync.cpp \
    libs/raknet/Rand.cpp \
    libs/raknet/RakWString.cpp \
    libs/raknet/RakThread.cpp \
    libs/raknet/RakString.cpp \
    libs/raknet/RakSleep.cpp \
    libs/raknet/RakPeer.cpp \
    libs/raknet/RakNetTypes.cpp \
    libs/raknet/RakNetTransport2.cpp \
    libs/raknet/RakNetStatistics.cpp \
    libs/raknet/RakNetSocket2.cpp \
    libs/raknet/RakNetSocket2_WindowsStore8.cpp \
    libs/raknet/RakNetSocket2_Windows_Linux.cpp \
    libs/raknet/RakNetSocket2_Windows_Linux_360.cpp \
    libs/raknet/RakNetSocket2_Vita.cpp \
    libs/raknet/RakNetSocket2_PS4.cpp \
    libs/raknet/RakNetSocket2_PS3_PS4.cpp \
    libs/raknet/RakNetSocket2_NativeClient.cpp \
    libs/raknet/RakNetSocket2_Berkley.cpp \
    libs/raknet/RakNetSocket2_Berkley_NativeClient.cpp \
    libs/raknet/RakNetSocket2_360_720.cpp \
    libs/raknet/RakNetSocket.cpp \
    libs/raknet/RakNetCommandParser.cpp \
    libs/raknet/RakMemoryOverride.cpp \
    libs/raknet/Rackspace.cpp \
    libs/raknet/PS4Includes.cpp \
    libs/raknet/PluginInterface2.cpp \
    libs/raknet/PacketOutputWindowLogger.cpp \
    libs/raknet/PacketLogger.cpp \
    libs/raknet/PacketizedTCP.cpp \
    libs/raknet/PacketFileLogger.cpp \
    libs/raknet/PacketConsoleLogger.cpp \
    libs/raknet/NetworkIDObject.cpp \
    libs/raknet/NetworkIDManager.cpp \
    libs/raknet/NatTypeDetectionServer.cpp \
    libs/raknet/NatTypeDetectionCommon.cpp \
    libs/raknet/NatTypeDetectionClient.cpp \
    libs/raknet/NatPunchthroughServer.cpp \
    libs/raknet/NatPunchthroughClient.cpp \
    libs/raknet/MessageFilter.cpp \
    libs/raknet/LogCommandParser.cpp \
    libs/raknet/LocklessTypes.cpp \
    libs/raknet/LinuxStrings.cpp \
    libs/raknet/Itoa.cpp \
    libs/raknet/IncrementalReadInterface.cpp \
    libs/raknet/HTTPConnection2.cpp \
    libs/raknet/HTTPConnection.cpp \
    libs/raknet/GridSectorizer.cpp \
    libs/raknet/gettimeofday.cpp \
    libs/raknet/GetTime.cpp \
    libs/raknet/Gets.cpp \
    libs/raknet/Getche.cpp \
    libs/raknet/FullyConnectedMesh2.cpp \
    libs/raknet/FormatString.cpp \
    libs/raknet/FileOperations.cpp \
    libs/raknet/FileListTransfer.cpp \
    libs/raknet/FileList.cpp \
    libs/raknet/EpochTimeToString.cpp \
    libs/raknet/EmailSender.cpp \
    libs/raknet/DynDNS.cpp \
    libs/raknet/DS_Table.cpp \
    libs/raknet/DS_HuffmanEncodingTree.cpp \
    libs/raknet/DS_ByteQueue.cpp \
    libs/raknet/DS_BytePool.cpp \
    libs/raknet/DR_SHA1.cpp \
    libs/raknet/DirectoryDeltaTransfer.cpp \
    libs/raknet/DataCompressor.cpp \
    libs/raknet/ConsoleServer.cpp \
    libs/raknet/ConnectionGraph2.cpp \
    libs/raknet/CommandParserInterface.cpp \
    libs/raknet/CloudServer.cpp \
    libs/raknet/CloudCommon.cpp \
    libs/raknet/CloudClient.cpp \
    libs/raknet/CheckSum.cpp \
    libs/raknet/CCRakNetUDT.cpp \
    libs/raknet/CCRakNetSlidingWindow.cpp \
    libs/raknet/BitStream.cpp \
    libs/raknet/Base64Encoder.cpp \
    libs/raknet/_FindFirst.cpp \
    src/engine/systems/networksystem.cpp \
    src/vulpecula/menuscreen.cpp \
    src/vulpecula/responders/clientresponse.cpp \
    src/vulpecula/responders/serverresponse.cpp \
    src/engine/components/networkcomponent.cpp \
    src/vulpecula/systems/ProgressTracker.cpp \
    src/vulpecula/systems/particlesys.cpp \
    src/Particles/OpenGLShape.cpp \
    src/Particles/ResourceLoader2.cpp \
    src/vulpecula/responders/WinResp.cpp \
    src/engine/ui/scripts/TimedFade.cpp \
    src/engine/ui/scripts/FullScreenQuadScript.cpp \
    src/vulpecula/responders/Standable.cpp \
    src/vulpecula/responders/LowpassResp.cpp \
    src/vulpecula/responders/waterresponse.cpp

HEADERS += \
    libs/glew-1.10.0/include/GL/glew.h \
    libs/stb/stb_rect_pack.h \
    libs/stb/stb_truetype.h \
    src/mainwindow.h \
    src/view.h \
    src/viewformat.h \
    src/engine/util/CommonIncludes.h \
    src/engine/graphics/ResourceLoader.h \
    src/engine/graphics/CylinderData.h \
    src/engine/graphics/SphereData.h \
    src/engine/graphics/FBO.h \
    src/engine/graphics/IBO.h \
    src/engine/graphics/Texture.h \
    src/engine/graphics/Texture1D.h \
    src/engine/graphics/Texture2D.h \
    src/engine/graphics/Texture3D.h \
    src/engine/graphics/VAO.h \
    src/engine/graphics/VBO.h \
    src/engine/graphics/VBOAttribMarker.h \
    src/engine/graphics/CubeData.h \
    src/engine/graphics/Font.h \
    src/engine/graphics/DepthBuffer.h \
    src/engine/graphics/RenderBuffer.h \
    src/engine/graphics/GraphicsDebug.h \
    src/engine/graphics/Shader.h \
    src/engine/graphics/ShaderAttribLocations.h \
    src/engine/graphics/Camera.h \
    src/engine/graphics/Graphics.h \
    src/engine/graphics/Shape.h \
    src/engine/graphics/Material.h \
    src/engine/graphics/Light.h \
    src/engine/graphics/Constants.h \
    src/engine/frame/Application.h \
    src/engine/frame/Screen.h \
    src/warmup1/WarmupApp.h \
    src/engine/input/InputManager.h \
    src/engine/objectManagement/GameWorld.h \
    src/engine/objectManagement/GameObject.h \
    src/engine/components/Component.h \
    src/engine/components/CCollider.h \
    src/engine/systems/System.h \
    src/engine/systems/CollisionSystem.h \
    src/engine/components/CTransform.h \
    src/warmup1/CustomComponents/CPhysics.h \
    src/engine/systems/RenderSystem.h \
    src/warmup1/CustomSystems/PhysicsSystem.h \
    src/engine/gen/SpacePartMap.h \
    src/dungeon_crawler/DungeonCrawlerApp.h \
    src/dungeon_crawler/GameplayScreen.h \
    src/dungeon_crawler/ChunkFactory.h \
    src/dungeon_crawler/ChunkLoader.h \
    src/engine/graphics/AABB.h \
    src/engine/components/volumes/CollisionVolume.h \
    src/engine/components/volumes/CollBox.h \
    src/engine/components/volumes/CollSphere.h \
    src/engine/components/volumes/CollCylinder.h \
    src/engine/components/responders/CollisionResponse.h \
    src/engine/components/CRenderable.h \
    src/engine/systems/CameraSystem.h \
    src/engine/components/CCamera.h \
    src/engine/systems/FPSCameraSystem.h \
    src/warmup1/CustomSystems/InputSystem.h \
    src/dungeon_crawler/MovementSys.h \
    src/engine/components/CInputReceiver.h \
    src/engine/components/volumes/CollComposite.h \
    src/engine/systems/RaycastSystem.h \
    src/engine/systems/AISystem.h \
    src/engine/components/AIComponent.h \
    src/engine/ai/BTNode.h \
    src/engine/ai/CompositeNode.h \
    src/engine/ai/Condition.h \
    src/engine/ai/Action.h \
    src/engine/ai/Sequence.h \
    src/engine/ai/Selector.h \
    src/engine/ai/AStarSearch.h \
    src/engine/ai/AStarNode.h \
    src/engine/components/CNavMesh.h \
    src/dungeon_crawler/NavmeshDrawer.h \
    src/dungeon_crawler/ai/RadiusCondition.h \
    src/engine/ai/Blackboard.h \
    src/dungeon_crawler/ai/MoveToPlayer.h \
    src/dungeon_crawler/ai/EnemyAISystem.h \
    src/dungeon_crawler/CChunkInfo.h \
    src/dungeon_crawler/ai/BackAway.h \
    src/dungeon_crawler/ai/Strafe.h \
    src/dungeon_crawler/ai/FaceAndShoot.h \
    src/dungeon_crawler/CHealth.h \
    src/dungeon_crawler/CooldownSys.h \
    src/dungeon_crawler/CCooldown.h \
    src/dungeon_crawler/ShootSystem.h \
    src/dungeon_crawler/CBullet.h \
    src/dungeon_crawler/PlayerCollision.h \
    src/engine/ai/RandomSelector.h \
    src/engine/ai/Inverter.h \
    src/engine/ai/RepeatUntilDone.h \
    src/engine/ui/UIObject.h \
    src/engine/ui/UIComponent.h \
    src/engine/ui/UIDrawer.h \
    src/engine/ui/UIRenderable.h \
    src/engine/ui/UITransform.h \
    src/engine/ui/UIText.h \
    src/engine/ui/UISystem.h \
    src/engine/ui/TextDrawer.h \
    src/engine/ui/UIScriptComp.h \
    src/dungeon_crawler/HealthScript.h \
    src/engine/ui/UIScriptSys.h \
    src/dungeon_crawler/LandingScreen.h \
    src/engine/ui/UIHitbox.h \
    src/engine/ui/ClickResponse.h \
    src/engine/ui/UIClickSystem.h \
    src/dungeon_crawler/StartButtonResp.h \
    src/dungeon_crawler/PauseScreen.h \
    src/engine/components/CAudioSource.h \
    src/engine/systems/AudioSystem.h \
    src/vulpecula/MainScreen.h \
    src/engine/systems/ThirdPersonCamSys.h \
    src/vulpecula/VulpeculaApp.h \
    src/warmup1/WarmupScreen.h \
    src/vulpecula/components/RandomAudioSource.h \
    src/vulpecula/systems/RandomAudioManager.h \
    src/vulpecula/systems/PlayerMovementSys.h \
    src/vulpecula/responders/GuitarZoneResp.h \
    src/vulpecula/responders/GuitarStarResp.h \
    src/engine/graphics/RiggedMesh.h \
    src/engine/systems/AnimationSystem.h \
    src/engine/components/CAnimatedMesh.h \
    src/engine/graphics/Animation.h \
    src/engine/graphics/Armature.h \
    src/engine/graphics/TextureCube.h \
    src/engine/components/cmeshcol.h \
    src/engine/components/ColEllipsoid.h \
    libs/raknet/XBox360Includes.h \
    libs/raknet/WSAStartupSingleton.h \
    libs/raknet/WindowsIncludes.h \
    libs/raknet/VitaIncludes.h \
    libs/raknet/VariadicSQLParser.h \
    libs/raknet/VariableListDeltaTracker.h \
    libs/raknet/VariableDeltaSerializer.h \
    libs/raknet/UDPProxyServer.h \
    libs/raknet/UDPProxyCoordinator.h \
    libs/raknet/UDPProxyCommon.h \
    libs/raknet/UDPProxyClient.h \
    libs/raknet/UDPForwarder.h \
    libs/raknet/TwoWayAuthentication.h \
    libs/raknet/TransportInterface.h \
    libs/raknet/ThreadsafePacketLogger.h \
    libs/raknet/ThreadPool.h \
    libs/raknet/TelnetTransport.h \
    libs/raknet/TeamManager.h \
    libs/raknet/TeamBalancer.h \
    libs/raknet/TCPInterface.h \
    libs/raknet/TableSerializer.h \
    libs/raknet/SuperFastHash.h \
    libs/raknet/StringTable.h \
    libs/raknet/StringCompressor.h \
    libs/raknet/StatisticsHistory.h \
    libs/raknet/SocketLayer.h \
    libs/raknet/SocketIncludes.h \
    libs/raknet/SocketDefines.h \
    libs/raknet/SingleProducerConsumer.h \
    libs/raknet/SimpleTCPServer.h \
    libs/raknet/SimpleMutex.h \
    libs/raknet/SignaledEvent.h \
    libs/raknet/SendToThread.h \
    libs/raknet/SecureHandshake.h \
    libs/raknet/RPC4Plugin.h \
    libs/raknet/Router2.h \
    libs/raknet/ReplicaManager3.h \
    libs/raknet/ReplicaEnums.h \
    libs/raknet/ReliabilityLayer.h \
    libs/raknet/RelayPlugin.h \
    libs/raknet/RefCountedObj.h \
    libs/raknet/ReadyEvent.h \
    libs/raknet/RandSync.h \
    libs/raknet/Rand.h \
    libs/raknet/RakWString.h \
    libs/raknet/RakThread.h \
    libs/raknet/RakString.h \
    libs/raknet/RakSleep.h \
    libs/raknet/RakPeerInterface.h \
    libs/raknet/RakPeer.h \
    libs/raknet/RakNetVersion.h \
    libs/raknet/RakNetTypes.h \
    libs/raknet/RakNetTransport2.h \
    libs/raknet/RakNetTime.h \
    libs/raknet/RakNetStatistics.h \
    libs/raknet/RakNetSocket2.h \
    libs/raknet/RakNetSocket.h \
    libs/raknet/RakNetSmartPtr.h \
    libs/raknet/RakNetDefinesOverrides.h \
    libs/raknet/RakNetDefines.h \
    libs/raknet/RakNetCommandParser.h \
    libs/raknet/RakMemoryOverride.h \
    libs/raknet/RakAssert.h \
    libs/raknet/RakAlloca.h \
    libs/raknet/Rackspace.h \
    libs/raknet/PS4Includes.h \
    libs/raknet/PS3Includes.h \
    libs/raknet/PluginInterface2.h \
    libs/raknet/PacketPriority.h \
    libs/raknet/PacketPool.h \
    libs/raknet/PacketOutputWindowLogger.h \
    libs/raknet/PacketLogger.h \
    libs/raknet/PacketizedTCP.h \
    libs/raknet/PacketFileLogger.h \
    libs/raknet/PacketConsoleLogger.h \
    libs/raknet/NetworkIDObject.h \
    libs/raknet/NetworkIDManager.h \
    libs/raknet/NatTypeDetectionServer.h \
    libs/raknet/NatTypeDetectionCommon.h \
    libs/raknet/NatTypeDetectionClient.h \
    libs/raknet/NatPunchthroughServer.h \
    libs/raknet/NatPunchthroughClient.h \
    libs/raknet/NativeTypes.h \
    libs/raknet/NativeFeatureIncludesOverrides.h \
    libs/raknet/NativeFeatureIncludes.h \
    libs/raknet/MTUSize.h \
    libs/raknet/MessageIdentifiers.h \
    libs/raknet/MessageFilter.h \
    libs/raknet/LogCommandParser.h \
    libs/raknet/LocklessTypes.h \
    libs/raknet/LinuxStrings.h \
    libs/raknet/Kbhit.h \
    libs/raknet/Itoa.h \
    libs/raknet/InternalPacket.h \
    libs/raknet/IncrementalReadInterface.h \
    libs/raknet/HTTPConnection2.h \
    libs/raknet/HTTPConnection.h \
    libs/raknet/GridSectorizer.h \
    libs/raknet/gettimeofday.h \
    libs/raknet/GetTime.h \
    libs/raknet/Gets.h \
    libs/raknet/Getche.h \
    libs/raknet/FullyConnectedMesh2.h \
    libs/raknet/FormatString.h \
    libs/raknet/FileOperations.h \
    libs/raknet/FileListTransferCBInterface.h \
    libs/raknet/FileListTransfer.h \
    libs/raknet/FileListNodeContext.h \
    libs/raknet/FileList.h \
    libs/raknet/Export.h \
    libs/raknet/EpochTimeToString.h \
    libs/raknet/EmptyHeader.h \
    libs/raknet/EmailSender.h \
    libs/raknet/DynDNS.h \
    libs/raknet/DS_WeightedGraph.h \
    libs/raknet/DS_Tree.h \
    libs/raknet/DS_ThreadsafeAllocatingQueue.h \
    libs/raknet/DS_Table.h \
    libs/raknet/DS_RangeList.h \
    libs/raknet/DS_QueueLinkedList.h \
    libs/raknet/DS_Queue.h \
    libs/raknet/DS_OrderedList.h \
    libs/raknet/DS_OrderedChannelHeap.h \
    libs/raknet/DS_Multilist.h \
    libs/raknet/DS_MemoryPool.h \
    libs/raknet/DS_Map.h \
    libs/raknet/DS_List.h \
    libs/raknet/DS_LinkedList.h \
    libs/raknet/DS_HuffmanEncodingTreeNode.h \
    libs/raknet/DS_HuffmanEncodingTreeFactory.h \
    libs/raknet/DS_HuffmanEncodingTree.h \
    libs/raknet/DS_Heap.h \
    libs/raknet/DS_Hash.h \
    libs/raknet/DS_ByteQueue.h \
    libs/raknet/DS_BytePool.h \
    libs/raknet/DS_BPlusTree.h \
    libs/raknet/DS_BinarySearchTree.h \
    libs/raknet/DR_SHA1.h \
    libs/raknet/DirectoryDeltaTransfer.h \
    libs/raknet/DataCompressor.h \
    libs/raknet/ConsoleServer.h \
    libs/raknet/ConnectionGraph2.h \
    libs/raknet/CommandParserInterface.h \
    libs/raknet/CloudServer.h \
    libs/raknet/CloudCommon.h \
    libs/raknet/CloudClient.h \
    libs/raknet/CheckSum.h \
    libs/raknet/CCRakNetUDT.h \
    libs/raknet/CCRakNetSlidingWindow.h \
    libs/raknet/BitStream.h \
    libs/raknet/Base64Encoder.h \
    libs/raknet/AutopatcherRepositoryInterface.h \
    libs/raknet/AutopatcherPatchContext.h \
    libs/raknet/_FindFirst.h \
    src/engine/systems/networksystem.h \
    src/vulpecula/menuscreen.h \
    src/vulpecula/responders/clientresponse.h \
    src/vulpecula/responders/serverresponse.h \
    src/engine/components/networkcomponent.h \
    src/vulpecula/systems/ProgressTracker.h \
    src/vulpecula/systems/particlesys.h \
    src/Particles/OpenGLShape.h \
    src/Particles/ResourceLoader2.h \
    src/vulpecula/responders/WinResp.h \
    src/engine/ui/scripts/TimedFade.h \
    src/engine/ui/scripts/FullScreenQuadScript.h \
    src/engine/ui/scripts/Script.h \
    src/vulpecula/responders/Standable.h \
    src/vulpecula/responders/LowpassResp.h \
    src/vulpecula/responders/waterresponse.h

FORMS += src/mainwindow.ui

RESOURCES += \
    res/shaders/shaders.qrc \
    res/fonts/fonts.qrc \
    res/images/images.qrc \
    res/models/models.qrc \
    res/sounds/sounds.qrc

OTHER_FILES += \
    res/images/grass.png \
    libs/raknet/RakNet.vcproj \
    libs/raknet/RakNet_vc9.vcproj \
    libs/raknet/RakNet_vc8.vcproj \
    libs/raknet/CMakeLists.txt

DISTFILES += \
    res/shaders/shader.vert \
    res/shaders/shader.frag

INCLUDEPATH += src libs glm libs/glew-1.10.0/include
DEPENDPATH += src libs glm libs/glew-1.10.0/include

# Don't add the -pg flag unless you know what you are doing. It makes QThreadPool freeze on Mac OS X
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON += -Waddress -Warray-bounds -Wc++0x-compat -Wchar-subscripts -Wformat\
                          -Wmain -Wmissing-braces -Wparentheses -Wreorder -Wreturn-type \
                          -Wsequence-point -Wsign-compare -Wstrict-overflow=1 -Wswitch \
                          -Wtrigraphs -Wuninitialized -Wunused-label -Wunused-variable \
                          -Wvolatile-register-var -Wno-extra

QMAKE_CXXFLAGS += -g
