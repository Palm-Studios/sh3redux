/** @file
 *  Functions to extract data from @c arc.arc.
 *  
 *  @copyright 2016-2017  Palm Studios
 */
#include "SH3/arc/mft.hpp"

#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstring>
#include <iterator>
#include <limits>
#include <string>
#include <vector>

#include "SH3/arc/subarc.hpp"
#include "SH3/arc/types.hpp"
#include "SH3/system/log.hpp"

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

static constexpr const char* mftPath = "data/arc.arc";

mft::mft()
    :gzHandle(gzopen(mftPath, "rb"))
{
    if(!IsOpen())
    {
        die("E00001: mft::mft( ): Unable to find /data/arc.arc!");
    }


    // Read and check the header
    read_error readError;
    ReadObject(header, readError);
    if(readError)
    {
        die("E00002: mft::mft( ): Error reading arc.arc header: %s! Was the handle opened correctly?!", readError.message().c_str());
    }

    static constexpr decltype(header.magic) magic = 0x20030417; /**< @c arc.arc file magic */
    if(header.magic != magic)
    {
        die("E00003: mft::mft( ): arc.arc, Invalid File Marker!!!");
    }

    ReadObject(data, readError);
    if(readError)
    {
        die("E00004: mft::mft( ): Invalid read of arc.arc information: %s!", readError.message().c_str());
    }
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
    assert(len <= std::numeric_limits<int>::max()); // overflow check

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

void mft::ReadNextSection(sh3::arc::subarc& section)
{
    assert(IsOpen());

    read_error readError;

    sh3_arc_section_header_t header;
    ReadObject(header, readError);
    if(readError)
    {
        die("E00006: mft::ReadNextSection( ): Invalid read of arc.arc section: %s!", readError.message().c_str());
    }

    ReadString(section.name, header.hsize - sizeof(header), readError);
    if(section.name.back() != '\0')
    {
        die("E00007: mft::ReadNextSection( ): Garbage read when reading section name (NUL terminator missing): %s!", section.name.c_str());
    }
    // remove trailing NUL
    // Some filenames seem to have multiple NULs.
    section.name.resize(section.name.find_last_not_of('\0') + 1);
    section.name.shrink_to_fit();

    // We have now loaded information about the section, so we can start
    // reading in all the files located in it (not in full, obviously...)
    for(std::size_t i = 0; i < header.numFiles; ++i)
    {
        sh3_arc_file_entry_t file;
        ReadObject(file.header, readError);

        ReadString(file.fname, file.header.fileSize - sizeof(file.header), readError);
        if(file.fname.back() != '\0')
        {
            die("E00008: mft::ReadNextSection( ): Garbage read when reading file name (NUL terminator missing): %s!", file.fname.c_str());
        }
        // remove trailing NUL
        // Some filenames seem to have multiple NULs.
        while(file.fname.back() == '\0')
        {
            file.fname.pop_back();
        }
        file.fname.shrink_to_fit();
        //Log(LogLevel::INFO, "Read file: %s", file->fname.c_str());

        section.files[file.fname] = file.header.arcIndex; // Map the file name to its subarc index
        //Log(LogLevel::INFO, "Added file to file list!");
    }
}
