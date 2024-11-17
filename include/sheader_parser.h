#include <elf.h>

#include <fstream>

class SectionHeader {
public:
    SectionHeader(std::ifstream &fd, bool is_32bit)
        : is_32bit(is_32bit), fd(&fd) {
        ReadSectionHeader();
    }

    uint64_t GetName();
    uint32_t GetType();
    uint32_t GetFlags();
    uint64_t GetOffset();
    uint64_t GetVirtualAddress();
    uint64_t GetSize();
    uint64_t GetLink();
    uint64_t GetInfo();
    uint64_t GetAddrAlign();
    uint64_t GetEntSize();

    std::string GetSectionName();

    // Human-readable description of section type
    std::string PrintSectionType(uint32_t type);

private:
    bool is_32bit;
    std::ifstream *fd;

    union {
        Elf32_Shdr shdr32;
        Elf64_Shdr shdr64;
    };

    void ReadSectionHeader();
};
