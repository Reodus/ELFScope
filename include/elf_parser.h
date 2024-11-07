#ifndef ELF_PARSER_H
#define ELF_PARSER_H

#include <elf.h>

#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "eheader_parser.h"
#include "pheader_parser.h"

enum elf_structs
{
    EHEADER,
    PHEADER
};

class ELFParser
{
  public:
    ELFParser(std::string &file_path);
    bool IsValid();

    std::unique_ptr<ELFHeader> elf_header;
    std::vector<std::unique_ptr<ProgramHeader>> program_headers;

  private:
    std::ifstream fd;
    bool valid;
    bool is_32bit;
    void IsFileValid();
};

#endif