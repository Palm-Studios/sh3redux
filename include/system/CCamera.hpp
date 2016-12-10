/*++

Copyright (c) 2016  Palm Studios

Module Name:
        CCamera.hpp

Abstract:
        Emulation of the Classic Silent Hill style "follow camera"
        (although it's much rather like a director's camera in a movie than a follow cam).
        The camera needs to be able to follow the player model as well as detect clipping at angles
        so that it doesn't go through a wall/ceiling.

        If we are referring to the camera in terms of following Heather, basically it sits in 3D space
        and follows her, as well as switching angles/location according to where she is currently
        located in the map. The camera can also (at any time) be moved behind Heather and moved as if
        she is looking around with her head (sort of like a camera pan/2D translation). This is done by
        holding TAB in the PC version.

        The math behind this should be as simple as c^2=a^2+b^2 (i.e, c = sqrt(cam_height^2 + distance_to_heather^2)),
        so the hard part really should be efficiently detecting clipping and smooth transitions
        (as well as transitions in general).

        In terms of a cutscene ("Dad, it's me.."), we need a way of storing a list of camera translations/effects
        according to a specific time. Each bit of dialogue is actually in one file (i.e, the "Hey dad, it's me scene
        is actually in a single file, which makes life a lot easier). This could probably be done with json parsing
        or something similar (maybe even a text file with duration/effect/position???).

        Apparently the animation timings/transitions are in some .kg2 file (according to the Author of the map/model ripper).
        If this is the case, we need to pass this data into the camera every frame to update.


Author:
        Quaker762

Environment:

Notes:
        A quick note on co-ordinates (for OpenGL! :^])

        X-Axis: Left and right
        Y-Axis: Up and down
        Z-Axis: Towards and away from the user

Revision History:
        6-12-2016: File created (no implementation at all yet)

--*/
#ifndef CCAMERA_HPP_INCLUDED
#define CCAMERA_HPP_INCLUDED

#define CAMERA_DIRECTOR_MODE    0 // Director mode, i.e, cutscenes
#define CAMERA_GAME_MODE        1 // Game mode, regular camera mode

#include <GL/gl.h>
#include <glm/glm.hpp>

class CCamera
{
public:
    CCamera();
    ~CCamera();


private:
    int mode; // Camera mode we are currently in (director, auto, manual etc)

    glm::vec3 cvec; // Camera position in 3D space
};



#endif // CCAMERA_HPP_INCLUDED
