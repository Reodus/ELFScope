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
#include "sheader_parser.h"

enum elf_structs { EHEADER, PHEADER, SHEADER };

class ELFParser {
public:
    ELFParser(std::string &file_path);
    bool IsValid();

    std::unique_ptr<ELFHeader> elf_header;
    std::vector<std::unique_ptr<ProgramHeader>> program_headers;
    std::vector<std::unique_ptr<SectionHeader>> section_headers;

    std::string GetStringFromStringTable(uint32_t index);

private:
    std::ifstream fd;
    bool valid;
    bool is_32bit;
    void IsFileValid();
};

#endif