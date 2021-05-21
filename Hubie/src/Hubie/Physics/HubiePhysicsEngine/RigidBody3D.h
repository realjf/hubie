#pragma once

#include "Hubie/Physics/RigidBody.h"
#include "CollisionShape.h"
#include "Hubie/Physics/HubiePhysicsEngine/SphereCollisionShape.h"
#include "Hubie/Physics/HubiePhysicsEngine/CuboidCollisionShape.h"
#include "Hubie/Physics/HubiePhysicsEngine/PyramidCollisionShape.h"
#include "Hubie/Physics/HubiePhysicsEngine/HullCollisionShape.h"

#include "Hubie/Maths/Maths.h"
#include <cereal/types/polymorphic.hpp>
#include <cereal/cereal.hpp>

CEREAL_REGISTER_TYPE(Hubie::SphereCollisionShape);
CEREAL_REGISTER_TYPE(Hubie::CuboidCollisionShape);
CEREAL_REGISTER_TYPE(Hubie::PyramidCollisionShape);
CEREAL_REGISTER_TYPE(Hubie::HullCollisionShape);

CEREAL_REGISTER_POLYMORPHIC_RELATION(Hubie::CollisionShape, Hubie::SphereCollisionShape);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Hubie::CollisionShape, Hubie::CuboidCollisionShape);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Hubie::CollisionShape, Hubie::PyramidCollisionShape);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Hubie::CollisionShape, Hubie::HullCollisionShape);

namespace Hubie
{
    class HubiePhysicsEngine;
    class Manifold;

    //Callback function called whenever a collision is detected between two objects
    //Params:
    //	RigidBody3D* this_obj			- The current object class that contains the callback
    //	RigidBody3D* colliding_obj	- The object that is colliding with the given object
    //Return:
    //  True	- The physics engine should process the collision as normal
    //	False	- The physics engine should drop the collision pair and not do any further collision resolution/manifold generation
    //			  > This can be useful for AI to see if a player/agent is inside an area/collision volume
    typedef std::function<bool(RigidBody3D* this_obj, RigidBody3D* colliding_obj)> PhysicsCollisionCallback;

    struct RigidBody3DProperties
    {
        Maths::Vector3 Position = Maths::Vector3(0.0f);
        Maths::Vector3 LinearVelocity = Maths::Vector3(0.0f);
        Maths::Vector3 Force = Maths::Vector3(0.0f);
        float Mass = 1.0f;
        Maths::Quaternion Orientation = Maths::Quaternion();
        Maths::Vector3 AngularVelocity = Maths::Vector3(0.0f);
        Maths::Vector3 Torque = Maths::Vector3(0.0f);
        bool Static = false;
        float Elasticity = 0.9f;
        float Friction = 0.8f;
        bool AtRest = false;
        bool isTrigger = false;
        Ref<CollisionShape> Shape = nullptr;
    };

    class HB_EXPORT RigidBody3D : public RigidBody
    {
        friend class HubiePhysicsEngine;

    public:
        RigidBody3D(const RigidBody3DProperties& properties = RigidBody3DProperties());
        virtual ~RigidBody3D();

        //<--------- GETTERS ------------->
        const Maths::Vector3& GetPosition() const
        {
            return m_Position;
        }
        const Maths::Vector3& GetLinearVelocity() const
        {
            return m_LinearVelocity;
        }
        const Maths::Vector3& GetForce() const
        {
            return m_Force;
        }
        float GetInverseMass() const
        {
            return m_InvMass;
        }
        const Maths::Quaternion& GetOrientation() const
        {
            return m_Orientation;
        }
        const Maths::Vector3& GetAngularVelocity() const
        {
            return m_AngularVelocity;
        }
        const Maths::Vector3& GetTorque() const
        {
            return m_Torque;
        }
        const Maths::Matrix3& GetInverseInertia() const
        {
            return m_InvInertia;
        }
        const Maths::Matrix4& GetWorldSpaceTransform() const; //Built from scratch or returned from cached value

        const Maths::BoundingBox& GetWorldSpaceAABB();

        void WakeUp() override;
        void SetIsAtRest(const bool isAtRest) override;

        Maths::BoundingBox GetLocalBoundingBox() const
        {
            return m_localBoundingBox;
        }

        void SetRestVelocityThreshold(float vel)
        {
            if(vel <= 0.0f)
                m_RestVelocityThresholdSquared = -1.0f;
            else
                m_RestVelocityThresholdSquared = vel * vel;
        }

        void SetLocalBoundingBox(const Maths::BoundingBox& bb)
        {
            m_localBoundingBox = bb;
            m_wsAabbInvalidated = true;
        }

        //<--------- SETTERS ------------->

        void SetPosition(const Maths::Vector3& v)
        {
            m_Position = v;
            m_wsTransformInvalidated = true;
            m_wsAabbInvalidated = true;
            //m_AtRest = false;
        }

        void SetLinearVelocity(const Maths::Vector3& v)
        {
            if(m_Static)
                return;
            m_LinearVelocity = v;
        }
        void SetForce(const Maths::Vector3& v)
        {
            if(m_Static)
                return;
            m_Force = v;
        }

        void SetOrientation(const Maths::Quaternion& v)
        {
            m_Orientation = v;
            m_wsTransformInvalidated = true;
            //m_AtRest = false;
        }

        void SetAngularVelocity(const Maths::Vector3& v)
        {
            if(m_Static)
                return;
            m_AngularVelocity = v;
        }
        void SetTorque(const Maths::Vector3& v)
        {
            if(m_Static)
                return;
            m_Torque = v;
        }
        void SetInverseInertia(const Maths::Matrix3& v)
        {
            m_InvInertia = v;
        }

        //<---------- CALLBACKS ------------>
        void SetOnCollisionCallback(PhysicsCollisionCallback& callback)
        {
            m_OnCollisionCallback = callback;
        }

        bool FireOnCollisionEvent(RigidBody3D* obj_a, RigidBody3D* obj_b)
        {
            const bool handleCollision = (m_OnCollisionCallback) ? m_OnCollisionCallback(obj_a, obj_b) : true;

            // Wake up on collision
            if(handleCollision)
                WakeUp();

            return handleCollision;
        }

        void FireOnCollisionManifoldCallback(RigidBody3D* a, RigidBody3D* b, Manifold* manifold)
        {
            for(auto it = m_onCollisionManifoldCallbacks.begin(); it != m_onCollisionManifoldCallbacks.end(); ++it)
                it->operator()(a, b, manifold);
        }

        void AutoResizeBoundingBox();
        void RestTest();

        virtual void DebugDraw(uint64_t flags) const;

        typedef std::function<void(RigidBody3D*, RigidBody3D*, Manifold*)> OnCollisionManifoldCallback;

        void AddOnCollisionManifoldCallback(const OnCollisionManifoldCallback callback)
        {
            m_onCollisionManifoldCallbacks.push_back(callback);
        }

        void SetCollisionShape(const Ref<CollisionShape>& shape)
        {
            m_CollisionShape = shape;
            m_InvInertia = m_CollisionShape->BuildInverseInertia(m_InvMass);
            AutoResizeBoundingBox();
        }

        void SetCollisionShape(CollisionShapeType type);

        void CollisionShapeUpdated()
        {
            if(m_CollisionShape)
                m_InvInertia = m_CollisionShape->BuildInverseInertia(m_InvMass);
            AutoResizeBoundingBox();
        }

        void SetInverseMass(const float& v)
        {
            m_InvMass = v;
            if(m_CollisionShape)
                m_InvInertia = m_CollisionShape->BuildInverseInertia(m_InvMass);
        }

        void SetMass(const float& v)
        {
            HB_ASSERT(v > 0, "Physics object mass <= 0");
            m_InvMass = 1.0f / v;

            if(m_CollisionShape)
                m_InvInertia = m_CollisionShape->BuildInverseInertia(m_InvMass);
        }

        const Ref<CollisionShape>& GetCollisionShape() const
        {
            return m_CollisionShape;
        }

        bool GetIsTrigger() const { return m_Trigger; }
        void SetIsTrigger(bool trigger) { m_Trigger = trigger; }

        template <typename Archive>
        void save(Archive& archive) const
        {
            auto shape = std::unique_ptr<CollisionShape>(m_CollisionShape.get());

            archive(cereal::make_nvp("Position", m_Position), cereal::make_nvp("Orientation", m_Orientation), cereal::make_nvp("LinearVelocity", m_LinearVelocity), cereal::make_nvp("Force", m_Force), cereal::make_nvp("Mass", 1.0f / m_InvMass), cereal::make_nvp("AngularVelocity", m_AngularVelocity), cereal::make_nvp("Torque", m_Torque), cereal::make_nvp("Static", m_Static), cereal::make_nvp("Friction", m_Friction), cereal::make_nvp("Elasticity", m_Elasticity), cereal::make_nvp("CollisionShape", shape), cereal::make_nvp("Trigger", m_Trigger));

            shape.release();
        }

        template <typename Archive>
        void load(Archive& archive)
        {
            auto shape = std::unique_ptr<CollisionShape>(m_CollisionShape.get());
            archive(cereal::make_nvp("Position", m_Position), cereal::make_nvp("Orientation", m_Orientation), cereal::make_nvp("LinearVelocity", m_LinearVelocity), cereal::make_nvp("Force", m_Force), cereal::make_nvp("Mass", 1.0f / m_InvMass), cereal::make_nvp("AngularVelocity", m_AngularVelocity), cereal::make_nvp("Torque", m_Torque), cereal::make_nvp("Static", m_Static), cereal::make_nvp("Friction", m_Friction), cereal::make_nvp("Elasticity", m_Elasticity), cereal::make_nvp("CollisionShape", shape), cereal::make_nvp("Trigger", m_Trigger));

            m_CollisionShape = Ref<CollisionShape>(shape.get());
            CollisionShapeUpdated();
            shape.release();
        }

    protected:
        mutable bool m_wsTransformInvalidated;
        float m_RestVelocityThresholdSquared;
        float m_AverageSummedVelocity;

        mutable Maths::Matrix4 m_wsTransform;
        Maths::BoundingBox m_localBoundingBox; //!< Model orientated bounding box in model space
        mutable bool m_wsAabbInvalidated; //!< Flag indicating if the cached world space transoformed AABB is invalid
        mutable Maths::BoundingBox m_wsAabb; //!< Axis aligned bounding box of this object in world space

        //<---------LINEAR-------------->
        Maths::Vector3 m_Position;
        Maths::Vector3 m_LinearVelocity;
        Maths::Vector3 m_Force;
        float m_InvMass;
        bool m_Trigger = false;

        //<----------ANGULAR-------------->
        Maths::Quaternion m_Orientation;
        Maths::Vector3 m_AngularVelocity;
        Maths::Vector3 m_Torque;
        Maths::Matrix3 m_InvInertia;

        //<----------COLLISION------------>
        Ref<CollisionShape> m_CollisionShape;
        PhysicsCollisionCallback m_OnCollisionCallback;
        std::vector<OnCollisionManifoldCallback> m_onCollisionManifoldCallbacks; //!< Collision callbacks post manifold generation
    };
}
