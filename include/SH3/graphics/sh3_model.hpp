/*++

Copyright (c) 2016  Palm Studios and Mike M (@perdedork)

Module Name:
        sh3_model.hpp

Abstract:
        Data structures and class to load and draw a SILENT HILL 3 Model file

Author:
        Quaker762

Environment:

Notes:

Revision History:
        27-12-2016: File Created                                                [Quaker762]

--*/
#ifndef SH3_MODEL_HPP_INCLUDED
#define SH3_MODEL_HPP_INCLUDED

#include <cstdint>

typedef struct
{
    uint32_t unused1;               // Unused DWORD
    uint32_t modelID;               // Model Identifier (not sure how this was used yet)
    uint32_t numTextures;           // Number of textures for this model (1 model can contain many textures, i.e, Heather and Memory of Alessa)
    uint32_t textureDataoffset;     // Offset to the start of the (first??) texture.
    uint32_t headerSize;            // Size of this header
    uint32_t unused2;               // Unused DWORD
    uint32_t unused[8];             // Jesus Christ, KONAMI.....
} sh3_model_header_t;



#endif // SH3_MODEL_HPP_INCLUDED
