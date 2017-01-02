/*++

Copyright (c) 2016  Palm Studios

Module Name:
        stdtype.hpp

Abstract:
        Defines some common types, static functions and macros commonly used.

        All other headers/source files should include this header!

Author:
        Jesse Buhagiar

Environment:

Notes:

Revision History:
        13-12-2016: Initial Revision [jbuhagiar]
        14-12-2016: Added messagebox( ) function [jbuhagiar]
        17-12-2016: Added safedel( ) template function [jbuhagiar]

--*/
#ifndef STDTYPE_HPP_INCLUDED
#define STDTYPE_HPP_INCLUDED

#include "SH3/system/log.hpp"

/************************************************************/
/*                   TYPE DEFINITIONS                       */
/* Defines a few common types that are used throughout      */
/* the Engine                                               */
/************************************************************/

#define SH_OK   0
#define SH_BAD  -1

typedef int SHSTATUS; // Return type for functions

#endif // STDTYPE_HPP_INCLUDED
