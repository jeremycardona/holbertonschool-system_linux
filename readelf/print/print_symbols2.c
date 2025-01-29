void read_sym_tbl(elf_t *elf_header, int fd, int i);
char *read_sym_str_tbl(elf_t *elf_header, int fd, int i);
size_t find_ver_idx(Elf64_Verneed *verneed, size_t verneed_size, size_t index);
void print_ver_tbl(elf_t *elf_header, int fd, int i, char *sym_string_table, uint16_t *versym, Elf64_Verneed *verneed);
void *read_data(elf_t *elf_header, int fd, unsigned long offset, long size);