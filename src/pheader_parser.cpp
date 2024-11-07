#include "pheader_parser.h"

uint32_t ProgramHeader::GetType() {
    return is_32bit ? phdr32.p_type : phdr64.p_type;
}

uint32_t ProgramHeader::GetFlags() {
    return is_32bit ? phdr32.p_flags : phdr64.p_flags;
}

uint64_t ProgramHeader::GetOffset() {
    return is_32bit ? phdr32.p_offset : phdr64.p_offset;
}

uint64_t ProgramHeader::GetVirtualAddress() {
    return is_32bit ? phdr32.p_vaddr : phdr64.p_vaddr;
}

uint64_t ProgramHeader::GetPhysicalAddress() {
    return is_32bit ? phdr32.p_paddr : phdr64.p_paddr;
}

uint64_t ProgramHeader::GetFileSize() {
    return is_32bit ? phdr32.p_filesz : phdr64.p_filesz;
}

uint64_t ProgramHeader::GetMemorySize() {
    return is_32bit ? phdr32.p_memsz : phdr64.p_memsz;
}

uint64_t ProgramHeader::GetAlignment() {
    return is_32bit ? phdr32.p_align : phdr64.p_align;
}

std::string ProgramHeader::PrintSegmentType(uint32_t type) {
    switch (type) {
        case PT_NULL:
            return "NULL";
        case PT_LOAD:
            return "LOAD";
        case PT_DYNAMIC:
            return "DYNAMIC";
        case PT_INTERP:
            return "INTERP";
        case PT_NOTE:
            return "NOTE";
        case PT_SHLIB:
            return "SHLIB";
        case PT_PHDR:
            return "PHDR";
        case PT_TLS:
            return "TLS";
        case PT_GNU_EH_FRAME:
            return "GNU_EH_FRAME";
        case PT_GNU_STACK:
            return "GNU_STACK";
        case PT_GNU_RELRO:
            return "GNU_RELRO";
        case PT_GNU_PROPERTY:
            return "GNU_PROPERTY";
        case PT_SUNWBSS:
            return "SUNWBSS";
        case PT_SUNWSTACK:
            return "SUNWSTACK";

        // OS-specific range
        case PT_LOOS:
        case PT_HIOS:
            return "OS_SPECIFIC";

        // Processor-specific range
        case PT_LOPROC:
        case PT_HIPROC:
            return "PROCESSOR_SPECIFIC";

        default:
            return "UNKNOWN";
    }
}

void ProgramHeader::ReadProgramHeader() {
    if (is_32bit) {
        fd->read(reinterpret_cast<char *>(&phdr32), sizeof(Elf32_Phdr));
        if (!fd) {
            throw std::runtime_error("Failed to read 32-bit program header");
        }
    } else {
        fd->read(reinterpret_cast<char *>(&phdr64), sizeof(Elf64_Phdr));
        if (!fd) {
            throw std::runtime_error("Failed to read 64-bit program header");
        }
    }
}