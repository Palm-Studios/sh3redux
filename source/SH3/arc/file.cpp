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
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

void sh3_arc_file::read_error::set_error(sh3_arc_file::read_result res, gzFile file)
{
    result = res;
    zlib_err = Z_OK;
    os_err = 0;
    if(result == sh3_arc_file::read_result::GZ_ERROR)
    {
        assert(file);
        gzerror(file, &zlib_err);
        assert(zlib_err != Z_OK);
        if(zlib_err == Z_ERRNO)
        {
            os_err = errno;
            assert(os_err != 0);
        }
    }
}

std::string sh3_arc_file::read_error::message() const
{
    std::string error;
    switch(result)
    {
    case sh3_arc_file::read_result::SUCCESS:
        error = "Success";
    case sh3_arc_file::read_result::END_OF_FILE:
        error = "End of file";
    case sh3_arc_file::read_result::PARTIAL_READ:
        error = "Partial read";
    case sh3_arc_file::read_result::GZ_ERROR:
        error = "GZip error: ";
        error += zError(zlib_err);
        if(zlib_err == Z_ERRNO)
        {
            error += ": ";
            error += std::strerror(os_err);
        }
    }
    return std::move(error);
}

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
size_t sh3_arc_file::ReadData(void* destination, std::size_t len, read_error& e)
{
    assert(static_cast<int>(len) > 0); // overflow check

    int res = gzread(gzHandle.get(), destination, len);
    assert(res <= static_cast<int>(len));

    if(res == static_cast<int>(len))
    {
        e.set_error(read_result::SUCCESS, nullptr);
    }
    else if(res > 0)
    {
        //TODO: return res also, so that caller knows how much was read
        e.set_error(read_result::PARTIAL_READ, nullptr);
    }
    else if(res == 0)
    {
        e.set_error(read_result::END_OF_FILE, nullptr);
    }
    else
    {
        res = 0;
        e.set_error(read_result::GZ_ERROR, gzHandle.get());
    }

    return res;
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
std::size_t sh3_arc_file::ReadString(std::string& destination, std::size_t len, read_error& e)
{
    assert(static_cast<int>(len) > 0); // overflow check

    std::vector<char> buf(len);
    std::size_t res = ReadData(buf.data(), buf.size(), e);
    assert(res <= buf.size());

    auto beg = begin(buf);
    destination.assign(beg, next(beg, res));
    assert(destination.size() == res);

    return res;
}
