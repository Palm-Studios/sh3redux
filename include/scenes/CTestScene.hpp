/*++

Copyright (c) 2016  Palm Studios

Module Name:
        CTestScene.hpp

Abstract:
        Test scene (sort of like a playground for basic features!)

Author:
        Quaker762

Environment:

Notes:

Revision History:


--*/
#ifndef CTESTSCENE_HPP_INCLUDED
#define CTESTSCENE_HPP_INCLUDED

#include <GL/glew.h>
#include <GL/gl.h>

#include <system/CScene.hpp>
#include <system/CGameInstance.hpp>

class CTestScene : public CScene
{
public:
    CTestScene(CGameInstance* instance);
    virtual ~CTestScene();


    void render();
    void update();

private:
    CGameInstance*  instance;

    GLuint          vertexbuffer;

};

#endif // CTESTSCENE_HPP_INCLUDED
