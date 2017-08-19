/** @file
 *  Functions to extract data from @c arc.arc.
 *  
 *  @copyright 2016-2017  Palm Studios
 */
#include "SH3/arc/mft.hpp"

#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <limits>
#include <string>
#include <vector>

using namespace sh3::arc;

void mft::read_error::set_error(mft::read_result res, gzFile file)
{
    result = res;
    zlib_err = Z_OK;
    os_err = 0;
    if(result == mft::read_result::GZ_ERROR)
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

std::string mft::read_error::message() const
{
    std::string error;
    switch(result)
    {
    case read_result::SUCCESS:
        error = "Success";
        break;
    case read_result::END_OF_FILE:
        error = "End of file";
        break;
    case read_result::PARTIAL_READ:
        error = "Partial read";
        break;
    case read_result::GZ_ERROR:
        error = "GZip error: ";
        error += zError(zlib_err);
        if(zlib_err == Z_ERRNO)
        {
            error += ": ";
            error += std::strerror(os_err);
        }
        break;
    }
    return error;
}

mft::mft(const std::string& path)
    :gzHandle(gzopen(path.c_str(), "rb"))
{
}

std::size_t mft::ReadData(void* destination, std::size_t len, read_error& e)
{
    assert(std::numeric_limits<int>::max() >= len); // overflow check
    int ilen = static_cast<int>(len);

    int res = gzread(gzHandle.get(), destination, static_cast<unsigned>(ilen));
    assert(res <= ilen);

    if(res == ilen)
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

    static_assert(std::numeric_limits<std::size_t>::max() > std::numeric_limits<int>::max(), "std::size_t must be able to represent all positive int values");
    assert(res >= 0); // overflow check
    return static_cast<std::size_t>(res);
}

std::size_t mft::ReadString(std::string& destination, std::size_t len, read_error& e)
{
    assert(static_cast<int>(len) > 0); // overflow check

    std::vector<char> buf(len);
    std::size_t res = ReadData(buf.data(), buf.size(), e);
    assert(res <= buf.size());

    auto from = begin(buf);
    auto to = from;
    advance(to, res);
    destination.assign(from, to);
    assert(destination.size() == res);

    return res;
}
