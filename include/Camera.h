/*
 * File:   Camera.h
 * Author: Donald "Datriot" Whyte
 *
 * Created on June 16, 2009, 1:38 PM
 */

#ifndef CAMERA_H
#define CAMERA_H

//
// Header (.h) file below!
//

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

namespace parcel
{

namespace maths
{

    enum CameraBehavior
    {
        CAMERA_BEHAVIOR_FIRST_PERSON,
        CAMERA_BEHAVIOR_SPECTATOR,
        CAMERA_BEHAVIOR_FLIGHT,
        CAMERA_BEHAVIOR_ORBIT
    };

    template<typename T>
    class Camera
    {


    private:

        static const T defaultRotationSpeed;
        static const T defaultFovx;
        static const T defaultZNear;
        static const T defaultZFar;
        static const T defaultOrbitMinZoom;
        static const T defaultOrbitMaxZoom;
        static const T defaultOrbitOffsetDistance;
        static const Vector3<T> worldXAxis;
        static const Vector3<T> worldYAxis;
        static const Vector3<T> worldZAxis;

        CameraBehavior behavior;
        bool preferTargetYAxisOrbiting;
        T accumPitchDegrees;
        T savedAccumPitchDegrees;
        T rotationSpeed;
        T fovx;
        T aspectRatio;
        T znear;
        T zfar;
        T orbitMinZoom;
        T orbitMaxZoom;
        T orbitOffsetDistance;
        T firstPersonYOffset;
        Vector3<T> eye;
        Vector3<T> savedEye;
        Vector3<T> target;
        Vector3<T> targetYAxis;
        Vector3<T> xAxis;
        Vector3<T> yAxis;
        Vector3<T> zAxis;
        Vector3<T> viewDir;
        Vector3<T> acceleration;
        Vector3<T> currentVelocity;
        Vector3<T> velocity;
        Quaternion<T> orientation;
        Quaternion<T> savedOrientation;
        Matrix<T> viewMatrix;
        Matrix<T> projMatrix;
        Matrix<T> viewProjMatrix;


        void RotateFirstPerson(T headingDegrees, T pitchDegrees);
        void RotateFlight(T headingDegrees, T pitchDegrees, T rollDegrees);
        void RotateOrbit(T headingDegrees, T pitchDegrees, T rollDegrees);
        void UpdateVelocity(const Vector3<T> &direction, float elapsedTimeSec); // NOTE: is float
        void UpdateViewMatrix();


    public:

        // Constructor/destructor
        Camera();
        ~Camera();

        // Methods
        void LookAt(const Vector3<T> &target);
        void LookAt(const Vector3<T> &eye, const Vector3<T> &target, const Vector3<T> &up);
        void Move(T dx, T dy, T dz);
        void Move(const Vector3<T> &direction, const Vector3<T> &amount);
        void Perspective(T fovx, T aspect, T znear, T zfar);
        void Rotate(T headingDegrees, T pitchDegrees, T rollDegrees);
        void RotateSmoothly(T headingDegrees, T pitchDegrees, T rollDegrees);
        void UndoRoll();
        void UpdatePosition(const Vector3<T> &direction, T elapsedTimeSec);
        void Zoom(T zoom, T minZoom, T maxZoom);

        // Getters
        inline const Vector3<T> &Camera<T>::GetAcceleration() const
        { return acceleration; }

        inline CameraBehavior Camera<T>::GetBehavior() const
        { return behavior; }

        inline const Vector3<T> &Camera<T>::GetCurrentVelocity() const
        { return currentVelocity; }

        inline const Vector3<T> &Camera<T>::GetPosition() const
        { return eye; }

        inline float Camera<T>::GetOrbitMinZoom() const
        { return orbitMinZoom; }

        inline float Camera<T>::GetOrbitMaxZoom() const
        { return orbitMaxZoom; }

        inline float Camera<T>::GetOrbitOffsetDistance() const
        { return orbitOffsetDistance; }

        inline const Quaternion<T> &Camera<T>::GetOrientation() const
        { return orientation; }

        inline float Camera<T>::GetRotationSpeed() const
        { return rotationSpeed; }

        inline const Matrix<T> &Camera<T>::GetProjectionMatrix() const
        { return projMatrix; }

        inline const Vector3<T> &Camera<T>::GetVelocity() const
        { return velocity; }

        inline const Vector3<T> &Camera<T>::GetViewDirection() const
        { return viewDir; }

        inline const Matrix<T> &Camera<T>::GetViewMatrix() const
        { return viewMatrix; }

        inline const Matrix<T> &Camera<T>::GetViewProjectionMatrix() const
        { return viewProjMatrix; }

        inline const Vector3<T> &Camera<T>::GetXAxis() const
        { return xAxis; }

        inline const Vector3<T> &Camera<T>::GetYAxis() const
        { return yAxis; }

        inline const Vector3<T> &Camera<T>::GetZAxis() const
        { return zAxis; }

        inline bool Camera<T>::PreferTargetYAxisOrbiting() const
        { return preferTargetYAxisOrbiting; }

        // Setters
        void SetAcceleration(const Vector3<T> &newAcceleration);
        void SetBehavior(CameraBehavior newBehavior);
        void SetCurrentVelocity(const Vector3<T> &currentVelocity);
        void SetCurrentVelocity(T x, T y, T z);
        void SetOrbitMaxZoom(T orbitMaxZoom);
        void SetOrbitMinZoom(T orbitMinZoom);
        void SetOrbitOffsetDistance(T orbitOffsetDistance);
        void SetOrbitPitchMaxDegrees(T orbitPitchMaxDegrees);
        void SetOrbitPitchMinDegrees(T orbitPitchMinDegrees);
        void SetOrientation(const Quaternion<T> &newOrientation);
        void SetPosition(const Vector3<T> &newEye);
        void SetPreferTargetYAxisOrbiting(bool preferTargetYAxisOrbiting);
        void SetRotationSpeed(T rotationSpeed);
        void SetVelocity(const Vector3<T> &velocity);
        void SetVelocity(T x, T y, T z);


    };

    // Typedefs for easy typing
    typedef Camera<int> camerai;
    typedef Camera<float> cameraf;
    typedef Camera<double> camerad;

}

}

//
// Source (.cpp) file below!
//

namespace parcel
{

namespace maths
{

    template<typename T> const T Camera<T>::defaultRotationSpeed = 0.3f;
    template<typename T> const T Camera<T>::defaultFovx = 90.0f;
    template<typename T> const T Camera<T>::defaultZNear = 0.1f;
    template<typename T> const T Camera<T>::defaultZFar = 1000.0f;

    template<typename T> const T Camera<T>::defaultOrbitMinZoom = defaultZNear + 1.0f;
    template<typename T> const T Camera<T>::defaultOrbitMaxZoom = defaultZFar * 0.5f;

    template<typename T> const T Camera<T>::defaultOrbitOffsetDistance =
        defaultOrbitMinZoom + (defaultOrbitMaxZoom - defaultOrbitMinZoom) * 0.25f;

    template<typename T> const Vector3<T> Camera<T>::worldXAxis(1.0f, 0.0f, 0.0f);
    template<typename T> const Vector3<T> Camera<T>::worldYAxis(0.0f, 1.0f, 0.0f);
    template<typename T> const Vector3<T> Camera<T>::worldZAxis(0.0f, 0.0f, 1.0f);

    template<typename T>
    Camera<T>::Camera()
    {
        behavior = CAMERA_BEHAVIOR_FLIGHT;
        preferTargetYAxisOrbiting = true;

        accumPitchDegrees = 0.0f;
        savedAccumPitchDegrees = 0.0f;

        rotationSpeed = defaultRotationSpeed;
        fovx = defaultFovx;
        aspectRatio = 0.0f;
        znear = defaultZNear;
        zfar = defaultZFar;

        orbitMinZoom = defaultOrbitMinZoom;
        orbitMaxZoom = defaultOrbitMaxZoom;
        orbitOffsetDistance = defaultOrbitOffsetDistance;

        eye = Vector3<T>(0.0f, 0.0f, 0.0f);
        savedEye = Vector3<T>(0.0f, 0.0f, 0.0f);
        target = Vector3<T>(0.0f, 0.0f, 0.0f);
        xAxis = Vector3<T>(1.0f, 0.0f, 0.0f);
        yAxis = Vector3<T>(0.0f, 1.0f, 0.0f);
        targetYAxis = Vector3<T>(0.0f, 1.0f, 0.0f);
        zAxis = Vector3<T>(0.0f, 0.0f, 1.0f);
        viewDir = Vector3<T>(0.0f, 0.0f, -1.0f);

        acceleration = Vector3<T>(0.0f, 0.0f, 0.0f);
        currentVelocity = Vector3<T>(0.0f, 0.0f, 0.0f);
        velocity = Vector3<T>(0.0f, 0.0f, 0.0f);

        orientation.ToIdentity();
        savedOrientation.ToIdentity();

        viewMatrix = Matrix<T>::Identity(4);
        viewMatrix.SetElement(2, 2, -1);
        //viewMatrix.SetElement(2, 3, -2000);
        projMatrix = Matrix<T>::Identity(4);
        viewProjMatrix = Matrix<T>::Identity(4);
    }

    template<typename T>
    Camera<T>::~Camera()
    {
    }

    template <typename T>
    void Camera<T>::LookAt(const Vector3<T> &target)
    {
        LookAt(eye, target, yAxis);
    }

    template <typename T>
    void Camera<T>::LookAt(const Vector3<T> &newEye, const Vector3<T> &newTarget, const Vector3<T> &newUp)
    {
        eye = newEye;
        target = newTarget;

        zAxis = eye - target;
        zAxis.Normalise();

        viewDir = -zAxis;

        xAxis = Vector3<T>::Cross(newUp, zAxis);
        xAxis.Normalise();

        yAxis = Vector3<T>::Cross(zAxis, xAxis);
        yAxis.Normalise();
        xAxis.Normalise();

        viewMatrix.SetElement(0, 0, xAxis.x);
        viewMatrix.SetElement(1, 0, xAxis.y);
        viewMatrix.SetElement(2, 0, xAxis.z);
        viewMatrix.SetElement(3, 0, -Vector3<T>::Dot(xAxis, eye));

        viewMatrix.SetElement(0, 2, yAxis.x);
        viewMatrix.SetElement(1, 2, yAxis.y);
        viewMatrix.SetElement(2, 2, yAxis.z);
        viewMatrix.SetElement(3, 2, -Vector3<T>::Dot(yAxis, eye));

        viewMatrix.SetElement(0, 2, zAxis.x);
        viewMatrix.SetElement(1, 2, zAxis.y);
        viewMatrix.SetElement(2, 2, zAxis.z);
        viewMatrix.SetElement(3, 2, -Vector3<T>::Dot(zAxis, eye));

        // Extract the pitch angle from the view matrix.
        accumPitchDegrees = RadiansToDegrees(asinf(viewMatrix(1, 2)));

        orientation.FromMatrix(viewMatrix);
    }

    template<typename T>
    void Camera<T>::Move(T dx, T dy, T dz)
    {
        // Moves the camera by dx world units to the left or right; dy
        // world units upwards or downwards; and dz world units forwards
        // or backwards.

        if (behavior == CAMERA_BEHAVIOR_ORBIT)
        {
            // Orbiting camera is always positioned relative to the
            // target position. See updateViewMatrix().
            return;
        }

        Vector3<T> newEye = eye;
        Vector3<T> forwards;

        if (behavior == CAMERA_BEHAVIOR_FIRST_PERSON)
        {
            // Calculate the forwards direction. Can't just use the camera's local
            // z axis as doing so will cause the camera to move more slowly as the
            // camera's view approaches 90 degrees straight up and down.

            forwards = Vector3<T>::Cross(worldYAxis, xAxis);
            forwards.Normalise();
        }
        else
        {
            forwards = viewDir;
        }

        newEye += xAxis * dx;
        newEye += worldYAxis * dy;
        newEye += forwards * dz;

        SetPosition(eye);
    }

    template<typename T>
    void Camera<T>::Move(const Vector3<T> &direction, const Vector3<T> &amount)
    {
        // Moves the camera by the specified amount of world units in the specified
        // direction in world space.

        if (behavior == CAMERA_BEHAVIOR_ORBIT)
        {
            // Orbiting camera is always positioned relative to the
            // target position. See updateViewMatrix().
            return;
        }

        eye.x += direction.x * amount.x;
        eye.y += direction.y * amount.y;
        eye.z += direction.z * amount.z;

        UpdateViewMatrix();
    }

    template<typename T>
    void Camera<T>::Perspective(T fovx, T aspect, T znear, T zfar)
    {
        // Construct a projection matrix based on the horizontal field of view
        // 'fovx' rather than the more traditional vertical field of view 'fovy'.

        T e = 1.0f / tanf(DegreesToRadians(fovx) / 2.0f);
        T aspectInv = 1.0f / aspect;
        T fovy = 2.0f * atanf(aspectInv / e);
        T xScale = 1.0f / tanf(0.5f * fovy);
        T yScale = xScale / aspectInv;

        projMatrix.SetElement(0, 0, xScale);
        projMatrix.SetElement(0, 1, 0.0f);
        projMatrix.SetElement(0, 2, 0.0f);
        projMatrix.SetElement(0, 3, 0.0f);

        projMatrix.SetElement(1, 0, 0.0f);
        projMatrix.SetElement(1, 1, yScale);
        projMatrix.SetElement(1, 2, 0.0f);
        projMatrix.SetElement(1, 3, 0.0f);

        projMatrix.SetElement(2, 0, 0.0f);
        projMatrix.SetElement(2, 1, 0.0f);
        projMatrix.SetElement(2, 2, (zfar + znear) / (znear - zfar));
        projMatrix.SetElement(2, 3, -1.0f);

        projMatrix.SetElement(3, 0, 0.0f);
        projMatrix.SetElement(3, 1, 0.0f);
        projMatrix.SetElement(3, 2, (2.0f * zfar * znear) / (znear - zfar));
        projMatrix.SetElement(3, 3, 0.0f);

        viewProjMatrix = viewMatrix * projMatrix;

        fovx = fovx;
        aspectRatio = aspect;
        znear = znear;
        zfar = zfar;
    }

    template<typename T>
    void Camera<T>::Rotate(T headingDegrees, T pitchDegrees, T rollDegrees)
    {
        // Rotates the camera based on its current behavior.
        // Note that not all behaviors support rolling.

        pitchDegrees = -pitchDegrees;
        headingDegrees = -headingDegrees;
        rollDegrees = -rollDegrees;

        switch (behavior)
        {
        default:
            break;

        case CAMERA_BEHAVIOR_FIRST_PERSON:
        case CAMERA_BEHAVIOR_SPECTATOR:
            RotateFirstPerson(headingDegrees, pitchDegrees);
            break;

        case CAMERA_BEHAVIOR_FLIGHT:
            RotateFlight(headingDegrees, pitchDegrees, rollDegrees);
            break;

        case CAMERA_BEHAVIOR_ORBIT:
            RotateOrbit(headingDegrees, pitchDegrees, rollDegrees);
            break;
        }

        UpdateViewMatrix();
    }

    template<typename T>
    void Camera<T>::RotateSmoothly(T headingDegrees, T pitchDegrees, T rollDegrees)
    {
        // This method applies a scaling factor to the rotation angles prior to
        // using these rotation angles to rotate the camera. This method is usually
        // called when the camera is being rotated using an input device (such as a
        // mouse or a joystick).

        headingDegrees *= rotationSpeed;
        pitchDegrees *= rotationSpeed;
        rollDegrees *= rotationSpeed;

        Rotate(headingDegrees, pitchDegrees, rollDegrees);
    }

    template<typename T>
    void Camera<T>::UndoRoll()
    {
        // Undo any camera rolling by leveling the camera. When the camera is
        // orbiting this method will cause the camera to become level with the
        // orbit target.

        if (behavior == CAMERA_BEHAVIOR_ORBIT)
            LookAt(eye, target, targetYAxis);
        else
            LookAt(eye, eye + viewDir, worldYAxis);
    }

    template<typename T>
    void Camera<T>::UpdatePosition(const Vector3<T> &direction, T elapsedTimeSec)
    {
        // Moves the camera using Newton's second law of motion. Unit mass is
        // assumed here to somewhat simplify the calculations. The direction vector
        // is in the range [-1,1].

        if (currentVelocity.SqrLength() != 0.0f)
        {
            // Only move the camera if the velocity vector is not of zero length.
            // Doing this guards against the camera slowly creeping around due to
            // Ting point rounding errors.

            Vector3<T> displacement = (currentVelocity * elapsedTimeSec) +
                (0.5f * acceleration * elapsedTimeSec * elapsedTimeSec);

            // Ting point rounding errors will slowly accumulate and cause the
            // camera to move along each axis. To prevent any unintended movement
            // the displacement vector is clamped to zero for each direction that
            // the camera isn't moving in. Note that the updateVelocity() method
            // will slowly decelerate the camera's velocity back to a stationary
            // state when the camera is no longer moving along that direction. To
            // account for this the camera's current velocity is also checked.

            if (direction.x == 0.0f && CloseEnough(currentVelocity.x, 0.0f))
                displacement.x = 0.0f;

            if (direction.y == 0.0f && CloseEnough(currentVelocity.y, 0.0f))
                displacement.y = 0.0f;

            if (direction.z == 0.0f && CloseEnough(currentVelocity.z, 0.0f))
                displacement.z = 0.0f;

            Move(displacement.x, displacement.y, displacement.z);
        }

        // Continuously update the camera's velocity vector even if the camera
        // hasn't moved during this call. When the camera is no longer being moved
        // the camera is decelerating back to its stationary state.

        UpdateVelocity(direction, elapsedTimeSec);
    }

    template<typename T>
    void Camera<T>::Zoom(T zoom, T minZoom, T maxZoom)
    {
        if (behavior == CAMERA_BEHAVIOR_ORBIT)
        {
            // Moves the camera closer to or further away from the orbit
            // target. The zoom amounts are in world units.

            orbitMaxZoom = maxZoom;
            orbitMinZoom = minZoom;

            Vector3<T> offset = eye - target;

            orbitOffsetDistance = offset.magnitude();
            offset.Normalise();
            orbitOffsetDistance += zoom;
            orbitOffsetDistance = std::min(std::max(orbitOffsetDistance, minZoom), maxZoom);

            offset *= orbitOffsetDistance;
            eye = offset + target;

            UpdateViewMatrix();
        }
        else
        {
            // For the other behaviors zoom is interpreted as changing the
            // horizontal field of view. The zoom amounts refer to the horizontal
            // field of view in degrees.

            zoom = std::min(std::max(zoom, minZoom), maxZoom);
            Perspective(zoom, aspectRatio, znear, zfar);
        }
    }

    template<typename T>
    void Camera<T>::SetAcceleration(const Vector3<T> &newAcceleration)
    {
        acceleration = newAcceleration;
    }

    template<typename T>
    void Camera<T>::SetBehavior(CameraBehavior newBehavior)
    {
        // Switch to a new camera mode (i.e., behavior).
        // This method is complicated by the fact that it tries to save the current
        // behavior's state prior to making the switch to the new camera behavior.
        // Doing this allows seamless switching between camera behaviors.

        CameraBehavior prevBehavior = behavior;

        if (prevBehavior == newBehavior)
            return;

        behavior = newBehavior;

        switch (newBehavior)
        {
        case CAMERA_BEHAVIOR_FIRST_PERSON:
            switch (prevBehavior)
            {
            default:
                break;

            case CAMERA_BEHAVIOR_FLIGHT:
                eye.y = firstPersonYOffset;
                UpdateViewMatrix();
                break;

            case CAMERA_BEHAVIOR_SPECTATOR:
                eye.y = firstPersonYOffset;
                UpdateViewMatrix();
                break;

            case CAMERA_BEHAVIOR_ORBIT:
                eye.x = savedEye.x;
                eye.z = savedEye.z;
                eye.y = firstPersonYOffset;
                orientation = savedOrientation;
                accumPitchDegrees = savedAccumPitchDegrees;
                UpdateViewMatrix();
                break;
            }

            UndoRoll();
            break;

        case CAMERA_BEHAVIOR_SPECTATOR:
            switch (prevBehavior)
            {
            default:
                break;

            case CAMERA_BEHAVIOR_FLIGHT:
                UpdateViewMatrix();
                break;

            case CAMERA_BEHAVIOR_ORBIT:
                eye = savedEye;
                orientation = savedOrientation;
                accumPitchDegrees = savedAccumPitchDegrees;
                UpdateViewMatrix();
                break;
            }

            UndoRoll();
            break;

        case CAMERA_BEHAVIOR_FLIGHT:
            if (prevBehavior == CAMERA_BEHAVIOR_ORBIT)
            {
                eye = savedEye;
                orientation = savedOrientation;
                accumPitchDegrees = savedAccumPitchDegrees;
                UpdateViewMatrix();
            }
            else
            {
                savedEye = eye;
                UpdateViewMatrix();
            }
            break;

        case CAMERA_BEHAVIOR_ORBIT:
            if (prevBehavior == CAMERA_BEHAVIOR_FIRST_PERSON)
                firstPersonYOffset = eye.y;

            savedEye = eye;
            savedOrientation = orientation;
            savedAccumPitchDegrees = accumPitchDegrees;

            targetYAxis = yAxis;

            Vector3<T> newEye = eye + zAxis * orbitOffsetDistance;
            Vector3<T> newTarget = eye;

            LookAt(newEye, newTarget, targetYAxis);
            break;
        }
    }

    template<typename T>
    void Camera<T>::SetCurrentVelocity(const Vector3<T> &newCurrentVelocity)
    {
        currentVelocity = newCurrentVelocity;
    }

    template<typename T>
    void Camera<T>::SetCurrentVelocity(T x, T y, T z)
    {
        currentVelocity = Vector3<T>(x, y, z);
    }

    template<typename T>
    void Camera<T>::SetOrbitMaxZoom(T newOrbitMaxZoom)
    {
        orbitMaxZoom = newOrbitMaxZoom;
    }

    template<typename T>
    void Camera<T>::SetOrbitMinZoom(T newOrbitMinZoom)
    {
        orbitMinZoom = newOrbitMinZoom;
    }

    template<typename T>
    void Camera<T>::SetOrbitOffsetDistance(T newOrbitOffsetDistance)
    {
        orbitOffsetDistance = newOrbitOffsetDistance;
    }

    template<typename T>
    void Camera<T>::SetOrientation(const Quaternion<T> &newOrientation)
    {
        Matrix<T> m = newOrientation.ToRotationMatrix();

        // Store the pitch for this new orientation.
        // First person and spectator behaviors limit pitching to
        // 90 degrees straight up and down.

        accumPitchDegrees = RadiansToDegrees(asinf(m(1, 2)));

        // First person and spectator behaviors don't allow rolling.
        // Negate any rolling that might be encoded in the new orientation.

        orientation = newOrientation;

        if (behavior == CAMERA_BEHAVIOR_FIRST_PERSON || behavior == CAMERA_BEHAVIOR_SPECTATOR)
            LookAt(eye, eye + viewDir, worldYAxis);

        UpdateViewMatrix();
    }

    template<typename T>
    void Camera<T>::SetPosition(const Vector3<T> &newEye)
    {
        eye = newEye;
        UpdateViewMatrix();
    }

    template<typename T>
    void Camera<T>::SetPreferTargetYAxisOrbiting(bool newPreferTargetYAxisOrbiting)
    {
        // Determines the behavior of Y axis rotations when the camera is
        // orbiting a target. When preferTargetYAxisOrbiting is true all
        // Y axis rotations are about the orbit target's local Y axis.
        // When preferTargetYAxisOrbiting is false then the camera's
        // local Y axis is used instead.

        preferTargetYAxisOrbiting = newPreferTargetYAxisOrbiting;

        if (preferTargetYAxisOrbiting)
            UndoRoll();
    }

    template<typename T>
    void Camera<T>::SetRotationSpeed(T newRotationSpeed)
    {
        // This is just an arbitrary value used to scale rotations
        // when rotateSmoothly() is called.

        rotationSpeed = newRotationSpeed;
    }

    template<typename T>
    void Camera<T>::SetVelocity(const Vector3<T> &newVelocity)
    {
        velocity = newVelocity;
    }

    template<typename T>
    void Camera<T>::SetVelocity(T x, T y, T z)
    {
        velocity = Vector3<T>(x, y, z);
    }

    template<typename T>
    void Camera<T>::RotateFirstPerson(T headingDegrees, T pitchDegrees)
    {
        // Implements the rotation logic for the first person style and
        // spectator style camera behaviors. Roll is ignored.

        accumPitchDegrees += pitchDegrees;

        if (accumPitchDegrees > 90.0f)
        {
            pitchDegrees = 90.0f - (accumPitchDegrees - pitchDegrees);
            accumPitchDegrees = 90.0f;
        }

        if (accumPitchDegrees < -90.0f)
        {
            pitchDegrees = -90.0f - (accumPitchDegrees - pitchDegrees);
            accumPitchDegrees = -90.0f;
        }

        Quaternion<T> rot;

        // Rotate camera about the world y axis.
        // Note the order the quaternions are multiplied. That is important!
        if (headingDegrees != 0.0f)
        {
            rot.FromAxisAngle(headingDegrees, worldYAxis);
            orientation = rot * orientation;
        }

        // Rotate camera about its local x axis.
        // Note the order the quaternions are multiplied. That is important!
        if (pitchDegrees != 0.0f)
        {
            rot.FromAxisAngle(pitchDegrees, worldXAxis);
            orientation = orientation * rot;
        }
    }

    template<typename T>
    void Camera<T>::RotateFlight(T headingDegrees, T pitchDegrees, T rollDegrees)
    {
        // Implements the rotation logic for the flight style camera behavior.

        accumPitchDegrees += pitchDegrees;

        if (accumPitchDegrees > 360.0f)
            accumPitchDegrees -= 360.0f;

        if (accumPitchDegrees < -360.0f)
            accumPitchDegrees += 360.0f;

        Quaternion<T> rot;

        rot.FromYawPitchRoll(headingDegrees, pitchDegrees, rollDegrees);
        orientation *= rot;
    }

    template<typename T>
    void Camera<T>::RotateOrbit(T headingDegrees, T pitchDegrees, T rollDegrees)
    {
        // Implements the rotation logic for the orbit style camera behavior.
        // Roll is ignored for target Y axis orbiting.
        //
        // Briefly here's how this orbit camera implementation works. Switching to
        // the orbit camera behavior via the setBehavior() method will set the
        // camera's orientation to match the orbit target's orientation. Calls to
        // rotateOrbit() will rotate this orientation. To turn this into a third
        // person style view the updateViewMatrix() method will move the camera
        // position back 'orbitOffsetDistance' world units along the camera's
        // local z axis from the orbit target's world position.

        Quaternion<T> rot;

        if (preferTargetYAxisOrbiting)
        {
            if (headingDegrees != 0.0f)
            {
                rot.FromAxisAngle(headingDegrees, targetYAxis);
                orientation = rot * orientation;
            }

            if (pitchDegrees != 0.0f)
            {
                rot.FromAxisAngle(pitchDegrees, worldXAxis);
                orientation = orientation * rot;
            }
        }
        else
        {
            rot.FromYawPitchRoll(headingDegrees, pitchDegrees, rollDegrees);
            orientation *= rot;
        }
    }

    template<typename T>
    void Camera<T>::UpdateVelocity(const Vector3<T> &direction, float elapsedTimeSec)
    {
        // Updates the camera's velocity based on the supplied movement direction
        // and the elapsed time (since this method was last called). The movement
        // direction is in the range [-1,1].

        if (direction.x != 0.0f)
        {
            // Camera is moving along the x axis.
            // Linearly accelerate up to the camera's max speed.

            currentVelocity.x += direction.x * acceleration.x * elapsedTimeSec;

            if (currentVelocity.x > velocity.x)
                currentVelocity.x = velocity.x;
            else if (currentVelocity.x < -velocity.x)
                currentVelocity.x = -velocity.x;
        }
        else
        {
            // Camera is no longer moving along the x axis.
            // Linearly decelerate back to stationary state.

            if (currentVelocity.x > 0.0f)
            {
                if ((currentVelocity.x -= acceleration.x * elapsedTimeSec) < 0.0f)
                    currentVelocity.x = 0.0f;
            }
            else
            {
                if ((currentVelocity.x += acceleration.x * elapsedTimeSec) > 0.0f)
                    currentVelocity.x = 0.0f;
            }
        }

        if (direction.y != 0.0f)
        {
            // Camera is moving along the y axis.
            // Linearly accelerate up to the camera's max speed.

            currentVelocity.y += direction.y * acceleration.y * elapsedTimeSec;

            if (currentVelocity.y > velocity.y)
                currentVelocity.y = velocity.y;
            else if (currentVelocity.y < -velocity.y)
                currentVelocity.y = -velocity.y;
        }
        else
        {
            // Camera is no longer moving along the y axis.
            // Linearly decelerate back to stationary state.

            if (currentVelocity.y > 0.0f)
            {
                if ((currentVelocity.y -= acceleration.y * elapsedTimeSec) < 0.0f)
                    currentVelocity.y = 0.0f;
            }
            else
            {
                if ((currentVelocity.y += acceleration.y * elapsedTimeSec) > 0.0f)
                    currentVelocity.y = 0.0f;
            }
        }

        if (direction.z != 0.0f)
        {
            // Camera is moving along the z axis.
            // Linearly accelerate up to the camera's max speed.

            currentVelocity.z += direction.z * acceleration.z * elapsedTimeSec;

            if (currentVelocity.z > velocity.z)
                currentVelocity.z = velocity.z;
            else if (currentVelocity.z < -velocity.z)
                currentVelocity.z = -velocity.z;
        }
        else
        {
            // Camera is no longer moving along the z axis.
            // Linearly decelerate back to stationary state.

            if (currentVelocity.z > 0.0f)
            {
                if ((currentVelocity.z -= acceleration.z * elapsedTimeSec) < 0.0f)
                    currentVelocity.z = 0.0f;
            }
            else
            {
                if ((currentVelocity.z += acceleration.z * elapsedTimeSec) > 0.0f)
                    currentVelocity.z = 0.0f;
            }
        }
    }

    template<typename T>
    void Camera<T>::UpdateViewMatrix()
    {
        // Reconstruct the view matrix.
        viewMatrix = orientation.ToRotationMatrix();

        xAxis = Vector3<T>(viewMatrix(0, 0), viewMatrix(1, 0), viewMatrix(2, 0));
        yAxis = Vector3<T>(viewMatrix(0, 1), viewMatrix(1, 1), viewMatrix(2, 1));
        zAxis = Vector3<T>(viewMatrix(0, 2), viewMatrix(1, 2), viewMatrix(2, 2));
        viewDir = -zAxis;

        if (behavior == CAMERA_BEHAVIOR_ORBIT)
        {
            // Calculate the new camera position based on the current
            // orientation. The camera must always maintain the same
            // distance from the target. Use the current offset vector
            // to determine the correct distance from the target.

            eye = target + zAxis * orbitOffsetDistance;
        }

        viewMatrix.SetElement(3, 0, -Vector3<T>::Dot(xAxis, eye));
        viewMatrix.SetElement(3, 1, -Vector3<T>::Dot(yAxis, eye));
        viewMatrix.SetElement(3, 2, -Vector3<T>::Dot(zAxis, eye));
    }

}

}

#endif
