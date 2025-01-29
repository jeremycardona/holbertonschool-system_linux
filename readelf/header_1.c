#include "hreadelf.h"

/**
 * ph - calls all the print functions
 * @elf_header: address of elf header struct
 * Return: 0 on success else exit_status
 */
int ph(elf_t *elf_header)
{
    printf("ELF Header:\n");
    pm(&elf_header->e64);
    pc(&elf_header->e64);
    pd(&elf_header->e64);
    pv(&elf_header->e64);
    posabi(&elf_header->e64);
    pav(&elf_header->e64);
    pt(elf_header);
    pmach(&elf_header->e64);
    pev(elf_header);
    pe(elf_header);
    pph(elf_header);
    psh(elf_header);
    pf(elf_header);
    return (0);
}

/**
 * pm - prints the 16 bytes of ELF header
 * @elf_header: address of elf header struct
 * Return: 0 on success else exit_status
 */
int pm(Elf64_Ehdr *elf_header)
{
    size_t i;

    printf("  Magic:   ");
    for (i = 0; i < EI_NIDENT; i++)
        printf("%.2x ", elf_header->e_ident[i]);
    printf("\n");
    return (0);
}

/**
 * pc - prints the byte architecture class
 * @elf_header: address of elf header struct
 * Return: 0 on success else exit_status
 */
int pc(Elf64_Ehdr *elf_header)
{
    printf("  Class:                             ");
    switch (elf_header->e_ident[EI_CLASS])
    {
        case ELFCLASS64:
            printf("ELF64");
        break;
        case ELFCLASS32:
            printf("ELF32");
        break;
        case ELFCLASSNONE:
            printf("none");
        break;
        default:
            printf("<unknown: %x>", elf_header->e_ident[EI_CLASS]);
        break;
    }
    printf("\n");
    return (0);
}

/**
 * pd - prints the endianness type
 * @elf_header: address of elf header struct
 * Return: 0 on success else exit_status
 */
int pd(Elf64_Ehdr *elf_header)
{
    printf("  Data:                              ");
    switch (elf_header->e_ident[EI_DATA])
    {
        case ELFDATA2LSB:
            printf("2's complement, little endian");
        break;
        case ELFDATA2MSB:
            printf("2's complement, big endian");
        break;
        case ELFDATANONE:
            printf("none");
        break;
        default:
            printf("<unknown: %x>", elf_header->e_ident[EI_DATA]);
        break;
    }
    printf("\n");
    return (0);
}

/**
 * pv - prints the ELF version
 * @elf_header: address of elf header struct
 * Return: 0 on success else exit_status
 */
int pv(Elf64_Ehdr *elf_header)
{
    printf("  Version:                           %d ",
        elf_header->e_ident[EI_VERSION]);
    if (elf_header->e_ident[EI_VERSION] == EV_CURRENT)
        printf("(current)");
    else if (elf_header->e_ident[EI_VERSION] != EV_NONE)
        printf("<unknown: %x", elf_header->e_ident[EI_VERSION]);
    printf("\n");
    return (0);
}