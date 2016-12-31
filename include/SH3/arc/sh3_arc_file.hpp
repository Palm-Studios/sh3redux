/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3_arc_file.hpp

Abstract:
        Functions to extract data from arc files.

Author:
        Alexander Hirsch

Environment:

Notes:

Revision History:

--*/
#ifndef SH3_ARC_FILE_HPP_INCLUDED
#define SH3_ARC_FILE_HPP_INCLUDED

#include <zlib.h>

#include <string>
#include <type_traits>
#include <memory>

#include <SH3/stdtype.hpp>

struct sh3_arc_file final
{
public:
    enum class read_result
    {
        Success,
        Eof,
        PartialRead,
        Error,
    };

    sh3_arc_file(const std::string& path);

    bool is_open() const {return static_cast<bool>(gzHandle);}

    read_result ReadData(void* destination, size_t len);
    template<typename T, typename = std::enable_if<std::is_standard_layout<T>::value>>
    read_result ReadObject(T& destination, size_t len = sizeof(T))
    {
        return ReadData(&destination, len);
    }
    read_result ReadString(std::string& destination, size_t len);

private:
    std::unique_ptr<gzFile_s, int(&)(gzFile)> gzHandle;
};

#endif // SH3_ARC_FILE_HPP_INCLUDED