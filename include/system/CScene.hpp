/*++

Copyright (c) 2016  Palm Studios

Module Name:
        CScene.hpp

Abstract:
        A scene is basically it's own self contained little game state. It deals with updating, drawing
        etcetera. For example, the main menu is one scene and the actual game is another. For example,
        the up and down arrows need to move a cursor in the inventory and main menu, but in game,
        we may want to map them to actually move the character!

Author:
        Quaker762

Environment:

Notes:

Revision History:
        30-11-2016: Initial Revision

--*/
#ifndef CSCENE_HPP_INCLUDED
#define CSCENE_HPP_INCLUDED

class CScene
{
public:

    virtual ~CScene() {}

    virtual void render() = 0;
    virtual void update() = 0;

protected:

private:

};




#endif // CSCENE_HPP_INCLUDED
