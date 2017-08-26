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
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <zlib.h>

#include "SH3/arc/subarc.hpp"
#include "SH3/error.hpp"
#include "SH3/system/log.hpp"

using namespace sh3::arc;

namespace {

    /** @defgroup arc-headers arc header types
     *  @{
     */

    /** Info about a subarc */
    struct subarc_header final
    {
        uint16_t type;     /**< This is 2 (for an arc subarc header) */
        uint16_t hsize;    /**< Size of this header in bytes */
        uint32_t numFiles; /**< Number of files in this subarc */
    };

    /** Info about a file in a subarc */
    struct subarc_file_info final
    {
        uint16_t type;        /**< This is 3 (for a file entry) */
        uint16_t fileSize;    /**< Size of this file entry(in bytes) */
        uint16_t arcIndex;    /**< Index of this file (in the .arc it is located in) */
        uint16_t subarcIndex; /**< Index of the current subarc we are in. */
    };
    static_assert(std::numeric_limits<decltype(std::declval<subarc_file_info>().arcIndex)>::min() >= std::numeric_limits<subarc::index_t>::min()
               && std::numeric_limits<decltype(std::declval<subarc_file_info>().arcIndex)>::max() <= std::numeric_limits<subarc::index_t>::max(),
                  "index_t must be able to represent arcIndex");

    /** Actual file entry */
    struct subarc_file_entry final
    {
        subarc_file_info header; /**< File info header */
        std::string      fname;  /**< File path and name (plus ext and @c NUL terminator) */
    };

    /** @} */

    /**
     *  A struct to read data from the @c arc.arc.
     */
    struct mft_reader final
    {
    private:
        /**
         *  zlib closer functor.
         */
        struct gz_file_closer final
        {
        public:
            void operator()(gzFile file) const { gzclose(file); }
        };

        /** @defgroup arc-headers arc header types
         *  @{
         */

        /** Type check header */
        struct header
        {
            uint32_t magic;     /**< Magic for @c arc.arc. This is ALWAYS @ref magic. */
            uint8_t unused[12]; /**< 12 filler bytes. */
        };

        /** Info about the MFT */
        struct data
        {
            uint16_t type;         /**< This is @c 1 (for @c arc.arc info). */
            uint16_t header_size;  /**< Size of this header, should be @c sizeof(Info). */
            uint32_t subarcCount; /**< How many subarcs there are in the mft. (i.e how many @c .arc files in @c /data/). */
            uint32_t fileCount;    /**< Number of files in this subarc. */
        };

        /** @} */

    public:
        /**
         *  An enum representing possible results from trying to read data.
         */
        enum class read_result
        {
            SUCCESS,      ///< Success.
            END_OF_FILE,  ///< Already reached the end of the file.
            PARTIAL_READ, ///< End of file reached before completing the read.
            GZ_ERROR,     ///< An error happened while decompressing.
        };

        /**
         *  The @ref error class for @ref read_result.
         */
        class read_error final : public error<read_result>
        {
        public:
            /**
             *  Set the wrapped @ref read_result while reading a @c gzFile.
             *  
             *  @param result The result to assign.
             *  @param file   The file that was read.
             *  
             *  If @p result is @ref read_result::GZ_ERROR then the <tt>file</tt>'s zlib error code is extracted.
             *  Otherwise, @p file may also be set to @c nullptr.
             */
            void set_error(read_result result, gzFile file);

            /**
             *  Stringify the @ref read_error.
             *  
             *  @returns The error as a @c std::string.
             */
            std::string message() const;

        private:
            int zlib_err = Z_OK;
            int os_err = 0;
        };

        /**
         *  Open the @c arc.arc.
         */
        mft_reader();

        /**
         *  Check whether the file was successfully opened.
         *  
         *  @returns @c true if the file was successfully opened, @c false otherwise.
         */
        bool IsOpen() const {return static_cast<bool>(gzHandle);}

        /**
         *  Read a @ref sh3::arc::subarc.
         */
        //TODO: struct subarc_read_error
        subarc ReadNextSubarc();

        std::size_t GetSubarcCount() const { return data.subarcCount; }

    private:
        /**
         *  Read binary data from an arc file to a buffer.
         *  
         *  @param[out] destination The destination buffer.
         *  @param      len         The number of bytes to read into @p destination.
         *  @param[out] e           The @ref read_error, which is set if any error occurs.
         *  
         *  @returns The number of bytes read.
         *  
         *  @see @ref ReadObject
         *  @see @ref ReadString
         */
        std::size_t ReadData(void* destination, std::size_t len, read_error& e);

        /**
         *  Read binary data from an arc file to an object.
         *  
         *  @tparam T The type of the object to read into.
         *  
         *  @param[out] destination The destination object.
         *  @param      len         The number of bytes to read into @p destination.
         *  @param[out] e           The @ref read_error, which is set if any error occurs.
         *  
         *  @returns The number of bytes read.
         *  
         *  The @p destination must be a trivially copyable type so that it can be serialized directly from binary data.
         *  
         *  @see @ref ReadObject(T& destination, read_error& e)
         *  @see @ref ReadData
         *  @see @ref ReadString
         */
        template<typename T, typename = std::enable_if<std::is_trivially_copyable<T>::value>>
        size_t ReadObject(T& destination, std::size_t len, read_error& e)
        {
            return ReadData(&destination, len, e);
        }

        /**
         *  Read binary data from an arc file to an object.
         *  
         *  @tparam T The type of the object to read into.
         *  
         *  @param[out] destination The destination object.
         *  @param[out] e           The @ref read_error, which is set if any error occurs.
         *  
         *  @returns The number of bytes read.
         *  
         *  The @p destination must be a trivially copyable type so that it can be serialized directly from binary data.
         *  
         *  @see @ref ReadObject(T& destination, std::size_t len, read_error& e)
         *  @see @ref ReadData
         *  @see @ref ReadString
         */
        template<typename T, typename = std::enable_if<std::is_trivially_copyable<T>::value>>
        size_t ReadObject(T& destination, read_error& e)
        {
            return ReadObject(destination, sizeof(destination), e);
        }

        /**
         *  Read a string from an arc file.
         *  
         *  @param[out] destination The string to read into.
         *  @param      len         The number of characters to read.
         *  @param[out] e           The @ref read_error, which is set if any error occurs.
         *  
         *  @returns The number of bytes read.
         *  
         *  @see @ref ReadObject
         *  @see @ref ReadString
         */
        std::size_t ReadString(std::string& destination, std::size_t len, read_error& e);

        std::unique_ptr<gzFile_s, gz_file_closer> gzHandle;
        header header;
        data data;
    };

    void mft_reader::read_error::set_error(read_result res, gzFile file)
    {
        result = res;
        zlib_err = Z_OK;
        os_err = 0;
        if(result == read_result::GZ_ERROR)
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

    std::string mft_reader::read_error::message() const
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

    mft_reader::mft_reader()
        :gzHandle(gzopen(mftPath, "rb"))
    {
        if(!IsOpen())
        {
            die("E00001: mft_reader::mft_reader( ): Unable to find /data/arc.arc!");
        }


        // Read and check the header
        read_error readError;
        ReadObject(header, readError);
        if(readError)
        {
            die("E00002: mft_reader::mft_reader( ): Error reading arc.arc header: %s! Was the handle opened correctly?!", readError.message().c_str());
        }

        static constexpr decltype(header.magic) magic = 0x20030417; /**< @c arc.arc file magic */
        if(header.magic != magic)
        {
            die("E00003: mft_reader::mft_reader( ): arc.arc, Invalid File Marker!!!");
        }

        ReadObject(data, readError);
        if(readError)
        {
            die("E00004: mft_reader::mft_reader( ): Invalid read of arc.arc information: %s!", readError.message().c_str());
        }
    }

    std::size_t mft_reader::ReadData(void* destination, std::size_t len, read_error& e)
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

    std::size_t mft_reader::ReadString(std::string& destination, std::size_t len, read_error& e)
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

    subarc mft_reader::ReadNextSubarc()
    {
        assert(IsOpen());

        read_error readError;

        subarc_header header;
        ReadObject(header, readError);
        if(readError)
        {
            die("E00006: mft_reader::ReadNextSubarc( ): Invalid read of arc.arc subarc: %s!", readError.message().c_str());
        }

        std::string subarcName;
        ReadString(subarcName, header.hsize - sizeof(header), readError);
        if(subarcName.back() != '\0')
        {
            die("E00007: mft_reader::ReadNextSubarc( ): Garbage read when reading subarc name (NUL terminator missing): %s!", subarcName.c_str());
        }
        // remove trailing NUL
        // Some filenames seem to have multiple NULs.
        subarcName.resize(subarcName.find_last_not_of('\0') + 1);
        subarcName.shrink_to_fit();

        // We have now loaded information about the subarc, so we can start
        // reading in all the files located in it (not in full, obviously...)
        subarc::files_map fileList;
        for(std::size_t i = 0; i < header.numFiles; ++i)
        {
            subarc_file_entry file;
            ReadObject(file.header, readError);

            ReadString(file.fname, file.header.fileSize - sizeof(file.header), readError);
            if(file.fname.back() != '\0')
            {
                die("E00008: mft_reader::ReadNextSubarc( ): Garbage read when reading file name (NUL terminator missing): %s!", file.fname.c_str());
            }
            // remove trailing NUL
            // Some filenames seem to have multiple NULs.
            while(file.fname.back() == '\0')
            {
                file.fname.pop_back();
            }
            file.fname.shrink_to_fit();
            //Log(LogLevel::INFO, "Read file: %s", file.fname.c_str());

            fileList[file.fname] = file.header.arcIndex; // Map the file name to its subarc index
            //Log(LogLevel::INFO, "Added file to file list!");
        }

        return subarc(std::move(subarcName), std::move(fileList));
    }
}

mft::mft()
{
    mft_reader reader;

    // Load each sub-arc
    std::size_t numSubarcs = reader.GetSubarcCount();
    subarcs.reserve(numSubarcs);

    for(std::size_t i = 0; i < numSubarcs; ++i)
    {
        subarcs.emplace_back(reader.ReadNextSubarc());
    }
}

int mft::LoadFile(const std::string& filename, std::vector<std::uint8_t>& buffer, std::vector<std::uint8_t>::iterator& start)
{
    for(subarc& candidate : subarcs)
    {
        int result = candidate.LoadFile(filename, buffer, start);
        if(result != arcFileNotFound)
        {
            return result;
        }
    }

    return arcFileNotFound;
}
