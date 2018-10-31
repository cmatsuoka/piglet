#include "file_io.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <fstream>


namespace {

    std::string error()
    {
        char *s = strerror(errno);
        return std::string(s);
    }


}  // namespace


File::File(std::string path, std::ios_base::openmode mode) :
    file(path, mode),
    path(path)
{
    if (file.fail()) {
        throw std::runtime_error(path + ": " + error());
    }
}

File::File(std::string path) : File(path, std::fstream::in | std::fstream::out)
{
}

bool File::eof()
{
    return file.eof();
}

File& File::read_line(std::wstring &s)
{
    s.clear();
    std::getline(file, s);
    return *this;
}

/**
 * Set the position of the next byte to read.
 * @param pos The offset to set
 * @return This file object
 */
File& File::seekg(off_t pos)
{
    file.seekg(pos);
    return *this;
}

/**
 * Check whether this file exists.
 * @param name Full path name
 * @return True if file exists
 */
bool File::exists(std::string const& name)
{
    struct stat st;
    return stat(name.c_str(), &st) == 0;
}

/**
 * Check whether this name refers to a regular file.
 * @param name Full path name
 * @param except Throw an exception if file doesn't exist
 * @return True if is a regular file
 */
bool File::is_file(std::string const& name, bool except)
{
    struct stat st;
    if (stat(name.c_str(), &st) < 0) {
        if (except) {
            throw std::runtime_error(name + ": " + error());
        } else {
            return false;
        }
    }
    return (st.st_mode & S_IFMT) == S_IFREG;
}

/**
 * Check whether this name refers to a directory.
 * @param name Full path name
 * @param except Throw an exception if file doesn't exist
 * @return True if is a directory
 */
bool File::is_directory(std::string const& name, bool except)
{
    struct stat st;
    if (stat(name.c_str(), &st) < 0) {
        if (except) {
            throw std::runtime_error(name + ": " + error());
        } else {
            return false;
        }
    }
    return (st.st_mode & S_IFMT) == S_IFDIR;
}

char File::separator()
{
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}


// InputFile

InputFile::InputFile(std::string path) : File(path, std::fstream::in)
{
}


