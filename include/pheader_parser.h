#ifndef P_HEADER_PAESER_H
#define P_HEADER_PAESER_H

#include <elf.h>

#include <fstream>

class ProgramHeader
{
  public:
    ProgramHeader(std::ifstream &fd, bool is_32bit)
        : is_32bit(is_32bit), fd(&fd)
    {
        ReadProgramHeader();
    }

    uint32_t GetType();
    uint32_t GetFlags();
    uint64_t GetOffset();
    uint64_t GetVirtualAddress();
    uint64_t GetPhysicalAddress();
    uint64_t GetFileSize();
    uint64_t GetMemorySize();
    uint64_t GetAlignment();

    // Human-readable description of segment type
    std::string PrintSegmentType(uint32_t type);

  private:
    bool is_32bit;
    std::ifstream *fd;

    union
    {
        Elf32_Phdr phdr32;
        Elf64_Phdr phdr64;
    };

    void ReadProgramHeader();
};

#endif