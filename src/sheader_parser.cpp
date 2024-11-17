#include "sheader_parser.h"

void SectionHeader::ReadSectionHeader() {
    if (is_32bit) {
        fd->read(reinterpret_cast<char *>(&shdr32), sizeof(Elf32_Phdr));
        if (!fd) {
            throw std::runtime_error("Failed to read 32-bit section header");
        }
    } else {
        fd->read(reinterpret_cast<char *>(&shdr64), sizeof(Elf64_Phdr));
        if (!fd) {
            throw std::runtime_error("Failed to read 64-bit section header");
        }
    }
}

uint64_t SectionHeader::GetName() {
    return is_32bit ? shdr32.sh_name : shdr64.sh_name;
}

uint32_t SectionHeader::GetType() {
    return is_32bit ? shdr32.sh_type : shdr64.sh_type;
}

uint32_t SectionHeader::GetFlags() {
    return is_32bit ? shdr32.sh_flags : shdr64.sh_flags;
}

uint64_t SectionHeader::GetOffset() {
    return is_32bit ? shdr32.sh_offset : shdr64.sh_offset;
}

uint64_t SectionHeader::GetVirtualAddress() {
    return is_32bit ? shdr32.sh_addr : shdr64.sh_addr;
}

uint64_t SectionHeader::GetSize() {
    return is_32bit ? shdr32.sh_size : shdr64.sh_size;
}

uint64_t SectionHeader::GetLink() {
    return is_32bit ? shdr32.sh_link : shdr64.sh_link;
}

uint64_t SectionHeader::GetInfo() {
    return is_32bit ? shdr32.sh_info : shdr64.sh_info;
}

uint64_t SectionHeader::GetAddrAlign() {
    return is_32bit ? shdr32.sh_addralign : shdr64.sh_addralign;
}

uint64_t SectionHeader::GetEntSize() {
    return is_32bit ? shdr32.sh_entsize : shdr64.sh_entsize;
}

std::string SectionHeader::PrintSectionType(uint32_t type) {
    switch (type) {
        case 0x0:
            return "SHT_NULL";  // Section header table entry unused
        case 0x1:
            return "SHT_PROGBITS";  // Program-defined information
        case 0x2:
            return "SHT_SYMTAB";  // Symbol table
        case 0x3:
            return "SHT_STRTAB";  // String table
        case 0x4:
            return "SHT_RELA";  // Relocation entries with addends
        case 0x5:
            return "SHT_HASH";  // Symbol hash table
        case 0x6:
            return "SHT_DYNAMIC";  // Dynamic linking information
        case 0x7:
            return "SHT_NOTE";  // Notes
        case 0x8:
            return "SHT_NOBITS";  // Program space with no file data
        case 0x9:
            return "SHT_REL";  // Relocation entries, no addends
        case 0xA:
            return "SHT_SHLIB";  // Reserved
        case 0xB:
            return "SHT_DYNSYM";  // Dynamic linker symbol table
        case 0xE:
            return "SHT_INIT_ARRAY";  // Array of constructors
        case 0xF:
            return "SHT_FINI_ARRAY";  // Array of destructors
        case 0x10:
            return "SHT_PREINIT_ARRAY";  // Array of pre-constructors
        case 0x11:
            return "SHT_GROUP";  // Section group
        case 0x12:
            return "SHT_SYMTAB_SHNDX";  // Extended section indices
        case 0x13:
            return "SHT_NUM";  // Number of defined types
        default:
            if (type >= 0x60000000 && type <= 0x6FFFFFFF) {
                return "SHT_LOOS";  // Operating system-specific semantics
            }
            if (type >= 0x70000000 && type <= 0x7FFFFFFF) {
                return "SHT_LOPROC";  // Processor-specific semantics
            }
            return "Unknown";  // Unrecognized section type
    }
}