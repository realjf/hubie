#include "Precompiled.h"
#include "RigidBody.h"

namespace Hubie
{

    RigidBody::RigidBody()
        : m_Static(false)
        , m_Elasticity(0.9f)
        , m_Friction(0.8f)
        , m_AtRest(false)
    {
    }

    RigidBody::~RigidBody()
    {
    }
}
