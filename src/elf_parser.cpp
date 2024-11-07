#include "elf_parser.h"

ELFParser::ELFParser(std::string &file_path) : valid(false) {
    try {
        fd.open(file_path, std::ios::binary);
        if (!fd) {
            throw std::runtime_error("Failed to open file: " + file_path);
        }

        IsFileValid();

        // Initialize elf_header with the ELFHeader object
        elf_header = std::make_unique<ELFHeader>(fd, is_32bit);

        // Load program headers dynamically
        uint64_t ph_offset = elf_header->GetProgramHeaderOffset();
        uint16_t ph_entry_count = elf_header->GetProgramHeaderEntryCount();

        fd.seekg(ph_offset, std::ios::beg);

        for (uint16_t i = 0; i < ph_entry_count; ++i) {
            program_headers.push_back(
                std::make_unique<ProgramHeader>(fd, is_32bit));

            // Move to the next program header entry (seek forward by entry
            // size)
            fd.seekg(ph_offset + (i + 1) * (is_32bit ? sizeof(Elf32_Phdr)
                                                     : sizeof(Elf64_Phdr)),
                     std::ios::beg);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error parsing ELF file: " << e.what() << std::endl;
    }
}

void ELFParser::IsFileValid() {
    char elf_class_value[1];
    char magic_value[4];

    fd.seekg(0);

    fd.read(magic_value, 4);
    if (magic_value[EI_MAG0] != ELFMAG0 || magic_value[EI_MAG1] != ELFMAG1 ||
        magic_value[EI_MAG2] != ELFMAG2 || magic_value[EI_MAG3] != ELFMAG3) {
        valid = false;
        return;
    }

    valid = true;
}

bool ELFParser::IsValid() { return valid; }