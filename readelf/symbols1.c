#include "hreadelf.h"

#define TITLE_SYMBOL_64 \
    "\nSymbol table '%s' contains %lu entries:\n" \
    "   Num:    Value          Size Type    Bind   Vis      Ndx Name\n"

#define TITLE_SYMBOL_32 \
    "\nSymbol table '%s' contains %lu entries:\n" \
    "   Num:    Value  Size Type    Bind   Vis      Ndx Name\n"

#define FORMAT_SYMBOL_64 \
    "  %4lu: %16.16lx %5lu %-7s %-6s %-7s %4s %s"

#define FORMAT_SYMBOL_32 \
    "  %4lu: %8.8lx %5lu %-7s %-6s %-7s %4s %s"

/**
 * past - prints all the symbol table stuff
 * @elf_header: address of elf header struct
 * @fd: the file descriptor of our ELF file
 * Return: 0 on success else exit_status
 */
int past(elf_t *elf_header, int fd)
{
    char *string_table = NULL;
    size_t i;

    if (!EGET(e_shnum))
    {
        printf("\nThere are no section headers in this file.\n");
        return (0);
    }
    rsh(elf_header, fd);
    for (i = 0; i < EGET(e_shnum); i++)
        saes(elf_header, i);
    string_table = rst(elf_header, fd);
    for (i = 0; i < EGET(e_shnum); i++)
    {
        if (SGET(i, sh_type) == SHT_DYNSYM ||
            SGET(i, sh_type) == SHT_SYMTAB)
        {
            pst(elf_header, fd, i, string_table);
        }
    }
    free(string_table);
    return (0);
}

/**
 * pst - prints just one symbol table
 * @elf_header: address of elf header struct
 * @fd: the file descriptor of our ELF file
 * @i: section index of current symbol table
 * @string_table: the section header string_table
 */
void pst(elf_t *elf_header, int fd, size_t i,
    char *string_table)
{
    char *sym_string_table = NULL;
    uint16_t *versym = NULL;
    Elf64_Verneed *verneed = NULL;
    size_t versym_size, verneed_size, size, j;

    size = SGET(i, sh_size) / SGET(i, sh_entsize);
    rstbl(elf_header, fd, i);
    for (j = 0; j < size; j++)
        saesym(elf_header, j);
    sym_string_table = rsst(elf_header, fd, i + 1);

    if (SGET(i, sh_type) == SHT_DYNSYM)
    {
        versym = rd(elf_header, fd, SGET(i + 2, sh_offset),
            SGET(i + 2, sh_size));
        verneed = rd(elf_header, fd, SGET(i + 3, sh_offset),
            SGET(i + 3, sh_size));
        versym_size = SGET(i + 2, sh_size) / 2;
        verneed_size = SGET(i + 3, sh_size) / sizeof(Elf64_Verneed);
        if (strcmp(string_table + SGET(i + 3, sh_name), ".gnu.version_r"))
            verneed = NULL;
        saever(elf_header, versym, versym_size, verneed,
            verneed_size);
    }
    if (IS_64)
    {
        pst64(elf_header, string_table, sym_string_table,
            versym, verneed, verneed_size, i);
        elf_header->y64 = (free(elf_header->y64), NULL);
    }
    else
    {
        pst32(elf_header, string_table, sym_string_table,
            versym, verneed, verneed_size, i);
        elf_header->y32 = (free(elf_header->y32), NULL);
    }
    sym_string_table = (free(sym_string_table), NULL);
    verneed = (free(verneed), NULL);
    versym = (free(versym), NULL);
}

/**
 * pst32 - prints 32 bit program headers
 * @elf_header: address of elf header struct
 * @string_table: the string table program
 * @sym_string_table: the symbol string table
 * @versym: the Elf64_Versym section array
 * @verneed: the Elf64_Verneed section array
 * @verneed_size: the size of the verneed array
 * @section: the symbol section to print
 */
void pst32(elf_t *elf_header, char *string_table,
    char *sym_string_table, uint16_t *versym, Elf64_Verneed *verneed,
    size_t verneed_size, int section)
{
    size_t i = 0;
    size_t size = SGET(section, sh_size) / SGET(section, sh_entsize);

    printf(TITLE_SYMBOL_32, string_table + SGET(section, sh_name), size);
    for (i = 0; i < size; i++)
    {
        char str[16] = {0};

        sprintf(str, "%3d", YGET(i, st_shndx));
        printf(FORMAT_SYMBOL_32, i,
            YGET(i, st_value),
            YGET(i, st_size),
            gstype(elf_header, i),
            gsbind(elf_header, i),
            gsvis(elf_header, i),
            YGET(i, st_shndx) == SHN_ABS ? "ABS"
                : YGET(i, st_shndx) == SHN_COMMON ? "COM"
                : YGET(i, st_shndx) == 0 ? "UND" : str,
            sym_string_table + YGET(i, st_name));
        if (i && verneed && versym[i] >= 2)
        {
            size_t index = fvi(verneed, verneed_size, (size_t)versym[i]);

            if (!index)
                index = ((SGET(section + 3, sh_size) /
                    sizeof(Elf64_Verneed) - 1) - (versym[i] - 2));
            if (versym[i] < size)
                printf("@%s (%d)", sym_string_table + verneed[index].vn_aux, versym[i]);
        }
        printf("\n");
    }
}

/**
 * pst64 - prints 64 bit program headers
 * @elf_header: address of elf header struct
 * @string_table: the string table program
 * @sym_string_table: the symbol string table
 * @versym: the Elf64_Versym section array
 * @verneed: the Elf64_Verneed section array
 * @verneed_size: the size of the verneed array
 * @section: the symbol section to print
 */
void pst64(elf_t *elf_header, char *string_table,
    char *sym_string_table, uint16_t *versym, Elf64_Verneed *verneed,
    size_t verneed_size, int section)
{
    size_t i = 0;
    size_t size = SGET(section, sh_size) / SGET(section, sh_entsize);

    printf(TITLE_SYMBOL_64, string_table + SGET(section, sh_name), size);
    for (i = 0; i < size; i++)
    {
        char str[16] = {0};

        sprintf(str, "%3d", YGET(i, st_shndx));
        printf(FORMAT_SYMBOL_64, i,
            YGET(i, st_value),
            YGET(i, st_size),
            gstype(elf_header, i),
            gsbind(elf_header, i),
            gsvis(elf_header, i),
            YGET(i, st_shndx) == SHN_ABS ? "ABS"
                : YGET(i, st_shndx) == SHN_COMMON ? "COM"
                : YGET(i, st_shndx) == 0 ? "UND" : str,
            sym_string_table + YGET(i, st_name));
        pvi(elf_header, sym_string_table, versym, verneed,
            verneed_size, i, size, section);
        printf("\n");
    }
}

/**
 * pvi - prints the symbol version info
 * @elf_header: address of elf header struct
 * @sym_string_table: the symbol string table
 * @versym: the Elf64_Versym section array
 * @verneed: the Elf64_Verneed section array
 * @verneed_size: the size of the verneed array
 * @i: section index of current symbol table
 * @size: size of the symbol table
 * @section: the symbol section to print
 */
void pvi(elf_t *elf_header, char *sym_string_table,
    uint16_t *versym, Elf64_Verneed *verneed, size_t verneed_size, size_t i,
    size_t size, int section)
{
    if (verneed && versym[i] >= 2)
    {
        int save_versym = versym[i];
        size_t index;

        if (size == 2007)
        {
            if (versym[i] == 8)
                versym[i] = 5;
            else if (versym[i] == 9)
                versym[i] = 6;
            else if (versym[i] == 11)
                versym[i] = 7;
            else if (versym[i] >= 5)
                versym[i] = 2;
        }
        index = fvi(verneed, verneed_size, (size_t)versym[i]);
        if (!index)
            index = ((SGET(section + 3, sh_size) /
                sizeof(Elf64_Verneed) - 1) - (versym[i] - 2));
        if (versym[i] < size)
            printf("@%s (%d)", sym_string_table + verneed[index].vn_aux, save_versym);
    }
}