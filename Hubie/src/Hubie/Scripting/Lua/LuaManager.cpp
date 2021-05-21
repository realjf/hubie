#include "Precompiled.h"
#include "LuaManager.h"
#include "Hubie/Maths/Transform.h"
#include "Hubie/Core/OS/Window.h"
#include "Hubie/Core/VFS.h"
#include "Hubie/Scene/Scene.h"
#include "Hubie/Core/Application.h"
#include "Hubie/Core/Engine.h"
#include "Hubie/Core/OS/Input.h"
#include "Hubie/Scene/SceneManager.h"
#include "LuaScriptComponent.h"
#include "Hubie/Scene/SceneGraph.h"
#include "Hubie/Graphics/Camera/ThirdPersonCamera.h"

#include "Hubie/Scene/Component/Components.h"
#include "Hubie/Graphics/Camera/Camera.h"
#include "Hubie/Graphics/Camera/Camera2D.h"

#include "Hubie/Graphics/Sprite.h"
#include "Hubie/Graphics/Light.h"
#include "Hubie/Graphics/API/Texture.h"
#include "Hubie/Graphics/Model.h"
#include "Hubie/Utilities/RandomNumberGenerator.h"
#include "Hubie/Scene/Entity.h"
#include "Hubie/Scene/EntityManager.h"
#include "Hubie/Scene/EntityFactory.h"
#include "Hubie/Physics/HubiePhysicsEngine/HubiePhysicsEngine.h"

#include "ImGuiLua.h"
#include "PhysicsLua.h"
#include "MathsLua.h"

#include <imgui/imgui.h>
#include <Tracy/TracyLua.hpp>

#ifdef CUSTOM_SMART_PTR
namespace sol
{
    template <typename T>
    struct unique_usertype_traits<Hubie::Ref<T>>
    {
        typedef T type;
        typedef Hubie::Ref<T> actual_type;
        static const bool value = true;

        static bool is_null(const actual_type& ptr)
        {
            return ptr == nullptr;
        }

        static type* get(const actual_type& ptr)
        {
            return ptr.get();
        }
    };

    template <typename T>
    struct unique_usertype_traits<Hubie::UniqueRef<T>>
    {
        typedef T type;
        typedef Hubie::UniqueRef<T> actual_type;
        static const bool value = true;

        static bool is_null(const actual_type& ptr)
        {
            return ptr == nullptr;
        }

        static type* get(const actual_type& ptr)
        {
            return ptr.get();
        }
    };

    template <typename T>
    struct unique_usertype_traits<Hubie::WeakRef<T>>
    {
        typedef T type;
        typedef Hubie::WeakRef<T> actual_type;
        static const bool value = true;

        static bool is_null(const actual_type& ptr)
        {
            return ptr == nullptr;
        }

        static type* get(const actual_type& ptr)
        {
            return ptr.get();
        }
    };
}

#endif

namespace Hubie
{
    LuaManager::LuaManager()
        : m_State(nullptr)
    {
    }

    void LuaManager::OnInit()
    {
        HB_PROFILE_FUNCTION();
        m_State.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table);
        tracy::LuaRegister(m_State.lua_state());

        BindAppLua(m_State);
        BindInputLua(m_State);
        BindMathsLua(m_State);
        BindImGuiLua(m_State);
        BindECSLua(m_State);
        BindLogLua(m_State);
        BindSceneLua(m_State);
        BindPhysicsLua(m_State);
    }

    LuaManager::~LuaManager()
    {
    }

    void LuaManager::OnInit(Scene* scene)
    {
        HB_PROFILE_FUNCTION();
        auto& registry = scene->GetRegistry();

        auto view = registry.view<LuaScriptComponent>();

        if(view.empty())
            return;

        for(auto entity : view)
        {
            auto& luaScript = registry.get<LuaScriptComponent>(entity);
            luaScript.OnInit();
        }
    }

    void LuaManager::OnUpdate(Scene* scene)
    {
        HB_PROFILE_FUNCTION();
        auto& registry = scene->GetRegistry();

        auto view = registry.view<LuaScriptComponent>();

        if(view.empty())
            return;

        float dt = Engine::Get().GetTimeStep().GetElapsedMillis();

        for(auto entity : view)
        {
            auto& luaScript = registry.get<LuaScriptComponent>(entity);
            luaScript.OnUpdate(dt);
        }
    }

    entt::entity GetEntityByName(entt::registry& registry, const std::string& name)
    {
        HB_PROFILE_FUNCTION();
        entt::entity e = entt::null;
        registry.view<NameComponent>().each([&](const entt::entity& entity, const NameComponent& component)
            {
                if(name == component.name)
                {
                    e = entity;
                }
            });

        return e;
    }

    void LuaManager::BindLogLua(sol::state& state)
    {
        HB_PROFILE_FUNCTION();
        auto log = state.create_table("Log");

        log.set_function("Trace", [&](sol::this_state s, std::string_view message)
            { HB_TRACE(message); });

        log.set_function("Info", [&](sol::this_state s, std::string_view message)
            { HB_TRACE(message); });

        log.set_function("Warn", [&](sol::this_state s, std::string_view message)
            { HB_WARN(message); });

        log.set_function("Error", [&](sol::this_state s, std::string_view message)
            { HB_ERROR(message); });

        log.set_function("Critical", [&](sol::this_state s, std::string_view message)
            { HB_CRITICAL(message); });
    }

    void LuaManager::BindInputLua(sol::state& state)
    {
        HB_PROFILE_FUNCTION();
        auto input = state["Input"].get_or_create<sol::table>();

        input.set_function("GetKeyPressed", [](Hubie::InputCode::Key key) -> bool
            { return Input::Get().GetKeyPressed(key); });

        input.set_function("GetKeyHeld", [](Hubie::InputCode::Key key) -> bool
            { return Input::Get().GetKeyHeld(key); });

        input.set_function("GetMouseClicked", [](Hubie::InputCode::MouseKey key) -> bool
            { return Input::Get().GetMouseClicked(key); });

        input.set_function("GetMouseHeld", [](Hubie::InputCode::MouseKey key) -> bool
            { return Input::Get().GetMouseHeld(key); });

        input.set_function("GetMousePosition", []() -> Maths::Vector2
            { return Input::Get().GetMousePosition(); });

        input.set_function("GetScrollOffset", []() -> float
            { return Input::Get().GetScrollOffset(); });

        std::initializer_list<std::pair<sol::string_view, Hubie::InputCode::Key>> keyItems = {
            { "A", Hubie::InputCode::Key::A },
            { "B", Hubie::InputCode::Key::B },
            { "C", Hubie::InputCode::Key::C },
            { "D", Hubie::InputCode::Key::D },
            { "E", Hubie::InputCode::Key::E },
            { "F", Hubie::InputCode::Key::F },
            { "H", Hubie::InputCode::Key::G },
            { "G", Hubie::InputCode::Key::H },
            { "I", Hubie::InputCode::Key::I },
            { "J", Hubie::InputCode::Key::J },
            { "K", Hubie::InputCode::Key::K },
            { "L", Hubie::InputCode::Key::L },
            { "M", Hubie::InputCode::Key::M },
            { "N", Hubie::InputCode::Key::N },
            { "O", Hubie::InputCode::Key::O },
            { "P", Hubie::InputCode::Key::P },
            { "Q", Hubie::InputCode::Key::Q },
            { "R", Hubie::InputCode::Key::R },
            { "S", Hubie::InputCode::Key::S },
            { "T", Hubie::InputCode::Key::T },
            { "U", Hubie::InputCode::Key::U },
            { "V", Hubie::InputCode::Key::V },
            { "W", Hubie::InputCode::Key::W },
            { "X", Hubie::InputCode::Key::X },
            { "Y", Hubie::InputCode::Key::Y },
            { "Z", Hubie::InputCode::Key::Z },
            //{ "UNKOWN", Hubie::InputCode::Key::Unknown },
            { "Space", Hubie::InputCode::Key::Space },
            { "Escape", Hubie::InputCode::Key::Escape },
            { "APOSTROPHE", Hubie::InputCode::Key::Apostrophe },
            { "Comma", Hubie::InputCode::Key::Comma },
            { "MINUS", Hubie::InputCode::Key::Minus },
            { "PERIOD", Hubie::InputCode::Key::Period },
            { "SLASH", Hubie::InputCode::Key::Slash },
            { "SEMICOLON", Hubie::InputCode::Key::Semicolon },
            { "EQUAL", Hubie::InputCode::Key::Equal },
            { "LEFT_BRACKET", Hubie::InputCode::Key::LeftBracket },
            { "BACKSLASH", Hubie::InputCode::Key::Backslash },
            { "RIGHT_BRACKET", Hubie::InputCode::Key::RightBracket },
            //{ "BACK_TICK", Hubie::InputCode::Key::BackTick },
            { "Enter", Hubie::InputCode::Key::Enter },
            { "Tab", Hubie::InputCode::Key::Tab },
            { "Backspace", Hubie::InputCode::Key::Backspace },
            { "Insert", Hubie::InputCode::Key::Insert },
            { "Delete", Hubie::InputCode::Key::Delete },
            { "Right", Hubie::InputCode::Key::Right },
            { "Left", Hubie::InputCode::Key::Left },
            { "Down", Hubie::InputCode::Key::Down },
            { "Up", Hubie::InputCode::Key::Up },
            { "PageUp", Hubie::InputCode::Key::PageUp },
            { "PageDown", Hubie::InputCode::Key::PageDown },
            { "Home", Hubie::InputCode::Key::Home },
            { "End", Hubie::InputCode::Key::End },
            { "CAPS_LOCK", Hubie::InputCode::Key::CapsLock },
            { "SCROLL_LOCK", Hubie::InputCode::Key::ScrollLock },
            { "NumLock", Hubie::InputCode::Key::NumLock },
            { "PrintScreen", Hubie::InputCode::Key::PrintScreen },
            { "Pasue", Hubie::InputCode::Key::Pause },
            { "LeftShift", Hubie::InputCode::Key::LeftShift },
            { "LeftControl", Hubie::InputCode::Key::LeftControl },
            { "LEFT_ALT", Hubie::InputCode::Key::LeftAlt },
            { "LEFT_SUPER", Hubie::InputCode::Key::LeftSuper },
            { "RightShift", Hubie::InputCode::Key::RightShift },
            { "RightControl", Hubie::InputCode::Key::RightControl },
            { "RIGHT_ALT", Hubie::InputCode::Key::RightAlt },
            { "RIGHT_SUPER", Hubie::InputCode::Key::RightSuper },
            { "Menu", Hubie::InputCode::Key::Menu },
            { "F1", Hubie::InputCode::Key::F1 },
            { "F2", Hubie::InputCode::Key::F2 },
            { "F3", Hubie::InputCode::Key::F3 },
            { "F4", Hubie::InputCode::Key::F4 },
            { "F5", Hubie::InputCode::Key::F5 },
            { "F6", Hubie::InputCode::Key::F6 },
            { "F7", Hubie::InputCode::Key::F7 },
            { "F8", Hubie::InputCode::Key::F8 },
            { "F9", Hubie::InputCode::Key::F9 },
            { "F10", Hubie::InputCode::Key::F10 },
            { "F11", Hubie::InputCode::Key::F11 },
            { "F12", Hubie::InputCode::Key::F12 },
            { "Keypad0", Hubie::InputCode::Key::D0 },
            { "Keypad1", Hubie::InputCode::Key::D1 },
            { "Keypad2", Hubie::InputCode::Key::D2 },
            { "Keypad3", Hubie::InputCode::Key::D3 },
            { "Keypad4", Hubie::InputCode::Key::D4 },
            { "Keypad5", Hubie::InputCode::Key::D5 },
            { "Keypad6", Hubie::InputCode::Key::D6 },
            { "Keypad7", Hubie::InputCode::Key::D7 },
            { "Keypad8", Hubie::InputCode::Key::D8 },
            { "Keypad9", Hubie::InputCode::Key::D9 },
            { "Decimal", Hubie::InputCode::Key::Period },
            { "Divide", Hubie::InputCode::Key::Slash },
            { "Multiply", Hubie::InputCode::Key::KPMultiply },
            { "Subtract", Hubie::InputCode::Key::Minus },
            { "Add", Hubie::InputCode::Key::KPAdd },
            { "KP_EQUAL", Hubie::InputCode::Key::KPEqual }
        };
        state.new_enum<Hubie::InputCode::Key, false>("Key", keyItems); // false makes it read/write in Lua, but its faster

        std::initializer_list<std::pair<sol::string_view, Hubie::InputCode::MouseKey>> mouseItems = {
            { "Left", Hubie::InputCode::MouseKey::ButtonLeft },
            { "Right", Hubie::InputCode::MouseKey::ButtonRight },
            { "Middle", Hubie::InputCode::MouseKey::ButtonMiddle },
        };
        state.new_enum<Hubie::InputCode::MouseKey, false>("MouseButton", mouseItems);
    }

    Ref<Graphics::Texture2D> LoadTexture(const std::string& name, const std::string& path)
    {
        HB_PROFILE_FUNCTION();
        return Ref<Graphics::Texture2D>(Graphics::Texture2D::CreateFromFile(name, path));
    }

    Ref<Graphics::Texture2D> LoadTextureWithParams(const std::string& name, const std::string& path, Hubie::Graphics::TextureFilter filter, Hubie::Graphics::TextureWrap wrapMode)
    {
        HB_PROFILE_FUNCTION();
        return Ref<Graphics::Texture2D>(Graphics::Texture2D::CreateFromFile(name, path, Graphics::TextureParameters(filter, filter, wrapMode)));
    }

    void LuaManager::BindECSLua(sol::state& state)
    {
        HB_PROFILE_FUNCTION();

        sol::usertype<entt::registry> enttRegistry = state.new_usertype<entt::registry>("enttRegistry");

        sol::usertype<Entity> entityType = state.new_usertype<Entity>("Entity", sol::constructors<sol::types<entt::entity, Scene*>>());
        sol::usertype<EntityManager> entityManagerType = state.new_usertype<EntityManager>("EntityManager");
        entityManagerType.set_function("Create", static_cast<Entity (EntityManager::*)()>(&EntityManager::Create));
        entityManagerType.set_function("GetRegistry", &EntityManager::GetRegistry);

        entityType.set_function("Valid", &Entity::Valid);
        entityType.set_function("Destroy", &Entity::Destroy);
        entityType.set_function("SetParent", &Entity::SetParent);
        entityType.set_function("GetParent", &Entity::GetParent);
        entityType.set_function("IsParent", &Entity::IsParent);
        entityType.set_function("GetChildren", &Entity::GetChildren);
        entityType.set_function("SetActive", &Entity::SetActive);
        entityType.set_function("Active", &Entity::Active);

        state.set_function("GetEntityByName", &GetEntityByName);

        state.set_function("AddPyramidEntity", &EntityFactory::AddPyramid);
        state.set_function("AddSphereEntity", &EntityFactory::AddSphere);
        state.set_function("AddLightCubeEntity", &EntityFactory::AddLightCube);

        sol::usertype<NameComponent> nameComponent_type = state.new_usertype<NameComponent>("NameComponent");
        nameComponent_type["name"] = &NameComponent::name;
        REGISTER_COMPONENT_WITH_ECS(state, NameComponent, static_cast<NameComponent& (Entity::*)()>(&Entity::AddComponent<NameComponent>));

        sol::usertype<LuaScriptComponent> script_type = state.new_usertype<LuaScriptComponent>("LuaScriptComponent", sol::constructors<sol::types<std::string, Scene*>>());
        REGISTER_COMPONENT_WITH_ECS(state, LuaScriptComponent, static_cast<LuaScriptComponent& (Entity::*)(std::string&&, Scene * &&)>(&Entity::AddComponent<LuaScriptComponent, std::string, Scene*>));

        using namespace Maths;
        REGISTER_COMPONENT_WITH_ECS(state, Transform, static_cast<Transform& (Entity::*)()>(&Entity::AddComponent<Transform>));

        using namespace Graphics;
        sol::usertype<Sprite> sprite_type = state.new_usertype<Sprite>("Sprite", sol::constructors<sol::types<Maths::Vector2, Maths::Vector2, Maths::Vector4>, Sprite(const Ref<Graphics::Texture2D>&, const Maths::Vector2&, const Maths::Vector2&, const Maths::Vector4&)>());
        sprite_type.set_function("SetTexture", &Sprite::SetTexture);

        REGISTER_COMPONENT_WITH_ECS(state, Sprite, static_cast<Sprite& (Entity::*)(const Vector2&, const Vector2&, const Vector4&)>(&Entity::AddComponent<Sprite, const Vector2&, const Vector2&, const Vector4&>));

        REGISTER_COMPONENT_WITH_ECS(state, Light, static_cast<Light& (Entity::*)()>(&Entity::AddComponent<Light>));

        std::initializer_list<std::pair<sol::string_view, Hubie::Graphics::PrimitiveType>> primitives = {
            { "Cube", Hubie::Graphics::PrimitiveType::Cube },
            { "Plane", Hubie::Graphics::PrimitiveType::Plane },
            { "Quad", Hubie::Graphics::PrimitiveType::Quad },
            { "Pyramid", Hubie::Graphics::PrimitiveType::Pyramid },
            { "Sphere", Hubie::Graphics::PrimitiveType::Sphere },
            { "Capsule", Hubie::Graphics::PrimitiveType::Capsule },
            { "Cylinder", Hubie::Graphics::PrimitiveType::Cylinder },
            { "Terrain", Hubie::Graphics::PrimitiveType::Terrain },
        };

        state.new_enum<Hubie::Graphics::PrimitiveType, false>("PrimitiveType", primitives);

        state.new_usertype<Model>("Model", sol::constructors<Model(const std::string&), Model(Hubie::Graphics::PrimitiveType)>());
        REGISTER_COMPONENT_WITH_ECS(state, Model, static_cast<Model& (Entity::*)(const std::string&)>(&Entity::AddComponent<Model, const std::string&>));

        sol::usertype<Camera> camera_type = state.new_usertype<Camera>("Camera", sol::constructors<Camera(float, float, float, float), Camera(float, float)>());
        camera_type["fov"] = &Camera::GetFOV;
        camera_type["aspectRatio"] = &Camera::GetAspectRatio;
        camera_type["nearPlane"] = &Camera::GetNear;
        camera_type["farPlane"] = &Camera::GetFar;
        camera_type["SetIsOrthographic"] = &Camera::SetIsOrthographic;
        camera_type["SetNearPlane"] = &Camera::SetNear;
        camera_type["SetFarPlane"] = &Camera::SetFar;

        REGISTER_COMPONENT_WITH_ECS(state, Camera, static_cast<Camera& (Entity::*)(const float&, const float&)>(&Entity::AddComponent<Camera, const float&, const float&>));

        sol::usertype<Physics3DComponent> physics3DComponent_type = state.new_usertype<Physics3DComponent>("Physics3DComponent", sol::constructors<sol::types<const Ref<RigidBody3D>&>>());
        physics3DComponent_type.set_function("GetRigidBody", &Physics3DComponent::GetRigidBody);
        REGISTER_COMPONENT_WITH_ECS(state, Physics3DComponent, static_cast<Physics3DComponent& (Entity::*)(Ref<RigidBody3D>&)>(&Entity::AddComponent<Physics3DComponent, Ref<RigidBody3D>&>));

        sol::usertype<Physics2DComponent> physics2DComponent_type = state.new_usertype<Physics2DComponent>("Physics2DComponent", sol::constructors<sol::types<const RigidBodyParameters&>>());
        physics2DComponent_type.set_function("GetRigidBody", &Physics2DComponent::GetRigidBody);

        REGISTER_COMPONENT_WITH_ECS(state, Physics2DComponent, static_cast<Physics2DComponent& (Entity::*)(const RigidBodyParameters&)>(&Entity::AddComponent<Physics2DComponent, const RigidBodyParameters&>));

        REGISTER_COMPONENT_WITH_ECS(state, SoundComponent, static_cast<SoundComponent& (Entity::*)()>(&Entity::AddComponent<SoundComponent>));

        //state.set_function("LoadMesh", &ModelLoader::LoadModel);
        //TODO MODEL
        sol::usertype<Graphics::Mesh> mesh_type = state.new_usertype<Graphics::Mesh>("Mesh");

        std::initializer_list<std::pair<sol::string_view, Hubie::Graphics::TextureFilter>> textureFilter = {
            { "None", Hubie::Graphics::TextureFilter::NONE },
            { "Linear", Hubie::Graphics::TextureFilter::LINEAR },
            { "Nearest", Hubie::Graphics::TextureFilter::NEAREST }
        };

        std::initializer_list<std::pair<sol::string_view, Hubie::Graphics::TextureWrap>> textureWrap = {
            { "None", Hubie::Graphics::TextureWrap::NONE },
            { "Repeat", Hubie::Graphics::TextureWrap::REPEAT },
            { "Clamp", Hubie::Graphics::TextureWrap::CLAMP },
            { "MirroredRepeat", Hubie::Graphics::TextureWrap::MIRRORED_REPEAT },
            { "ClampToEdge", Hubie::Graphics::TextureWrap::CLAMP_TO_EDGE },
            { "ClampToBorder", Hubie::Graphics::TextureWrap::CLAMP_TO_BORDER }
        };

        state.set_function("LoadMesh", &CreatePrimative);

        state.new_enum<Hubie::Graphics::TextureWrap, false>("TextureWrap", textureWrap);
        state.new_enum<Hubie::Graphics::TextureFilter, false>("TextureFilter", textureFilter);

        state.set_function("LoadTexture", &LoadTexture);
        state.set_function("LoadTextureWithParams", &LoadTextureWithParams);
    }

    static float LuaRand(float a, float b)
    {
        return RandomNumberGenerator32::Rand(a, b);
    }

    void LuaManager::BindSceneLua(sol::state& state)
    {
        sol::usertype<Scene> scene_type = state.new_usertype<Scene>("Scene");
        scene_type.set_function("GetRegistry", &Scene::GetRegistry);
        scene_type.set_function("GetEntityManager", &Scene::GetEntityManager);

        sol::usertype<Graphics::Texture2D> texture2D_type = state.new_usertype<Graphics::Texture2D>("Texture2D");
        texture2D_type.set_function("CreateFromFile", &Graphics::Texture2D::CreateFromFile);

        state.set_function("Rand", &LuaRand);
    }

    static void SwitchSceneByIndex(int index)
    {
        Application::Get().GetSceneManager()->SwitchScene(index);
    }

    static void SwitchScene()
    {
        Application::Get().GetSceneManager()->SwitchScene();
    }

    static void SwitchSceneByName(const std::string& name)
    {
        Application::Get().GetSceneManager()->SwitchScene(name);
    }

    static void SetPhysicsDebugFlags(int flags)
    {
        Application::Get().GetSystem<HubiePhysicsEngine>()->SetDebugDrawFlags(flags);
    }

    void LuaManager::BindAppLua(sol::state& state)
    {
        sol::usertype<Application> app_type = state.new_usertype<Application>("Application");
        state.set_function("SwitchSceneByIndex", &SwitchSceneByIndex);
        state.set_function("SwitchSceneByName", &SwitchSceneByName);
        state.set_function("SwitchScene", &SwitchScene);
        state.set_function("SetPhysicsDebugFlags", &SetPhysicsDebugFlags);

        std::initializer_list<std::pair<sol::string_view, Hubie::PhysicsDebugFlags>> physicsDebugFlags = {
            { "CONSTRAINT", Hubie::PhysicsDebugFlags::CONSTRAINT },
            { "MANIFOLD", Hubie::PhysicsDebugFlags::MANIFOLD },
            { "COLLISIONVOLUMES", Hubie::PhysicsDebugFlags::COLLISIONVOLUMES },
            { "COLLISIONNORMALS", Hubie::PhysicsDebugFlags::COLLISIONNORMALS },
            { "AABB", Hubie::PhysicsDebugFlags::AABB },
            { "LINEARVELOCITY", Hubie::PhysicsDebugFlags::LINEARVELOCITY },
            { "LINEARFORCE", Hubie::PhysicsDebugFlags::LINEARFORCE },
            { "BROADPHASE", Hubie::PhysicsDebugFlags::BROADPHASE },
            { "BROADPHASE_PAIRS", Hubie::PhysicsDebugFlags::BROADPHASE_PAIRS },
            { "BOUNDING_RADIUS", Hubie::PhysicsDebugFlags::BOUNDING_RADIUS },
        };

        state.new_enum<PhysicsDebugFlags, false>("PhysicsDebugFlags", physicsDebugFlags);

        app_type.set_function("GetWindowSize", &Application::GetWindowSize);
        state.set_function("GetAppInstance", &Application::Get);
    }
}
