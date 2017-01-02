/** @file
 *
 *  Implementation of functions found in the class sh3_arc_file
 *
 *  \copyright 2016  Palm Studios
 *
 *  \note
 *
 *  \date 1-1-2017
 *
 *  \author Alexander Hirsch
 */
#include "SH3/arc/file.hpp"

#include <cassert>
#include <cerrno>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

void sh3_arc_file::read_error::set_error(sh3_arc_file::read_result res, gzFile file)
{
    result = res;
    zlib_err = Z_OK;
    os_err = 0;
    if(result == sh3_arc_file::read_result::GZError)
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
    case sh3_arc_file::read_result::Success:
        error = "Success";
    case sh3_arc_file::read_result::Eof:
        error = "End of file";
    case sh3_arc_file::read_result::PartialRead:
        error = "Partial read";
    case sh3_arc_file::read_result::GZError:
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

/**
 *  Read binary data from an arc file to a buffer.
 *
 *  @param destination - destination buffer
 *  @param len - amount of bytes to read
 *
 *  @returns \c sh3_arc_file::read_result
*/
size_t sh3_arc_file::ReadData(void* destination, std::size_t len, read_error& e)
{
    assert(static_cast<int>(len) > 0); // overflow check

    int res = gzread(gzHandle.get(), destination, len);
    assert(res <= static_cast<int>(len));

    if(res == static_cast<int>(len))
    {
        e.set_error(read_result::Success, nullptr);
    }
    else if(res > 0)
    {
        //TODO: return res also, so that caller knows how much was read
        e.set_error(read_result::PartialRead, nullptr);
    }
    else if(res == 0)
    {
        e.set_error(read_result::Eof, nullptr);
    }
    else
    {
        res = 0;
        e.set_error(read_result::GZError, gzHandle.get());
    }

    return res;
}

/**
 *  Read a string from an arc file.
 *
 *  @param destination - destination string
 *  @param len - amount of bytes to read
 *
 *  @returns \c sh3_arc_file::read_result.
*/
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
