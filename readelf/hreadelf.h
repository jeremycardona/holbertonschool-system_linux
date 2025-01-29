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
int of(char *name, int silent);
int ce(char *elf_header);
void pb(void *ptr, size_t n);

/* endian.c */
unsigned short se2(unsigned short n);
unsigned int se4(unsigned int n);
unsigned long se8(unsigned long n);
void sae(elf_t *elf_header);
void saes(elf_t *h, size_t i);

/* print_header_1.c */
int ph(elf_t *elf_header);
int pm(Elf64_Ehdr *elf_header);
int pc(Elf64_Ehdr *elf_header);
int pd(Elf64_Ehdr *elf_header);
int pv(Elf64_Ehdr *elf_header);

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

#endif /* HREADELF_H */