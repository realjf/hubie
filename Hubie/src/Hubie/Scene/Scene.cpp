#include "Precompiled.h"
#include "Scene.h"
#include "Hubie/Core/OS/Input.h"
#include "Hubie/Core/Application.h"
#include "Hubie/Graphics/API/GraphicsContext.h"
#include "Hubie/Graphics/Renderers/RenderGraph.h"
#include "Hubie/Graphics/Camera/Camera.h"
#include "Hubie/Graphics/Sprite.h"
#include "Hubie/Graphics/AnimatedSprite.h"
#include "Hubie/Utilities/TimeStep.h"
#include "Hubie/Audio/AudioManager.h"
#include "Hubie/Physics/HubiePhysicsEngine/SortAndSweepBroadphase.h"
#include "Hubie/Physics/HubiePhysicsEngine/BruteForceBroadphase.h"
#include "Hubie/Physics/HubiePhysicsEngine/OctreeBroadphase.h"
#include "Hubie/Physics/HubiePhysicsEngine/HubiePhysicsEngine.h"
#include "Hubie/Physics/HubiePhysicsEngine/SphereCollisionShape.h"
#include "Hubie/Physics/HubiePhysicsEngine/CuboidCollisionShape.h"
#include "Hubie/Physics/HubiePhysicsEngine/PyramidCollisionShape.h"

#include "Hubie/Events/Event.h"
#include "Hubie/Events/ApplicationEvent.h"

#include "Hubie/Maths/Transform.h"
#include "Hubie/Core/OS/FileSystem.h"
#include "Hubie/Scene/Component/Components.h"
#include "Hubie/Scripting/Lua/LuaScriptComponent.h"
#include "Hubie/Scripting/Lua/LuaManager.h"
#include "Hubie/Graphics/MeshFactory.h"
#include "Hubie/Graphics/Light.h"
#include "Hubie/Graphics/Model.h"
#include "Hubie/Graphics/Environment.h"
#include "Hubie/Scene/EntityManager.h"
#include "Hubie/Scene/Component/SoundComponent.h"
#include "SceneGraph.h"

#include <cereal/types/polymorphic.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <entt/entity/registry.hpp>
#include <sol/sol.hpp>

namespace Hubie
{
    Scene::Scene(const std::string& friendly_name)
        : m_SceneName(friendly_name)
        , m_ScreenWidth(0)
        , m_ScreenHeight(0)
    {
        m_EntityManager = CreateUniqueRef<EntityManager>(this);
        m_EntityManager->AddDependency<Physics3DComponent, Maths::Transform>();
        m_EntityManager->AddDependency<Physics2DComponent, Maths::Transform>();
        m_EntityManager->AddDependency<Camera, Maths::Transform>();
        m_EntityManager->AddDependency<Graphics::Model, Maths::Transform>();
        m_EntityManager->AddDependency<Graphics::Light, Maths::Transform>();
        m_EntityManager->AddDependency<Graphics::Sprite, Maths::Transform>();
        m_EntityManager->AddDependency<Graphics::AnimatedSprite, Maths::Transform>();

        m_SceneGraph = CreateUniqueRef<SceneGraph>();
        m_SceneGraph->Init(m_EntityManager->GetRegistry());
    }

    Scene::~Scene()
    {
        m_EntityManager->Clear();
    }

    entt::registry& Scene::GetRegistry()
    {
        return m_EntityManager->GetRegistry();
    }

    void Scene::OnInit()
    {
        HB_PROFILE_FUNCTION();
        LuaManager::Get().GetState().set("registry", &m_EntityManager->GetRegistry());
        LuaManager::Get().GetState().set("scene", this);

        //Default physics setup
        Application::Get().GetSystem<HubiePhysicsEngine>()->SetDampingFactor(0.999f);
        Application::Get().GetSystem<HubiePhysicsEngine>()->SetIntegrationType(IntegrationType::RUNGE_KUTTA_4);
        Application::Get().GetSystem<HubiePhysicsEngine>()->SetBroadphase(Hubie::CreateRef<OctreeBroadphase>(5, 5, Hubie::CreateRef<SortAndSweepBroadphase>()));

        LuaManager::Get().OnInit(this);
    }

    void Scene::OnCleanupScene()
    {
        HB_PROFILE_FUNCTION();
        DeleteAllGameObjects();

        LuaManager::Get().GetState().collect_garbage();

        Application::Get().GetRenderGraph()->Reset();

        auto audioManager = Application::Get().GetSystem<AudioManager>();
        if(audioManager)
        {
            audioManager->ClearNodes();
        }
    };

    void Scene::DeleteAllGameObjects()
    {
        HB_PROFILE_FUNCTION();
        m_EntityManager->Clear();
    }

    void Scene::OnUpdate(const TimeStep& timeStep)
    {
        HB_PROFILE_FUNCTION();
        const Maths::Vector2 mousePos = Input::Get().GetMousePosition();

        auto defaultCameraControllerView = m_EntityManager->GetEntitiesWithType<DefaultCameraController>();

        if(!defaultCameraControllerView.Empty())
        {
            auto& cameraController = defaultCameraControllerView.Front().GetComponent<DefaultCameraController>();
            auto trans = defaultCameraControllerView.Front().TryGetComponent<Maths::Transform>();
            if(Application::Get().GetSceneActive() && trans && cameraController.GetController())
            {
                cameraController.GetController()->HandleMouse(*trans, timeStep.GetSeconds(), mousePos.x, mousePos.y);
                cameraController.GetController()->HandleKeyboard(*trans, timeStep.GetSeconds());
            }
        }

        m_SceneGraph->Update(m_EntityManager->GetRegistry());

        auto animatedSpriteView = m_EntityManager->GetEntitiesWithType<Graphics::AnimatedSprite>();

        for(auto entity : animatedSpriteView)
        {
            auto& animSprite = entity.GetComponent<Graphics::AnimatedSprite>();
            animSprite.OnUpdate(timeStep.GetSeconds());
        }
    }

    void Scene::OnEvent(Event& e)
    {
        HB_PROFILE_FUNCTION();
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Scene::OnWindowResize));
    }

    bool Scene::OnWindowResize(WindowResizeEvent& e)
    {
        HB_PROFILE_FUNCTION();
        if(!Application::Get().GetSceneActive())
            return false;

        auto cameraView = m_EntityManager->GetRegistry().view<Camera>();
        if(!cameraView.empty())
        {
            m_EntityManager->GetRegistry().get<Camera>(cameraView.front()).SetAspectRatio(static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight()));
        }

        return false;
    }

    void Scene::SetScreenSize(uint32_t width, uint32_t height)
    {
        m_ScreenWidth = width;
        m_ScreenHeight = height;

        auto cameraView = m_EntityManager->GetRegistry().view<Camera>();
        if(!cameraView.empty())
        {
            m_EntityManager->GetRegistry().get<Camera>(cameraView.front()).SetAspectRatio(static_cast<float>(m_ScreenWidth) / static_cast<float>(m_ScreenHeight));
        }
    }

#define ALL_COMPONENTSV1 Maths::Transform, NameComponent, ActiveComponent, Hierarchy, Camera, LuaScriptComponent, Graphics::Model, Graphics::Light, Physics3DComponent, Graphics::Environment, Graphics::Sprite, Physics2DComponent, DefaultCameraController

#define ALL_COMPONENTSV2 ALL_COMPONENTSV1, Graphics::AnimatedSprite
#define ALL_COMPONENTSV3 ALL_COMPONENTSV2, SoundComponent

    void Scene::Serialise(const std::string& filePath, bool binary)
    {
        HB_PROFILE_FUNCTION();
        HB_INFO("Scene saved - {0}", filePath);
        std::string path = filePath;
        path += StringUtilities::RemoveSpaces(m_SceneName);
        if(binary)
        {
            path += std::string(".bin");

            std::ofstream file(path, std::ios::binary);

            {
                // output finishes flushing its contents when it goes out of scope
                cereal::BinaryOutputArchive output { file };
                output(*this);
                entt::snapshot { m_EntityManager->GetRegistry() }.entities(output).component<ALL_COMPONENTSV3>(output);
            }
            file.close();
        }
        else
        {
            std::stringstream storage;
            path += std::string(".lsn");

            {
                // output finishes flushing its contents when it goes out of scope
                cereal::JSONOutputArchive output { storage };
                output(*this);
                entt::snapshot { m_EntityManager->GetRegistry() }.entities(output).component<ALL_COMPONENTSV3>(output);
            }
            FileSystem::WriteTextFile(path, storage.str());
        }
    }

    void Scene::Deserialise(const std::string& filePath, bool binary)
    {
        HB_PROFILE_FUNCTION();
        m_EntityManager->Clear();
        m_SceneGraph->DisableOnConstruct(true, m_EntityManager->GetRegistry());
        std::string path = filePath;
        path += StringUtilities::RemoveSpaces(m_SceneName);

        if(binary)
        {
            path += std::string(".bin");

            if(!FileSystem::FileExists(path))
            {
                HB_ERROR("No saved scene file found {0}", path);
                return;
            }

            std::ifstream file(path, std::ios::binary);
            cereal::BinaryInputArchive input(file);
            input(*this);
            if(m_SceneSerialisationVersion < 2)
                entt::snapshot_loader { m_EntityManager->GetRegistry() }.entities(input).component<ALL_COMPONENTSV1>(input);
            else if(m_SceneSerialisationVersion == 3)
                entt::snapshot_loader { m_EntityManager->GetRegistry() }.entities(input).component<ALL_COMPONENTSV2>(input);
            else if(m_SceneSerialisationVersion == 4)
                entt::snapshot_loader { m_EntityManager->GetRegistry() }.entities(input).component<ALL_COMPONENTSV3>(input);
        }
        else
        {
            path += std::string(".lsn");

            if(!FileSystem::FileExists(path))
            {
                HB_ERROR("No saved scene file found {0}", path);
                return;
            }
            std::string data = FileSystem::ReadTextFile(path);
            std::istringstream istr;
            istr.str(data);
            cereal::JSONInputArchive input(istr);
            input(*this);

            if(m_SceneSerialisationVersion < 2)
                entt::snapshot_loader { m_EntityManager->GetRegistry() }.entities(input).component<ALL_COMPONENTSV1>(input);
            else if(m_SceneSerialisationVersion == 3)
                entt::snapshot_loader { m_EntityManager->GetRegistry() }.entities(input).component<ALL_COMPONENTSV2>(input);
            else if(m_SceneSerialisationVersion == 4)
                entt::snapshot_loader { m_EntityManager->GetRegistry() }.entities(input).component<ALL_COMPONENTSV3>(input);
        }

        m_SceneGraph->DisableOnConstruct(false, m_EntityManager->GetRegistry());
    }

    void Scene::UpdateSceneGraph()
    {
        HB_PROFILE_FUNCTION();
        m_SceneGraph->Update(m_EntityManager->GetRegistry());
    }

    template <typename T>
    static void CopyComponentIfExists(entt::entity dst, entt::entity src, entt::registry& registry)
    {
        if(registry.has<T>(src))
        {
            auto& srcComponent = registry.get<T>(src);
            registry.emplace_or_replace<T>(dst, srcComponent);
        }
    }

    Entity Scene::CreateEntity()
    {
        return m_EntityManager->Create();
    }

    Entity Scene::CreateEntity(const std::string& name)
    {
        HB_PROFILE_FUNCTION();
        return m_EntityManager->Create(name);
    }

    void Scene::DuplicateEntity(Entity entity)
    {
        HB_PROFILE_FUNCTION();
        Entity newEntity = m_EntityManager->Create();

        CopyComponentIfExists<Maths::Transform>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<Graphics::Model>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<LuaScriptComponent>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<Camera>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<Graphics::Sprite>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<Physics2DComponent>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<Physics3DComponent>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<Graphics::Light>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<SoundComponent>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<Graphics::Environment>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
    }

    void Scene::DuplicateEntity(Entity entity, Entity parent)
    {
        HB_PROFILE_FUNCTION();
        Entity newEntity = m_EntityManager->Create();

        CopyComponentIfExists<Maths::Transform>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<Graphics::Model>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<LuaScriptComponent>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<Camera>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<Graphics::Sprite>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<Physics2DComponent>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<Physics3DComponent>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<Graphics::Light>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<SoundComponent>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());
        CopyComponentIfExists<Graphics::Environment>(newEntity.GetHandle(), entity.GetHandle(), m_EntityManager->GetRegistry());

        if(parent)
            newEntity.SetParent(parent);
    }
}
