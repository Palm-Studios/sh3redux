/** @file
 *
 *  SILENT HILL 3: Redux Configuration parser
 *
 *  \copyright 2016  Palm Studios
 *
 *  \note
 *
 *  \date 27-12-2016
 *
 *  \author Jesse Buhagiar
 */
#ifndef SH3_CONFIG_H_INCLUDED
#define SH3_CONFIG_H_INCLUDED

#include <map>

/**
 * Config file class.
 */
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
