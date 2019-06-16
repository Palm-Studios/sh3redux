/** @file
 *
 *  Implementation of config.hpp
 *
 *  @copyright 2016-2019  Palm Studios
 *
 *  @date 31-3-2019
 *
 *  @author Jesse Buhagiar
 */
#include "SH3/system/config.hpp"
#include "SH3/system/log.hpp"

#include <algorithm>
#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include <boost/algorithm/string.hpp>

using namespace sh3::system;

void CConfigurationFile::Load()
{
    std::string     line;
    std::uint64_t   byteIndex = 0;

    cfgFile.open(FILENAME);

    // It seems the file does not exist... Oops!
    if(!cfgFile.is_open())
    {
        Log(LogLevel::ERROR, "Unable to find configuration file! Reverting to default values...");
        return;
    }

    while(std::getline(cfgFile, line))
    {
        byteIndex += line.length(); // Get the length of the current line
        if(line.length() > 0)
        {
            // This line is a comment
            if(line.at(0) == '/' && line.at(1) == '/')
                continue;

            // This line is the start of a configuration section
            if(line.at(0) == '[' && line.at(line.length() - 1) == ']')
            {
                sectionMap.insert(std::make_pair(line, byteIndex)); // Index the beginning of this section. This way we can automatically seek to the correct location
            }
        }
    }

    cfgFile.close();
}

std::int64_t CConfigurationFile::GetSectionOffset(const std::string& sectionName) const
{
    std::int64_t            ret = -1;

    auto search = sectionMap.find(sectionName);
    if(search != sectionMap.end())
        ret = search->second;

    return ret;
}

template <>
bool CConfigurationFile::GetConfigurationValue(const std::string& section, const std::string& option)
{
    bool        ret = BOOL_DEFAULT_VAL;
    int64_t     offset = GetSectionOffset(section);
    std::string line;
    std::vector<std::string> split;

    if(offset == -1)
        return ret;

    cfgFile.open(FILENAME, std::ios_base::openmode::_S_in);
    cfgFile.seekg(offset, std::ios::beg); // Seek to the section location
    while(std::getline(cfgFile, line))
    {
        /**
        if(line.size() < 2)
        {
            Log(LogLevel::ERROR, "CConfigurationFile::GetConfigurationValue<bool>: line.size() < 2, using default value!);");
            break;
        }
        **/

        boost::split(split, line, boost::algorithm::is_any_of(" "));
        if(split.at(0) == option)
        {
            ret = static_cast<bool>(std::stoi(split.at(1)));
            break;
        }
    }

    return ret;
}

template <>
int CConfigurationFile::GetConfigurationValue(const std::string& section, const std::string& option)
{
    int         ret = INT_DEFAULT_VAL;
    int64_t     offset = GetSectionOffset(section);
    std::string line;
    std::vector<std::string> split;

    if(offset == -1)
        return ret;

    cfgFile.open(FILENAME, std::ios_base::openmode::_S_in);
    cfgFile.seekg(offset, std::ios::beg); // Seek to the section location
    while(std::getline(cfgFile, line))
    {
        /**
        if(line.size() < 2)
        {
            Log(LogLevel::ERROR, "CConfigurationFile::GetConfigurationValue<bool>: line.size() < 2, using default value!);");
            break;
        }
        **/

        boost::split(split, line, boost::algorithm::is_any_of(" "));
        if(split.at(0) == option)
        {
            ret = static_cast<int>(std::stoi(split.at(1)));
            break;
        }
    }

    return ret;
}

template <>
float CConfigurationFile::GetConfigurationValue(const std::string& section, const std::string& option)
{
    float       ret = FLOAT_DEFAULT_VAL;
    int64_t     offset = GetSectionOffset(section);
    std::string line;
    std::vector<std::string> split;

    if(offset == -1)
        return ret;

    cfgFile.open(FILENAME, std::ios_base::openmode::_S_in);
    cfgFile.seekg(offset, std::ios::beg); // Seek to the section location
    while(std::getline(cfgFile, line))
    {
        /**
        if(line.size() < 2)
        {
            Log(LogLevel::ERROR, "CConfigurationFile::GetConfigurationValue<bool>: line.size() < 2, using default value!);");
            break;
        }
        **/

        boost::split(split, line, boost::algorithm::is_any_of(" "));
        if(split.at(0) == option)
        {
            ret = static_cast<float>(std::stof(split.at(1)));
            break;
        }
    }

    return ret;
}
