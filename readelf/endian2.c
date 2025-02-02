#include "hreadelf.h"

/**
 * saep - switches all endians if need be
 * @h: address of our internal struct
 * @i: index of this section
 */
void saep(elf_t *h, size_t i)
{
    if (IS_BE(h->e64) && !IS_32(h->e64))
    {
        h->p64[i].p_type = se4(h->p64[i].p_type);
        h->p64[i].p_offset = se8(h->p64[i].p_offset);
        h->p64[i].p_vaddr = se8(h->p64[i].p_vaddr);
        h->p64[i].p_paddr = se8(h->p64[i].p_paddr);
        h->p64[i].p_filesz = se8(h->p64[i].p_filesz);
        h->p64[i].p_memsz = se8(h->p64[i].p_memsz);
        h->p64[i].p_align = se8(h->p64[i].p_align);
        h->p64[i].p_flags = se4(h->p64[i].p_flags);
    }
    if (IS_BE(h->e64) && IS_32(h->e64))
    {
        h->p32[i].p_type = se4(h->p32[i].p_type);
        h->p32[i].p_offset = se4(h->p32[i].p_offset);
        h->p32[i].p_vaddr = se4(h->p32[i].p_vaddr);
        h->p32[i].p_paddr = se4(h->p32[i].p_paddr);
        h->p32[i].p_filesz = se4(h->p32[i].p_filesz);
        h->p32[i].p_memsz = se4(h->p32[i].p_memsz);
        h->p32[i].p_align = se4(h->p32[i].p_align);
        h->p32[i].p_flags = se4(h->p32[i].p_flags);
    }
}

/**
 * saesym - switches all endians if need be
 * @h: address of our internal struct
 * @i: index of this section
 */
void saesym(elf_t *h, size_t i)
{
    if (IS_BE(h->e64) && !IS_32(h->e64))
    {
        h->y64[i].st_name = se4(h->y64[i].st_name);
        h->y64[i].st_shndx = se2(h->y64[i].st_shndx);
        h->y64[i].st_value = se8(h->y64[i].st_value);
        h->y64[i].st_size = se8(h->y64[i].st_size);
    }
    if (IS_BE(h->e64) && IS_32(h->e64))
    {
        h->y32[i].st_name = se4(h->y32[i].st_name);
        h->y32[i].st_shndx = se2(h->y32[i].st_shndx);
        h->y32[i].st_value = se4(h->y32[i].st_value);
        h->y32[i].st_size = se4(h->y32[i].st_size);
    }
    (void)i;
}

/**
 * saever - switches all endians if need be
 * @h: address of our internal struct
 * @versym: version symbol table
 * @versym_size: size of version symbol table
 * @verneed: version need table
 * @verneed_size: size of version need table
 */
void saever(elf_t *h, uint16_t *versym, size_t versym_size,
    Elf64_Verneed *verneed, size_t verneed_size)
{
    size_t i = 0;

    if (!IS_BE(h->e64))
        return;

    for (i = 0; i < versym_size; i++)
        versym[i] = se2(versym[i]);
    for (i = 0; i < verneed_size; i++)
    {
        verneed[i].vn_version = se2(verneed[i].vn_version);
        verneed[i].vn_cnt = se2(verneed[i].vn_cnt);
        verneed[i].vn_file = se4(verneed[i].vn_file);
        verneed[i].vn_aux = se4(verneed[i].vn_aux);
        verneed[i].vn_next = se4(verneed[i].vn_next);
    }
    (void)versym;
}