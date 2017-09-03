/** @file
 *
 *  OpenGL Version of the SILENT HILL style camera.
 *
 *  The camera works on a few basic principles, firstly, we consider that Heather (or James),
 *  is the origin that the camera is <i>always</i> looking at. The camera can be in it's own position,
 *  however for the most part (not including cutscenes), we are always looking at the protagonist.
 *
 *  Next, the camera seems to be in its own 'volume' (for lack of a better term), in that it <i>can't</i>
 *  under any circumstances clip through a wall. There's a few links (albeit for different game engines) that
 *  suggest how it is possible to do this, found here:
 *  <a>https://www.reddit.com/r/Unity3D/comments/38qsx1/camera_collision_silent_hill_style/?st=j71bhmzc&sh=28a00dc6</a>
 *  <a>https://answers.unrealengine.com/questions/169933/set-up-a-camera-like-silent-hill-and-resident-evil.html</a>
 *
 *  I googled <a href = https://www.google.com.au/search?q=silent+hill+style+camera&oq=silent+hill+style+camera&aqs=chrome..69i57j69i60j69i57j69i65j69i60l2.2672j0j7&sourceid=chrome&ie=UTF-8>this</a> term.
 *
 *  It would seem that camera data is stored in .cam files in the arc sections. I assume that this data is fed into the camera
 *  when some kind of trigger (an AABB??) is triggered when Heather walks over/through it. It is currently
 *  unknown where this data is stored, however, Mike seems to think it is in the .cld files (Collision & Lighting Data [CLD]).
 *
 *
 *  There are a few modes the camera can be in:
 *      @ref STATIC - Static camera view, usually in tight spots (the bookstore store room).
 *      @ref FOLLOW - Follows Heather around (outside areas).
 *  @copyright 2017 Palm Studios
 *
 *  @date 1-9-2017
 *
 *  @author Jesse Buhagiar
 */
#ifndef CAMERA_HPP_INCLUDED
#define CAMERA_HPP_INCLUDED

#include "glm/glm.hpp"
#include "SH3/angle.hpp"

namespace sh3{namespace camera{

    /**
     *  Camera Modes.
     */
    enum class MODE
    {
        //FOLLOW,
        //STATIC,
        //CYLINDER_DEBUG,
        FIRST_PERSON
    };

    /**
     *  Our camera in 3D space. Keeps track of position, intended position, rotation etc.
     *
     *  Supports clipping in a 3D scene (that is, it can collide as well as a 3D model).
     */
    struct Camera final
    {
    public:

        /**
         *  Constructor
         *
         *  @param pos Initial position of the camera.
         *  @param fov Camera Horizontal field of view.
         *  @param aspect Aspect ratio of screen.
         *  @param zNear Near Z value for frustum culling.
         *  @param zFar Far Z value for frustum culling.
         */
        Camera(const glm::vec3& pos, angle<float> fov, float aspect, float zNear, float zFar);

        /**
         *  Set the physical position of the camera.
         *
         *  @param pos Position vector in 3D space we want the camera at.
         */
        void SetPosition(const glm::vec3& pos);

        /**
         *  Look at position @ref look in 3D space.
         *
         *  @param look 3D vector representing the position we want the camera to look at.
         */
        void LookAt(const glm::vec3& look);

        /**
         *  Translates the cameras current position by adding to the position vector.
         *
         *  @param trans Our translation vector.
         */
        void Translate(const glm::vec3& trans);

        /**
         *  Translates the cameras current position by adding the camera's Front Vector,
         *  @ref camFront, and then multiplying by a scale (which effectively chooses)
         *  the direction.
         *
         *  @param factor Translation factor.
         */
        void Translate(const float factor);

        /**
         *  Set the mode of the camera.
         *
         *  @param @ref MODE we want the camera to be in.
         */
        void SetMode(MODE mode);

        /**
         *  Set the horizontal FOV of the camera's projection matrix.
         *
         *  @param fov Field of view we want the camera to be.
         */
        void SetFOV(angle<float> fov);

        /**
         *  Yaw the camera. There is currently no protection against gimbal lock!
         *
         *  @param yaw Yaw to add to the camera's current yaw.
         */
        void AddYaw(angle<float> yaw);

        /**
         *  Pitch the camera. There is currently no protection against gimbal lock!
         *
         *  @note Pitch is clamped between [-89, 89] degrees!
         *
         *  @param pitch Pitch to add to the camera's current pitch.
         */
        void AddPitch(angle<float> pitch);

        /**
         *  Calculate the projection matrix of our camera by using glm::lookat.
         *
         *  @return glm::mat4 that is our projection matrix. Sent to the shader as part of the MVP matrix (it is the P part).
         */
        const glm::mat4& GetProjectionMatrix() const;

        #if 0
        /**
         *  Update the camera so we know where we should be each frame.
         */
         void Update();
        #endif

        /**
         *  Get x co-ordinate to camera position.
         */
        float GetX() const;

        /**
         *  Get y co-ordinate to camera position.
         */
        float GetY() const;

        /**
         *  Get z co-ordinate to camera position.
         */
        float GetZ() const;

        /**
         *  Get the current yaw of the camera in degrees.
         *
         *  @return @ref angle representing the current yaw of the camera.
         */
        angle<float> GetYaw() const;

        /**
         *  Get the current pitch of the camera in degrees.
         *
         *  @return @ref angle representing the current pitch of the camera.
         */
        angle<float> GetPitch() const;

    private:

        /**
         *  Recalculate the perspecitve matrix from updated values;
         */
        void RecalculatePerspective();

    private:
        glm::vec3       camPos;             /**< Position of the physical camera in 3D space. */
        angle<float>    camFov;             /**< Camera's Field of View. */
        float           aRatio;             /**< Aspect ratio for the camera. */
        float           camNear;            /**< Near Z value for frustum culling. */
        float           camFar;             /**< Far Z value for frustim culling (the reason we have fog!!) */
        glm::vec3       camFront;           /**< Where the camera is looking. Most likely Heather. (This is actually reversed!)*/
        glm::vec3       camUp;              /**< Up vector of the camera (not the actual world). */
        glm::mat4       pMatrix;            /**< Our camera's Perspective matrix. */
        //glm::mat4       vMatrix;            /**< Our camera's View Matrix. */
        glm::vec3       camRight;           /**< Camera's Right vector. */

        angle<float>    camPitch;           /**< Camera's pitch (angle on the y-axis). */
        angle<float>    camYaw;             /**< Camera's yaw (angle on the x-axis). */
        MODE            camMode;            /**< What mode the camera is currently in. */

        // bool shouldReset;       /**< Should we refresh the perspective matrix??? */

        /** Camera Physics Variables and methods */

    public:
        static constexpr double FRICTION_COEFFICIENT = 0.07;    /**< Camera Friction coefficient! TODO: Play with me! */

    private:
        // double          camVelocity;        /**< Current camera velocity */


    };
}}

#endif // CAMERA_HPP_INCLUDED
