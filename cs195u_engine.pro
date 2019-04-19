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
    src/engine/ui/FullScreenQuadScript.cpp \
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
    src/engine/graphics/TextureCube.cpp

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
    src/engine/ui/Script.h \
    src/dungeon_crawler/HealthScript.h \
    src/engine/ui/UIScriptSys.h \
    src/dungeon_crawler/LandingScreen.h \
    src/engine/ui/UIHitbox.h \
    src/engine/ui/ClickResponse.h \
    src/engine/ui/UIClickSystem.h \
    src/dungeon_crawler/StartButtonResp.h \
    src/dungeon_crawler/PauseScreen.h \
    src/engine/ui/FullScreenQuadScript.h \
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
    src/engine/graphics/TextureCube.h

FORMS += src/mainwindow.ui

RESOURCES += \
    res/shaders/shaders.qrc \
    res/fonts/fonts.qrc \
    res/images/images.qrc \
    res/models/models.qrc \
    res/sounds/sounds.qrc

OTHER_FILES += \
    res/images/grass.png

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
