/** @file
 *
 *  Implementation of sh3_arc_vfile.hpp
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 5-1-2017
 *
 *  @author Jesse Buhagiar
 */
#include <SH3/arc/vfile.hpp>
#include <SH3/arc/types.hpp>
#include <SH3/system/log.hpp>
#include <cstring>
#include <cassert>

bool sh3_arc_vfile::Open(sh3_arc& mft, const std::string& filename)
{
    if(open) return false;

    /*
        Load the file from the section it is, and set our local fsize to
        it (so we know how large it is without probing) though most headers contain the size of the
        full file
    */
    int size = mft.LoadFile(filename, buffer);
    if(size == ARC_FILE_NOT_FOUND)
    {
        open = false;
    }
    else
    {
        assert(size >= 0);
        fsize = static_cast<unsigned>(size);

        open = true;
    }
    return open;
}

std::string sh3_arc_vfile::read_error::message() const
{
    std::string error;
    switch(result)
    {
    case load_result::SUCCESS:
        error = "Success";
        break;
    case load_result::END_OF_FILE:
        error = "EOF";
        break;
    case load_result::PARTIAL_READ:
        error = "Partial Read";
        break;
    }
    return error;
};

void sh3_arc_vfile::Seek(long pos, std::ios_base::seekdir origin)
{
    if(origin == std::ios_base::beg)
    {
        fpos = static_cast<decltype(fpos)>(pos);
    }
    else if(origin == std::ios_base::cur)
    {
        // This works even for negative numbers.
        // fpos will overflow in that case, but that's fine because unsigned overflow is well defined and does the right thing here.
        static_assert(std::is_unsigned<decltype(fpos)>::value, "fpos might overflow, so it needs to be unsigned for the following operation.");
        fpos += static_cast<decltype(fpos)>(pos);
    }
    //FIXME: std::ios_base::end

    // Log if there is an attempt at something naughty
    if(fpos > fsize)
    {
        Log(LogLevel::WARN, "sh3_arc_vfile::Seek( ): Attempt to seek to position > fsize! Clamping to fsize!");
        fpos = fsize;
    }
}

std::size_t sh3_arc_vfile::ReadData(void* destination, std::size_t len, read_error& e)
{
    if(len >= buffer.size())
    {
        e.set_error(load_result::END_OF_FILE);
    }

    assert(fpos <= fsize); // This should never EVER happen. Terminate if it does.

    std::size_t nbytes = std::min(len, fsize - fpos);
    if(nbytes != len)
    {
        e.set_error(load_result::PARTIAL_READ);
    }

    std::memcpy(destination, buffer.data() + fpos, nbytes);

    fpos += nbytes; // Increment the position we are at in this file

    return nbytes;
}
