/** @file
 *  SILENT HILL 3: Redux Configuration parser
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 27-12-2016
 *
 *  @author Jesse Buhagiar
 */
#ifndef SH3_CONFIG_H_INCLUDED
#define SH3_CONFIG_H_INCLUDED

#include <map>
#include <string>

/**
 *  Interface for configuration file
 */
class sh3_config
{
public:

    /**
     *  Load the configuration file
     *
     *  @return Number of strings read from the file.
     */
    int Load();

    /**
     *  Retrieve a value from an option string
     *
     *  @return Value of option
     */
    int GetOptionValue(const std::string& option);

private:
    std::map<std::string, int> values;
};

#endif // SH3_CONFIG_H_INCLUDED
