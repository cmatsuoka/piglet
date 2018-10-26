#ifndef SRC_FILE_IO_H_
#define SRC_FILE_IO_H_

#include <string>
#include <fstream>


class File {
    std::string path;
    std::fstream file;
 public:
    explicit File(std::string);
    File(std::string, std::ios_base::openmode);
    uint32_t read32b();
    File& read(char *, int);
    File& read_line(std::string&);
    File& seekg(off_t);
    bool eof();
    static bool exists(std::string const&);
    static bool is_file(std::string const&, bool = false);
    static bool is_directory(std::string const&, bool = false);
    static char separator();
};


class InputFile : public File {
 public:
    explicit InputFile(std::string);
};

#endif  // SRC_FILE_IO_H_
