/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2014, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsiteс.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */

/**
 * \file object/object.h
 * \brief CObject - base class for all game objects
 */

#pragma once

#include "object/crash_sphere.h"
#include "object/object_interface_type.h"
#include "object/old_object_interface.h"

#include <vector>

namespace Gfx {
struct ModelCrashSphere;
} // namespace Gfx

/**
 * \class CObject
 * \brief Base class for all 3D in-game objects
 *
 * CObject serves as a base class for all in-game objects, including:
 *  - buildings,
 *  - robots,
 *  - astronaut,
 *  - plants,
 *  - aliens.
 *
 * As every object has its specific behavior, there are or will be
 * separate subclasses for each of the specific objects. For the time being,
 * old object interface is still present, but its functions will be moved to
 * appropriate subclasses with time. The new CObject interface implemented
 * here will feature only functions common to all objects.
 */
class CObject : public COldObjectInterface
{
protected:
    //! Constructor only accessible to subclasses
    CObject(int id, ObjectType type);

public:
    CObject(const CObject&) = delete;
    CObject& operator=(const CObject&) = delete;

    virtual ~CObject();

    //! Returns object type
    inline ObjectType  GetType() const
    {
        return m_type;
    }
    //! Returns object's unique id
    inline int GetID() const
    {
        return m_id;
    }

    //! Writes object properties to line in level file
    virtual void Write(CLevelParserLine* line) = 0;
    //! Reads object properties from line in level file
    virtual void Read(CLevelParserLine* line) = 0;

    //! Check if object implements the given type of interface
    inline bool Implements(ObjectInterfaceType type) const
    {
        return m_implementedInterfaces[static_cast<int>(type)];
    }

    //! Returns object's position
    virtual Math::Vector GetPosition() const;
    //! Sets object's position
    virtual void SetPosition(const Math::Vector& pos);

    //! Returns object's rotation (Euler angles)
    /** Angles are given in radians */
    virtual Math::Vector GetRotation() const;

    //! Returns object's scale
    virtual Math::Vector GetScale() const;

    //! Sets crash spheres for object
    void SetCrashSpheres(const std::vector<Gfx::ModelCrashSphere>& crashSpheres);
    //! Adds a new crash sphere
    /** Crash sphere position is given in object coordinates */
    void AddCrashSphere(const CrashSphere& crashSphere);
    //! Returns total number of crash spheres
    int GetCrashSphereCount();
    //! Returns the first crash sphere (assumes it exists)
    /** Crash sphere position is returned in world coordinates */
    CrashSphere GetFirstCrashSphere();
    //! Returns all crash spheres
    /** Crash sphere position is returned in world coordinates */
    std::vector<CrashSphere> GetAllCrashSpheres();
    //! Removes all crash spheres
    void DeleteAllCrashSpheres();

    //! Returns sphere used to test for camera collisions
    Math::Sphere GetCameraCollisionSphere();
    //! Sets sphere used to test for camera collisions
    // TODO: remove from here once no longer necessary
    void SetCameraCollisionSphere(const Math::Sphere& sphere);

    //! Sets the transparency of object
    virtual void SetTransparency(float value) = 0;

    //! Sets flag controlling animation effect on level reset
    void SetAnimateOnReset(bool animateOnReset);
    //! Returns flag controlling animation effect on level reset
    bool GetAnimateOnReset();

protected:
    //! Transform crash sphere by object's world matrix
    virtual void TransformCrashSphere(Math::Sphere& crashSphere) = 0;
    //! Transform crash sphere by object's world matrix
    virtual void TransformCameraCollisionSphere(Math::Sphere& collisionSphere) = 0;

protected:
    const int m_id; //!< unique identifier
    ObjectType m_type; //!< object type
    ObjectInterfaceTypes m_implementedInterfaces; //!< interfaces that the object implements
    Math::Vector m_position;
    Math::Vector m_rotation;
    Math::Vector m_scale;
    std::vector<CrashSphere> m_crashSpheres; //!< crash spheres
    Math::Sphere m_cameraCollisionSphere;
    bool m_animateOnReset;
};
