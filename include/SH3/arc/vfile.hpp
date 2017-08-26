/** @file
 *
 *  Class and data structures describing a Virtual File inside a SILENT HILL 3 arc section.
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 5-1-2017
 *
 *  @author Jesse Buhagiar
 */
#ifndef VFILE_HPP_INCLUDED
#define VFILE_HPP_INCLUDED

#include "SH3/error.hpp"
#include <ios>
#include <vector>

namespace sh3 { namespace arc {
    struct mft;
} }

/**
 *  Opens a handle to a Virtual File contained within arc.arc
 *
 *  After the handle to the file is open, it is possible to seek and rewind
 *  to a specific part of the file using @ref Seek().
 *
 *  Each read sets the @ref read_error indicating whether a partial read was performed,
 *  or that the end of file was encountered (meaning that @ref fpos `+ len` was equal to @ref fsize.
 *
 */
struct sh3_arc_vfile final
{
public:
    /**
     *  Failure codes for @ref ReadData.
     */
    enum class load_result
    {
        SUCCESS,
        END_OF_FILE,
        PARTIAL_READ,
    };

    struct read_error final : public error<load_result>
    {
    public:
        std::string message() const;
    };


    sh3_arc_vfile(sh3::arc::mft& mft, const std::string& filename): fpos(0), fname(filename)
    {Open(mft, filename);}

    /**
     *  Read @c len bytes of data into a destination buffer.
     *
     *  @param destination      Pointer to write read values into.
     *  @param len              Number of bytes to read from the file.
     *  @param e                @ref read_error from this operation.
     *
     *  @returns Number of bytes read (which should be len)
     */
    std::size_t ReadData(void* destination, std::size_t len, read_error& e);

    /**
     *  Rewind this file to the beginning (set fpos to 0).
     */
    void Rewind(){fpos = 0;}

    /**
     *  Seek to a certain position in the file.
     *
     *  @param pos      Position to seek to.
     *  @param origin   The origin we want to seek from.
     */
     void Seek(long pos, std::ios_base::seekdir origin);

     /**
      *  Dump the contents of the vector @ref buffer to a file on disk.
      */
      void Dump2Disk();

     /**
      * Get the size of this file (in bytes)
      */
      size_t GetFilesize() const {return fsize;}


private:
    std::size_t     fpos;           /**< Current file position */
    std::size_t     fsize;          /**< Size of this file inside the arc section in bytes */
    std::string     fname;          /**< The name of this file (taken from arc.arc) */
    bool            open = false;   /**< Is this file handle currently open? */

    std::vector<std::uint8_t> buffer; /**< Buffer that @ref ReadData() reads from */

    /**
     *  Open a handle to a virtual file.
     *
     *  @param mft      The @ref sh3::arc::mft Master File Table, arc.arc.
     *  @param filename The name of the file we want to open.
     *
     *  @note If the file is already open, this function returns false.
     *
     *  @returns @c true if the file was found, @c false if not.
     */
    bool Open(sh3::arc::mft& mft, const std::string& filename);
};

#endif // VFILE_HPP_INCLUDED
