#ifndef HREADELF_H
#define HREADELF_H

#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define USAGE "0-hreadelf elf_filename\n"
#define MYNAME "0-hreadelf"

#define ERR_PREFIX MYNAME ": Error: "
#define ERR_NO_ENTRY ERR_PREFIX \
    "'%s': No such file\n"
#define ERR_NO_ACCESS ERR_PREFIX \
    "Input file '%s' is not readable.\n"
#define ERR_NOT_MAGIC ERR_PREFIX \
    "Not an ELF file - it has the wrong magic bytes at the start\n"

#define IS_32(x) ((x).e_ident[EI_CLASS] == ELFCLASS32)
#define IS_64 ((elf_header->e64).e_ident[EI_CLASS] == ELFCLASS64)
#define IS_BE(x) ((x).e_ident[EI_DATA] == ELFDATA2MSB)
#define EGET(x) \
    (IS_32(elf_header->e64) ? elf_header->e32.x : elf_header->e64.x)
#define SGET(i, x) \
    (IS_32(elf_header->e64) ? elf_header->s32[i].x : elf_header->s64[i].x)
#define PGET(i, x) \
    (IS_32(elf_header->e64) ? elf_header->p32[i].x : elf_header->p64[i].x)
#define YGET(i, x) \
    (IS_32(elf_header->e64) ? elf_header->y32[i].x : elf_header->y64[i].x)

/**
 * struct Elf - stores 32/64 structs and other data
 * @e64: the 64 bit struct
 * @e32: the 32 bit struct
 * @s64: the 64 bit struct section array
 * @s32: the 32 bit struct section array
 * @p64: the 64 bit struct program array
 * @p32: the 32 bit struct program array
 * @y64: the 64 bit struct symbol array
 * @y32: the 32 bit struct symbol array
 */
typedef struct Elf
{
    Elf64_Ehdr e64;
    Elf32_Ehdr e32;
    Elf64_Shdr *s64;
    Elf32_Shdr *s32;
    Elf64_Phdr *p64;
    Elf32_Phdr *p32;
    Elf64_Sym *y64;
    Elf32_Sym *y32;

} elf_t;

/* utils.c */
int of(char *name, int silent); /* open_file */
int ce(char *elf_header); /* check_elf */
void pb(void *ptr, size_t n); /* print_bytes */

/* endian.c */
unsigned short se2(unsigned short n); /* switch_endian2 */
unsigned int se4(unsigned int n); /* switch_endian4 */
unsigned long se8(unsigned long n); /* switch_endian8 */
void sae(elf_t *elf_header); /* switch_all_endian */
void saes(elf_t *h, size_t i); /* switch_all_endian_section */

/* endian2.c */
void saep(elf_t *h, size_t i); /* switch_all_endian_program */
void saesym(elf_t *h, size_t i); /* switch_all_endian_symbol */
void saever(elf_t *h, uint16_t *versym, size_t versym_size,
    Elf64_Verneed *verneed, size_t verneed_size); /* switch_all_endian_ver */

/* print_header_1.c */
int ph(elf_t *elf_header); /* print_header */
int pm(Elf64_Ehdr *elf_header); /* print_magic */
int pc(Elf64_Ehdr *elf_header); /* print_class */
int pd(Elf64_Ehdr *elf_header); /* print_data */
int pv(Elf64_Ehdr *elf_header); /* print_version */

/* print_header_2.c */
int posabi(Elf64_Ehdr *elf_header); /* print_osabi */
int posabim(Elf64_Ehdr *elf_header); /* print_osabi_more */
int pav(Elf64_Ehdr *elf_header); /* print_abiversion */
int pt(elf_t *elf_header); /* print_type */
int pe(elf_t *elf_header); /* print_entry */

/* print_header_3.c */
int pmach(Elf64_Ehdr *elf_header); /* print_machine */
int pev(elf_t *elf_header); /* print_e_version */
int pph(elf_t *elf_header); /* print_program_headers */
char *gmach(Elf64_Ehdr *elf_header); /* get_machine */
char *gmach2(Elf64_Ehdr *elf_header); /* get_machine2 */

/* print_header_4.c */
int psh(elf_t *elf_header); /* print_section_headers */
int pf(elf_t *elf_header); /* print_flags */

/* elf1.c */
char *gt(elf_t *elf_header); /* get_type */

/* print_programs1.c */
int pphf(elf_t *elf_header, int fd); /* print_program_headers_full */
void pph32(elf_t *elf_header, char *string_table, int fd); /* print_program_headers32 */
void pph64(elf_t *elf_header, char *string_table, int fd); /* print_program_headers64 */
void rph(elf_t *elf_header, int fd); /* read_program_headers */
char *gsegtype(unsigned long p_type); /* get_segment_type */

/* print_programs2.c */
int ps2sm(elf_t *elf_header, char *string_table); /* print_section_to_segment_mapping */

/* print_sections_1.c */
int pshf(elf_t *elf_header, int fd); /* print_section_headers_full */
void rsh(elf_t *elf_header, int fd); /* read_section_headers */
char *rst(elf_t *elf_header, int fd); /* read_string_table */
char *gst(unsigned int sh_type); /* get_section_type */

/* print_sections_2.c */
void psh32(elf_t *elf_header, char *string_table); /* print_section_headers32 */
void psh64(elf_t *elf_header, char *string_table); /* print_section_headers64 */
char *gsf(elf_t *elf_header, size_t i); /* get_section_flags */

/* print_symbols1.c */
int past(elf_t *elf_header, int fd); /* print_all_symbol_tables */
void pst(elf_t *elf_header, int fd, size_t i, char *string_table); /* print_symbol_table */
void pst32(elf_t *elf_header, char *string_table, char *sym_string_table, uint16_t *versym, Elf64_Verneed *verneed, size_t verneed_size, int section); /* print_symbol_table32 */
void pst64(elf_t *elf_header, char *string_table, char *sym_string_table, uint16_t *versym, Elf64_Verneed *verneed, size_t verneed_size, int section); /* print_symbol_table64 */
void pvi(elf_t *elf_header, char *sym_string_table, uint16_t *versym, Elf64_Verneed *verneed, size_t verneed_size, size_t i, size_t size, int section); /* print_verneed_info */

/* print_symbols2.c */
void rstbl(elf_t *elf_header, int fd, int i); /* read_symbol_table */
char *rsst(elf_t *elf_header, int fd, int i); /* read_symbol_string_table */
size_t fvi(Elf64_Verneed *verneed, size_t verneed_size, size_t index); /* find_verneed_index */
void pvt(elf_t *elf_header, int fd, int i, char *sym_string_table, uint16_t *versym, Elf64_Verneed *verneed); /* print_verneed_table */
void *rd(elf_t *elf_header, int fd, unsigned long offset, long size); /* read_data */

/* print_symbols3.c */
char *gstype(elf_t *elf_header, size_t i); /* get_sym_type */
char *gsbind(elf_t *elf_header, size_t i); /* get_sym_bind */
char *gsvis(elf_t *elf_header, size_t i); /* get_sym_visibility */

#endif /* HREADELF_H */