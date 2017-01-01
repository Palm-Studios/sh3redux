/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3_config.hpp

Abstract:
        SILENT HILL 3: Redux Configuration parser

Author:
        Jesse Buhagiar

Environment:

Notes:

Revision History:
        27-12-2016: File Created                                                                    [jbuhagiar]
                    Base class created

--*/

#ifndef SH3_CONFIG_H_INCLUDED
#define SH3_CONFIG_H_INCLUDED

#include <map>

class sh3_config
{
public:
    sh3_config(){};
    ~sh3_config(){};

    int Load();
    int GetOptionValue(std::string option);

private:
    std::map<std::string, int> values;
};


#endif // SH3_CONFIG_H_INCLUDED
