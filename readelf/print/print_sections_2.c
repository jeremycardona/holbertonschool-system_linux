void print_shdrs32(elf_t *elf_header, char *string_table);
void print_shdrs64(elf_t *elf_header, char *string_table);
char *get_sh_flags(elf_t *elf_header, size_t i);

void print_section_headers32(elf_t *elf_header, char *string_table) {
    // Implementation for printing 32-bit section headers
}

void print_section_headers64(elf_t *elf_header, char *string_table) {
    // Implementation for printing 64-bit section headers
}

char *get_section_flags(elf_t *elf_header, size_t i) {
    // Implementation for retrieving section flags
}