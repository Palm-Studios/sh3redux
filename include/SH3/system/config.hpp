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

#include <string>
#include <unordered_map>
#include <cstdint>
#include <fstream>


namespace sh3 { namespace system {

/**
 *  SILENT HILL 3 Configuration File'
 *
 *  SILENT HILL 3 does natively come with a configuration file, instead opting to use
 *  some kind of bizarro binary @c .sys file format. This is a workaround to that, presenting all
 *  possible options in one single place, @c shr.cfg.
 *
 *  The search for a configuration value is sped up by storing them in 'sections', similar to
 *  an @c .ini file. This way we don't have to search the configuration file (which would
 *  be at worst case, O(n)) each time during run time to find an option value. The search for
 *  a configuration section is O(1).
 */
class CConfigurationFile
{
public:
    static constexpr char*  FILENAME            = "sh3r.cfg"; /**< Configuration file name */
    static constexpr bool   BOOL_DEFAULT_VAL    = false;
    static constexpr int    INT_DEFAULT_VAL     = 0;
    static constexpr float  FLOAT_DEFAULT_VAL   = 0.0f;

public:
    /**
     * Constructor
     */
    CConfigurationFile(){}

    /**
     * Load the file from the disk, and populate the section map
     */
    void Load();

    /**
     * Get a value from the configuration file. The section it resides in MUST
     * be specified.
     *
     * @tparam T        Type we want to return (interpreted from options)
     * @param section   Section this option is located in
     *
     * @return Option value
     *
     * @warning This function <i>could</i> cause a crash if an invalid type if selected. Double check you
     * have the correct type before using this function.
     */
    template <typename T>
    T GetConfigurationValue(const std::string& section, const std::string& optionName);

private:

    /**
     * Get byte offset of [section]
     *
     * @param sectionName Name of section we want to find
     *
     * @returns Byte offset of section on success, -1 on failure
     */
    std::int64_t GetSectionOffset(const std::string& sectionName) const;

private:
    std::ifstream cfgFile;
    std::unordered_map<std::string, std::uint64_t> sectionMap;
};

}}



#endif // SH3_CONFIG_H_INCLUDED
