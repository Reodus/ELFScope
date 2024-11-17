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

            fd.seekg(ph_offset + (i + 1) * (is_32bit ? sizeof(Elf32_Phdr)
                                                     : sizeof(Elf64_Phdr)),
                     std::ios::beg);
        }

        uint64_t sh_offset = elf_header->GetSectionHeaderOffset();
        uint16_t sh_entry_count = elf_header->GetSectionHeaderEntryCount();

        fd.seekg(sh_offset, std::ios::beg);

        for (uint16_t i = 0; i < sh_entry_count; ++i) {
            section_headers.push_back(
                std::make_unique<SectionHeader>(fd, is_32bit));

            fd.seekg(sh_offset + (i + 1) * (is_32bit ? sizeof(Elf32_Shdr)
                                                     : sizeof(Elf64_Shdr)),
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

std::string ELFParser::GetStringFromStringTable(uint32_t index) {
    if (!IsValid()) {
        throw std::runtime_error("Invalid ELF file.");
    }

    // Get the section header string table index from the ELF header
    uint16_t shstrndx = elf_header->GetSectionHeaderStringIndex();

    if (shstrndx >= section_headers.size()) {
        throw std::runtime_error("Invalid section header string table index.");
    }

    // Locate the string table section
    const auto &string_table_section = section_headers[shstrndx];
    if (string_table_section->GetType() != SHT_STRTAB) {
        throw std::runtime_error("Section header string table not found.");
    }

    // Read the string table content
    uint64_t offset = string_table_section->GetOffset();
    uint64_t size = string_table_section->GetSize();

    fd.seekg(offset, std::ios::beg);
    std::vector<char> string_table(size);
    fd.read(string_table.data(), size);

    if (index >= size) {
        throw std::out_of_range("Index out of range in string table.");
    }

    // Return the string at the specified index
    return std::string(&string_table[index]);
}
