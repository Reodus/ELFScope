#include "eheader_parser.h"

// Retrieves the ELF identification array (e_ident)
std::vector<unsigned char> ELFHeader::GetIdent()
{
    std::vector<unsigned char> ident(EI_NIDENT);
    fd->seekg(0, std::ios::beg);
    fd->read(reinterpret_cast<char *>(ident.data()), EI_NIDENT);
    return ident;
}

uint16_t ELFHeader::GetType()
{
    return is_32bit ? ehdr32.e_type : ehdr64.e_type;
}

uint16_t ELFHeader::GetMachine()
{
    return is_32bit ? ehdr32.e_machine : ehdr64.e_machine;
}

uint32_t ELFHeader::GetVersion()
{
    return is_32bit ? ehdr32.e_version : ehdr64.e_version;
}

uint64_t ELFHeader::GetEntryPoint()
{
    return is_32bit ? static_cast<uint64_t>(ehdr32.e_entry) : ehdr64.e_entry;
}

uint64_t ELFHeader::GetProgramHeaderOffset()
{
    return is_32bit ? static_cast<uint64_t>(ehdr32.e_phoff) : ehdr64.e_phoff;
}

uint64_t ELFHeader::GetSectionHeaderOffset()
{
    return is_32bit ? static_cast<uint64_t>(ehdr32.e_shoff) : ehdr64.e_shoff;
}

uint32_t ELFHeader::GetFlags()
{
    return is_32bit ? ehdr32.e_flags : ehdr64.e_flags;
}

uint16_t ELFHeader::GetHeaderSize()
{
    return is_32bit ? ehdr32.e_ehsize : ehdr64.e_ehsize;
}

uint16_t ELFHeader::GetProgramHeaderEntrySize()
{
    return is_32bit ? ehdr32.e_phentsize : ehdr64.e_phentsize;
}

uint16_t ELFHeader::GetProgramHeaderEntryCount()
{
    return is_32bit ? ehdr32.e_phnum : ehdr64.e_phnum;
}

uint16_t ELFHeader::GetSectionHeaderEntrySize()
{
    return is_32bit ? ehdr32.e_shentsize : ehdr64.e_shentsize;
}

uint16_t ELFHeader::GetSectionHeaderEntryCount()
{
    return is_32bit ? ehdr32.e_shnum : ehdr64.e_shnum;
}

uint16_t ELFHeader::GetSectionHeaderStringIndex()
{
    return is_32bit ? ehdr32.e_shstrndx : ehdr64.e_shstrndx;
}

std::string ELFHeader::GetClass() { return is_32bit ? "ELF32" : "ELF64"; }

std::string ELFHeader::GetDataEncoding()
{
    uint8_t data_encoding =
        is_32bit ? ehdr32.e_ident[EI_DATA] : ehdr64.e_ident[EI_DATA];
    switch (data_encoding)
    {
    case ELFDATA2LSB:
        return "Little Endian";
    case ELFDATA2MSB:
        return "Big Endian";
    default:
        return "Invalid";
    }
}

std::string ELFHeader::GetOSABI()
{
    uint8_t os_abi =
        is_32bit ? ehdr32.e_ident[EI_OSABI] : ehdr64.e_ident[EI_OSABI];
    switch (os_abi)
    {
    case ELFOSABI_SYSV:
        return "UNIX - System V";
    case ELFOSABI_HPUX:
        return "HP-UX";
    case ELFOSABI_NETBSD:
        return "NetBSD";
    case ELFOSABI_LINUX:
        return "Linux";
    case ELFOSABI_SOLARIS:
        return "Solaris";
    case ELFOSABI_IRIX:
        return "IRIX";
    case ELFOSABI_FREEBSD:
        return "FreeBSD";
    case ELFOSABI_TRU64:
        return "TRU64 UNIX";
    case ELFOSABI_ARM:
        return "ARM architecture";
    case ELFOSABI_STANDALONE:
        return "Standalone (embedded)";
    default:
        return "Unknown";
    }
}

std::string ELFHeader::PrintMachine(uint16_t e_machine)
{
    switch (e_machine)
    {
    case EM_386:
        return "Intel 80386";
    case EM_X86_64:
        return "AMD x86-64";
    case EM_ARM:
        return "ARM";
    case EM_AARCH64:
        return "AArch64";
    case EM_MIPS:
        return "MIPS";
    case EM_PPC:
        return "PowerPC";
    default:
        return "Unknown ";
    }
}

std::string ELFHeader::PrintELFType(uint16_t e_type)
{
    switch (e_type)
    {
    case ET_NONE:
        return "No file type";

    case ET_REL:
        return "Relocatable file";

    case ET_EXEC:
        return "Executable file";

    case ET_DYN:
        return "DYN (Position-Independent Executable file)";

    case ET_CORE:
        return "Core file";

    default:
        return "Unknown";
    }
}

uint8_t ELFHeader::GetABIVersion()
{
    return is_32bit ? ehdr32.e_ident[EI_ABIVERSION]
                    : ehdr64.e_ident[EI_ABIVERSION];
}

std::string ELFHeader::GetVersionString()
{
    return (is_32bit ? ehdr32.e_ident[EI_VERSION]
                     : ehdr64.e_ident[EI_VERSION]) == EV_CURRENT
               ? "Current"
               : "Invalid";
}

void ELFHeader::ReadHeader()
{
    char elf_class_value[1];
    char magic_value[4];

    fd->seekg(4, std::ios::beg);
    fd->read(elf_class_value, 1);

    if (elf_class_value[0] == ELFCLASS32)
    {
        is_32bit = true;
        fd->seekg(0, std::ios::beg);
        fd->read(reinterpret_cast<char *>(&ehdr32), sizeof(Elf32_Ehdr));
    }
    else if (elf_class_value[0] == ELFCLASS64)
    {
        is_32bit = false;
        fd->seekg(0, std::ios::beg);
        fd->read(reinterpret_cast<char *>(&ehdr64), sizeof(Elf64_Ehdr));
    }
    else
    {
        throw std::runtime_error(
            "Error: Not a valid ELF file - ELF CLASS is Unknown!!!");
    }
}
