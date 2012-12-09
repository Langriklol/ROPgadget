/*
** RopGadget - Release v3.4.2
** Jonathan Salwan - http://twitter.com/JonathanSalwan
** Allan Wirth - http://allanwirth.com/
** http://shell-storm.org
** 2012-11-11
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "ropgadget.h"

void display_elf_header(void)
{
  fprintf(stdout, "%sELF Header\n", YELLOW);
  fprintf(stdout, "============================================================%s\n\n", ENDC);
  fprintf(stdout, "entry        %s" ADDR_FORMAT "%s\n",	      RED, EHDR(->e_entry, Address), ENDC);
  fprintf(stdout, "phoff        %s" ADDR_FORMAT "%s\n",        RED, EHDR(->e_phoff, Address), ENDC);
  fprintf(stdout, "shoff        %s" ADDR_FORMAT "%s\n",        RED, EHDR(->e_shoff, Address), ENDC);
  fprintf(stdout, "flags        %s" ADDR_FORMAT "%s\n",        RED, EHDR(->e_flags, Size), ENDC);
  fprintf(stdout, "ehsize       %s" ADDR_FORMAT " (%2$d)%s\n",   RED, EHDR(->e_ehsize, Size), ENDC);
  fprintf(stdout, "phentsize    %s" ADDR_FORMAT " (%2$d)%s\n",   RED, EHDR(->e_phentsize, Size), ENDC);
  fprintf(stdout, "phnum        %s" ADDR_FORMAT " (%2$d)%s\n",   RED, EHDR(->e_phnum, Size), ENDC);
  fprintf(stdout, "shentsize    %s" ADDR_FORMAT " (%2$d)%s\n",   RED, EHDR(->e_shentsize, Size), ENDC);
  fprintf(stdout, "shnum        %s" ADDR_FORMAT " (%2$d)%s\n",   RED, EHDR(->e_shnum, Size), ENDC);
  fprintf(stdout, "shstrndx     %s" ADDR_FORMAT " (%2$d)%s\n\n\n", RED, EHDR(->e_shstrndx, Size), ENDC);
}

void display_symtab(void)
{
  t_list_symbols *tmp;
  int i;

  i = 0;
  tmp = list_symbols;
  fprintf(stdout, "%sSymbols Table\n", YELLOW);
  fprintf(stdout, "============================================================%s\n\n", ENDC);
  if (tmp == NULL)
    fprintf(stderr, "%s/!\\ no symbols in %s%s\n", RED, filemode.file, ENDC);
  else
    {
      fprintf(stderr, "%sidx  addr\tsize\t   name%s\n", GREEN, ENDC);
      while (tmp != NULL)
        {
          if (*tmp->name != '\0')
            {
              fprintf(stdout, "%s%.3x   %s" ADDR_FORMAT "\t" SIZE_FORMAT "   %s%s\n", GREEN, i, RED, tmp->st_value, tmp->st_size, ENDC, tmp->name);
              i++;
            }
          tmp = tmp->back;
        }
    }
  fprintf(stdout, "\n\n");
}

void display_program_header()
{
  int x = 0;

  fprintf(stdout, "%sProgram Header\n", YELLOW);
  fprintf(stdout, "============================================================%s\n\n", ENDC);
  while (x != pElf32_Header->e_phnum)
    {
      fprintf(stdout, "%s%s%s\n", YELLOW, get_seg(pElf32_Phdr->p_type), ENDC);
      fprintf(stdout, "\toffset %s0x%.8x%s ",  RED, pElf32_Phdr->p_offset, ENDC);
      fprintf(stdout, "vaddr %s0x%.8x%s ",  RED, pElf32_Phdr->p_vaddr, ENDC);
      fprintf(stdout, "paddr %s0x%.8x%s\n", RED, pElf32_Phdr->p_paddr, ENDC);
      fprintf(stdout, "\tfilesz %s0x%.8x%s ",  RED, pElf32_Phdr->p_filesz, ENDC);
      fprintf(stdout, "memsz %s0x%.8x%s ",  RED, pElf32_Phdr->p_memsz, ENDC);
      fprintf(stdout, "flags %s%s%s\n",   RED, get_flags(pElf32_Phdr->p_flags), ENDC);
      x++;
      pElf32_Phdr++;
    }
  pElf32_Phdr -= x;
  fprintf(stdout, "\n\n");
}

void display_section_header(void)
{
  Elf32_Shdr *pElf32_Shdr = (Elf32_Shdr *)filemode.data + pElf32_Header->e_shoff;
  char *ptrNameSection = NULL;
  int x = 0;

  while(x != pElf32_Header->e_shnum)
    {
      if (pElf32_Shdr->sh_type == SHT_STRTAB && pElf32_Shdr->sh_addr == 0)
        {
          ptrNameSection =  (char *)pMapElf + pElf32_Shdr->sh_offset;
          break;
        }
      x++;
      pElf32_Shdr++;
    }
    pElf32_Shdr -= x;
    x = 0;

    fprintf(stdout, "%sSection Header\n", YELLOW);
    fprintf(stdout, "============================================================%s\n\n", ENDC);
    fprintf(stdout, "%sidx\taddr\t\tsize\t\tsection%s\n", GREEN, ENDC);
    while (x != pElf32_Header->e_shnum)
      {
        fprintf(stdout, "%s%.2d%s\t", GREEN, x, ENDC);
        fprintf(stdout, "%s0x%.8x\t", RED, pElf32_Shdr->sh_addr);
        fprintf(stdout, "0x%.8x\t%s", pElf32_Shdr->sh_size, ENDC);
        fprintf(stdout, "%s\n", (char *)(ptrNameSection + pElf32_Shdr->sh_name));
        x++;
        pElf32_Shdr++;
      }
    pElf32_Shdr -= x;
    fprintf(stdout, "\n\n");
}
