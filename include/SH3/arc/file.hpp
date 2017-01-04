/** @file
 *  Functions to extract data from arc files.
 *
 *  @copyright 2016  Palm Studios
 *
 *  @date 1-1-2017
 *
 *  @author Alexander Hirsch
 */
#ifndef SH3_ARC_FILE_HPP_INCLUDED
#define SH3_ARC_FILE_HPP_INCLUDED

#include <zlib.h>

#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>

struct sh3_arc_file final
{
private:
    struct gz_file_closer final
    {
    public:
        void operator()(gzFile file) const { gzclose(file); }
    };

public:
    enum class read_result
    {
        SUCCESS,
        END_OF_FILE,
        PARTIAL_READ,
        GZ_ERROR,
    };

    class read_error final
    {
    public:
        operator bool() const { return result != read_result::SUCCESS; }
        read_result get_read_result() const { return result; }

        void set_error(sh3_arc_file::read_result result, gzFile file);

        std::string message() const;

    private:
        read_result result = read_result::SUCCESS;
        int zlib_err = Z_OK;
        int os_err = 0;
    };

    explicit sh3_arc_file(const std::string& path);

    bool is_open() const {return static_cast<bool>(gzHandle);}

    std::size_t ReadData(void* destination, std::size_t len, read_error& e);

    template<typename T, typename = std::enable_if<std::is_trivially_copyable<T>::value>>
    size_t ReadObject(T& destination, std::size_t len, read_error& e)
    {
        return ReadData(&destination, len, e);
    }

    template<typename T, typename = std::enable_if<std::is_trivially_copyable<T>::value>>
    size_t ReadObject(T& destination, read_error& e)
    {
        return ReadObject(destination, sizeof(destination), e);
    }

    std::size_t ReadString(std::string& destination, std::size_t len, read_error& e);

private:
    std::unique_ptr<gzFile_s, gz_file_closer> gzHandle;
};

#endif // SH3_ARC_FILE_HPP_INCLUDED
