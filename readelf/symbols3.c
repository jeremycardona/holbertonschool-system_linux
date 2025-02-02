#include "hreadelf.h"

/**
 * gstype - gets the STT type
 * @elf_header: the internal header
 * @i: index of the current symbol struct
 * Return: string type
 */
char *gstype(elf_t *elf_header, size_t i)
{
    switch (YGET(i, st_info) & 0xf)
    {
        case STT_NOTYPE: return ("NOTYPE");
        case STT_OBJECT: return ("OBJECT");
        case STT_FUNC: return ("FUNC");
        case STT_SECTION: return ("SECTION");
        case STT_FILE: return ("FILE");
        case STT_COMMON: return ("COMMON");
        case STT_TLS: return ("TLS");
        case STT_NUM: return ("NUM");
        case STT_LOOS: return ("LOOS");
        case STT_HIOS: return ("HIOS");
        case STT_LOPROC: return ("LOPROC");
        case STT_HIPROC: return ("HIPROC");
        default: return ("UNKNOWN");
    }
}

/**
 * gsbind - gets the STB type
 * @elf_header: the internal header
 * @i: index of the current symbol struct
 * Return: string bind type
 */
char *gsbind(elf_t *elf_header, size_t i)
{
    switch (YGET(i, st_info) >> 4)
    {
        case STB_LOCAL: return ("LOCAL");
        case STB_GLOBAL: return ("GLOBAL");
        case STB_WEAK: return ("WEAK");
        case STB_NUM: return ("NUM");
        case STB_LOOS: return ("LOOS");
        case STB_HIOS: return ("HIOS");
        case STB_LOPROC: return ("LOPROC");
        case STB_HIPROC: return ("HIPROC");
        default: return ("UNKNOWN");
    }
}

/**
 * gsvis - gets the STV type
 * @elf_header: the internal header
 * @i: index of the current symbol struct
 * Return: string visibility type
 */
char *gsvis(elf_t *elf_header, size_t i)
{
    switch (YGET(i, st_other) & 0x3)
    {
        case STV_DEFAULT: return ("DEFAULT");
        case STV_INTERNAL: return ("INTERNAL");
        case STV_HIDDEN: return ("HIDDEN");
        case STV_PROTECTED: return ("PROTECTED");
        default: return ("UNKNOWN");
    }
}