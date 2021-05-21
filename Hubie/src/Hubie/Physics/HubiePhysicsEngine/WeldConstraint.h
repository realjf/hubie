#pragma once

#include "Constraint.h"

namespace Hubie
{
    namespace Maths
    {
        class Vector3;
    }

    class Quaternion;
    class RigidBody3D;

    class HB_EXPORT WeldConstraint : public Constraint
    {
    public:
        WeldConstraint(RigidBody3D* obj1, RigidBody3D* obj2);

        virtual void ApplyImpulse() override;
        virtual void DebugDraw() const override;

    protected:
        RigidBody3D* m_pObj1;
        RigidBody3D* m_pObj2;

        Maths::Vector3 m_positionOffset;
        Maths::Quaternion m_orientation;
    };
}