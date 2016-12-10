/*++

Copyright (c) 2016  Palm Studios

Module Name:
        CGameInstance.hpp

Abstract:
        Defines the game instance class. This handles everything from input
        to the actual loop.

Author:
        Quaker762

Environment:

Notes:

Revision History:
        29-11-16: Initial Revision

--*/
#ifndef CGAMEINSTANCE_HPP_INCLUDED
#define CGAMEINSTANCE_HPP_INCLUDED

#include <system/CWindow.hpp>
#include <stdtype.hpp>

class CGameInstance
{
public:
    CGameInstance();
    ~CGameInstance();

    void init();
    void quit();

    void winSwap();

private:
    CWindow*            window;

    bool running;

    void loop();
    void handleInput();

};


#endif // CGAMEINSTANCE_HPP_INCLUDED
