#ifndef E_HEADER_PAESER_H
#define E_HEADER_PAESER_H

#include <elf.h>
#include <stdint.h>

#include <fstream>
#include <vector>

class ELFHeader
{
  public:
    // Raw value accessors
    ELFHeader(std::ifstream &fd, bool is_32bit)
    {
        this->fd = &fd;
        this->is_32bit = is_32bit;
        ReadHeader();
    }

    std::vector<unsigned char> GetIdent();
    uint16_t GetType();
    uint16_t GetMachine();
    uint32_t GetVersion();
    uint64_t GetEntryPoint();
    uint64_t GetProgramHeaderOffset();
    uint64_t GetSectionHeaderOffset();
    uint32_t GetFlags();
    uint16_t GetHeaderSize();
    uint16_t GetProgramHeaderEntrySize();
    uint16_t GetProgramHeaderEntryCount();
    uint16_t GetSectionHeaderEntrySize();
    uint16_t GetSectionHeaderEntryCount();
    uint16_t GetSectionHeaderStringIndex();

    // Wrapper functions for human-readable fields
    std::string GetClass();
    std::string GetDataEncoding();
    std::string GetOSABI();
    uint8_t GetABIVersion();
    std::string GetVersionString();
    std::string PrintELFType(uint16_t e_type);
    std::string PrintMachine(uint16_t e_machine);

    bool is_32bit;

  private:
    std::vector<unsigned char> ident;
    std::ifstream *fd;
    union
    {
        Elf32_Ehdr ehdr32;
        Elf64_Ehdr ehdr64;
    };

    void ReadHeader();
};

#endif