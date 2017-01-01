/*++

Copyright (c) 2016  Palm Studios

Module Name:
        sh3_arc_file.cpp

Abstract:
        Implementation of functions found in the class sh3_arc_file

Author:
        Alexander Hirsch

Environment:

Notes:

Revision History:

--*/
#include "SH3/arc/file.hpp"

#include <cassert>
#include <cstddef>
#include <iterator>
#include <vector>

sh3_arc_file::sh3_arc_file(const std::string& path)
    :gzHandle(gzopen(path.c_str(), "rb"))
{
}

/*++

Routine Description:
        Read binary data from an arc file to a buffer.

Arguments:
        destination - destination buffer
        len - amount of bytes to read

Return Type:
        sh3_arc_file::read_result

--*/
sh3_arc_file::read_result sh3_arc_file::ReadData(void* destination, std::size_t len)
{
    assert(static_cast<int>(len) > 0); // overflow check

    int res = gzread(gzHandle.get(), destination, len);
    assert(res <= static_cast<int>(len));

    if(res == static_cast<int>(len))
    {
        return read_result::Success;
    }
    if(res > 0)
    {
        //TODO: return res also, so that caller knows how much was read
        return read_result::PartialRead;
    }
    if(res == 0)
    {
        return read_result::Eof;
    }
    return read_result::Error;
}


/*++

Routine Description:
        Read a string from an arc file.

Arguments:
        destination - destination string
        len - amount of bytes to read

Return Type:
        sh3_arc_file::read_result

--*/
sh3_arc_file::read_result sh3_arc_file::ReadString(std::string& destination, std::size_t len)
{
    assert(static_cast<int>(len) > 0); // overflow check

    std::vector<char> buf(len);
    read_result res = ReadData(buf.data(), buf.size());

    if(res == read_result::Success)
    {
        destination.assign(begin(buf), end(buf));
    }
    else if(res == read_result::PartialRead)
    {
        //FIXME: assign partial string
        destination.clear();
    }
    return res;
}
