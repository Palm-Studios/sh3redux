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
#include <string>

class sh3_config
{
public:
    int Load();
    int GetOptionValue(const std::string& option);

private:
    std::map<std::string, int> values;
};

#endif // SH3_CONFIG_H_INCLUDED
