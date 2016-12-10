/*++

Copyright (c) 2016  Palm Studios

Module Name:
        CConfiguration.hpp

Abstract:
        A basic configuration file manager

Author:
        Quaker762

Environment:

Notes:

Revision History:
        29-11-2016: Initial revision

--*/
#ifndef CCONFIGURATION_HPP_INCLUDED
#define CCONFIGURATION_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <map>

class CConfiguration
{
public:
    CConfiguration();
    ~CConfiguration();

    int get(std::string key);

    void parse();
    void open(std::string fname);
    void close();

    bool fileExists();

protected:

private:
    std::ifstream cfg_file;
    std::map<std::string, int> values;

    bool exists;

};



#endif // CCONFIGURATION_HPP_INCLUDED
