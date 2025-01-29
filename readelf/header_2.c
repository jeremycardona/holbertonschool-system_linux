#include "hreadelf.h"

/**
 * posabi - prints to OS ABI version
 * @elf_header: address of elf header struct
 * Return: 0 on success else exit_status
 */
int posabi(Elf64_Ehdr *elf_header)
{
    printf("  OS/ABI:                            ");
    switch (elf_header->e_ident[EI_OSABI])
    {
        case ELFOSABI_NONE:
            printf("UNIX - System V");
            break;
        case ELFOSABI_HPUX:
            printf("UNIX - HP-UX");
            break;
        case ELFOSABI_NETBSD:
            printf("UNIX - NetBSD");
            break;
        case ELFOSABI_LINUX:
            printf("UNIX - Linux");
            break;
        case ELFOSABI_SOLARIS:
            printf("UNIX - Solaris");
            break;
        case ELFOSABI_AIX:
            printf("UNIX - AIX");
            break;
        case ELFOSABI_IRIX:
            printf("UNIX - IRIX");
            break;
        case ELFOSABI_FREEBSD:
            printf("UNIX - FreeBSD");
            break;
        case ELFOSABI_TRU64:
            printf("UNIX - TRU64");
            break;
        default:
            return (posabim(elf_header));
        break;
    }
    printf("\n");
    return (0);
}

/**
 * posabim - prints ELF osabi more
 * @elf_header: address of elf header struct
 * Return: 0 on success else exit_status
 */
int posabim(Elf64_Ehdr *elf_header)
{
    switch (elf_header->e_ident[EI_OSABI])
    {
        case ELFOSABI_MODESTO:
            printf("Novell - Modesto");
            break;
        case ELFOSABI_OPENBSD:
            printf("UNIX - OpenBSD");
            break;
        case ELFOSABI_STANDALONE:
            printf("Standalone App");
            break;
        case ELFOSABI_ARM:
            printf("ARM");
            break;
        default:
            printf("<unknown: %x>", elf_header->e_ident[EI_OSABI]);
            break;
    }
    printf("\n");
    return (0);
}

/**
 * pav  - prints ELF ABI version
 * @elf_header: address of elf header struct
 * Return: 0 on success else exit_status
 */
int pav(Elf64_Ehdr *elf_header)
{
    printf("  ABI Version:                       %d\n",
        elf_header->e_ident[EI_ABIVERSION]);
    return (0);
}

/**
 * pt - prints the ELF type
 * @elf_header: address of elf header struct
 * Return: 0 on success else exit_status
 */
int pt(elf_t *elf_header)
{
    printf("  Type:                              %s\n", gt(elf_header));
    return (0);
}

/**
 * pe - prints the ELF entry point address
 * @elf_header: address of elf header struct
 * Return: 0 on success else exit_status
 */
int pe(elf_t *elf_header)
{
    if (IS_32(elf_header->e64))
        printf("  Entry point address:               0x%lx\n", EGET(e_entry));
    else
        printf("  Entry point address:               0x%lx\n", EGET(e_entry));
    return (0);
}