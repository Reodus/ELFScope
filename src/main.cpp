#include <getopt.h>

#include <fstream>
#include <iostream>
#include <map>

#include "elf_parser.h"

void Usage();
bool Dispatcher(std::map<elf_structs, bool> flags, ELFParser *parser);

int main(int argc, char **argv) {
    int c;
    std::map<elf_structs, bool> flags;
    std::string file_path;

    while (1) {
        int option_index = 0;
        static struct option long_options[] = {
            {"help", no_argument, 0, 'H'},
            {"all", no_argument, 0, 'a'},
            {"header", no_argument, 0, 'h'},
            {"segments", no_argument, 0, 's'},
            {"sections", no_argument, 0, 'S'},
            {0, 0, 0, 0}};

        c = getopt_long(argc, argv, "ahsSH", long_options, &option_index);
        if (c == -1) break;

        switch (c) {
            case 'h':
                flags[EHEADER] = true;
                break;
            case 's':
                flags[PHEADER] = true;
                break;
            case 'S':
                flags[SHEADER] = true;
                break;
            case 'a':
                flags[EHEADER] = true;
                flags[PHEADER] = true;
                flags[SHEADER] = true;
                break;
            case 'H':
            default:
                Usage();
                exit(EXIT_FAILURE);
        }
    }

    if (optind < argc) {
        file_path = argv[optind];
    } else {
        std::cerr << "Error: No file path provided." << std::endl;
        Usage();
        exit(EXIT_FAILURE);
    }

    ELFParser parser(file_path);

    if (!parser.IsValid()) {
        std::cerr << "Error: Not an ELF file - it has the wrong magic bytes at "
                     "the start !"
                  << std::endl;
        return -1;
    }

    Dispatcher(flags, &parser);
    return 0;
}

void Usage() {
    std::cout << "ELF Parser\n"
              << "----------------------\n"
              << "ELFParser [OPTIONS] [FILE_PATH]\n"
              << "Usage:\n"
              << "-H --help			shows this message\n"
              << "-a --all          Equivalent to: -h -s\n"
              << "-h --header		shows executable header\n"
              << "-s --segments     shows program header segments\n"
              << "-S --sections     shows section header table\n"
              << std::endl;
}

bool Dispatcher(std::map<elf_structs, bool> flags, ELFParser *parser) {
    if (!parser) {
        std::cerr << "Invalid parser instance." << std::endl;
        return false;
    }

    for (auto it = flags.begin(); it != flags.end(); it++) {
        if (it->first == EHEADER && it->second == true) {
            // Print each field of the ELF header
            std::cout << "ELF Header: " << std::endl;
            std::cout << "  Magic: ";
            for (auto value : parser->elf_header->GetIdent()) {
                std::cout << std::hex << std::setw(2) << std::setfill('0')
                          << static_cast<unsigned int>(value) << " ";
            }
            std::cout << std::endl;

            std::cout << "  Class: " << parser->elf_header->GetClass()
                      << std::endl;
            std::cout << "  Data: " << parser->elf_header->GetDataEncoding()
                      << std::endl;
            std::cout << "  Type: "
                      << parser->elf_header->PrintELFType(
                             parser->elf_header->GetType())
                      << std::endl;
            std::cout << "  Machine: "
                      << parser->elf_header->PrintMachine(
                             parser->elf_header->GetMachine())
                      << std::endl;
            std::cout << "  OS/ABI: " << parser->elf_header->GetOSABI()
                      << std::endl;
            std::cout << "  Version: " << parser->elf_header->GetVersion()
                      << std::endl;
            std::cout << "  Entry Point: 0x" << std::hex
                      << parser->elf_header->GetEntryPoint() << std::dec
                      << std::endl;
            std::cout << "  Program Header Offset: "
                      << parser->elf_header->GetProgramHeaderOffset()
                      << std::endl;
            std::cout << "  Section Header Offset: "
                      << parser->elf_header->GetSectionHeaderOffset()
                      << std::endl;
            std::cout << "  Flags: " << parser->elf_header->GetFlags()
                      << std::endl;
            std::cout << "  ELF Header Size: "
                      << parser->elf_header->GetHeaderSize() << std::endl;
            std::cout << "  Program Header Entry Size: "
                      << parser->elf_header->GetProgramHeaderEntrySize()
                      << std::endl;
            std::cout << "  Program Header Entry Count: "
                      << parser->elf_header->GetProgramHeaderEntryCount()
                      << std::endl;
            std::cout << "  Section Header Entry Size: "
                      << parser->elf_header->GetSectionHeaderEntrySize()
                      << std::endl;
            std::cout << "  Section Header Entry Count: "
                      << parser->elf_header->GetSectionHeaderEntryCount()
                      << std::endl;
            std::cout << "  Section Header String Table Index: "
                      << parser->elf_header->GetSectionHeaderStringIndex()
                      << std::endl;

            std::cout << std::endl;
            it->second = false;
        } else if (it->first == PHEADER && it->second == true) {
            std::cout << "Program Header Information:" << std::endl;
            for (size_t i = 0; i < parser->program_headers.size(); ++i) {
                auto &header = parser->program_headers[i];
                std::cout << "  Program Header 0x" << std::hex << i + 1 << ":"
                          << std::endl;
                std::cout << "    Type: "
                          << header->PrintSegmentType(header->GetType())
                          << std::endl;
                std::cout << "    Flags: 0x" << std::hex << header->GetFlags()
                          << std::dec << std::endl;
                std::cout << "    Offset: 0x" << std::hex << header->GetOffset()
                          << std::endl;
                std::cout << "    Virtual Address: 0x" << std::hex
                          << header->GetVirtualAddress() << std::endl;
                std::cout << "    Physical Address: 0x" << std::hex
                          << header->GetPhysicalAddress() << std::endl;
                std::cout << "    File Size: 0x" << std::hex
                          << header->GetFileSize() << std::endl;
                std::cout << "    Memory Size: 0x" << std::hex
                          << header->GetMemorySize() << std::endl;
                std::cout << "    Alignment: 0x" << std::hex
                          << header->GetAlignment() << std::endl;
                std::cout << std::endl;
            }
            it->second = false;
        } else if (it->first == SHEADER && it->second == true) {
            std::cout << "Section Header Table: " << std::endl;
            for (size_t i = 0; i < parser->section_headers.size(); ++i) {
                auto &header = parser->section_headers[i];

                std::cout << "  Name: "
                          << parser->GetStringFromStringTable(header->GetName())
                          << std::endl;
                std::cout << "  Type: " << std::hex
                          << header->PrintSectionType(header->GetType())
                          << std::endl;
                std::cout << "  Address: 0x" << std::hex
                          << header->GetVirtualAddress() << std::endl;
                std::cout << "  Offset: 0x" << std::hex << header->GetOffset()
                          << std::endl;
                std::cout << "  Size: " << std::dec << header->GetSize()
                          << std::endl;
                std::cout << "  Entry size: " << header->GetEntSize()
                          << std::endl;
                std::cout << "  Flag: 0x" << std::hex << header->GetFlags()
                          << std::endl;
                std::cout << "  Link: 0x" << std::hex << header->GetLink()
                          << std::endl;
                std::cout << "  Info: " << std::dec << header->GetInfo()
                          << std::endl;
                std::cout << "  Address Align: " << header->GetAddrAlign()
                          << std::endl;

                std::cout << std::endl;
            }
            it->second = false;
        }
    }

    return true;
}