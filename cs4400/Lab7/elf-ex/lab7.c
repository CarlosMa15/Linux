#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <elf.h>

/* Given the in-memory ELF header pointer as `ehdr` and a section
   header pointer as `shdr`, returns a pointer to the memory that
   contains the in-memory content of the section */
#define AT_SEC(ehdr, shdr) ((void *)(ehdr) + (shdr)->sh_offset)

/*Elf64_Shdr*/ void *section_by_index(Elf64_Ehdr *ehdr, int idx) {
  
}

Elf64_Shdr *section_by_name(Elf64_Ehdr *ehdr, char *name) {
  Elf64_Shdr* shdrs = (void*)ehdr + ehdr->e_shoff;
  char* strs = AT_SEC(ehdr, shdrs + ehdr->e_shstrndx);
  int index = 0;
  for (index; index < ehdr->e_shnum; index++) 
  {
    char* currentSection = strs + shdrs[index].sh_name;
    if (strcmp(currentSection, name) == 0) {
      printf("%d %s\n", index,strs + shdrs[index].sh_name);
      return (void*)ehdr + ehdr->e_shoff;// + index;
    }
  }
  return (void*)ehdr + ehdr->e_shoff;
}

static void check_for_shared_object(Elf64_Ehdr *ehdr);
static void fail(char *reason, int err_code);
void print_symbols(Elf64_Ehdr* ehdr);

int main(int argc, char **argv) 
{
  int fd;
  size_t len;
  void *p;
  Elf64_Ehdr *ehdr;

  if (argc != 2)
    fail("expected one file on the command line", 0);

  /* Open the shared-library file */
  fd = open(argv[1], O_RDONLY);
  if (fd == -1)
    fail("could not open file", errno);

  /* Find out how big the file is: */
  len = lseek(fd, 0, SEEK_END);

  /* Map the whole file into memory: */
  p = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
  if (p == (void*)-1)
    fail("mmap failed", errno);

  /* Since the ELF file starts with an ELF header, the in-memory image
     can be cast to a `Elf64_Ehdr *` to inspect it: */
  ehdr = (Elf64_Ehdr *)p;

  /* Check that we have the right kind of file: */
  check_for_shared_object(ehdr);

  /* Your work for parts 1-6 goes here */

  /*
  //Part 1
  printf("%d\n",ehdr->e_shoff);
  printf("%d\n",ehdr->e_shstrndx);

  //Part2
  Elf64_Shdr* shdrs = (void*)ehdr + ehdr->e_shoff; //What type is shdrs??
  char* strs = AT_SEC(ehdr, shdrs + ehdr->e_shstrndx); //What type is strs??
  printf("%s\n", strs+1);

  //Part3
  int numShdr = ehdr->e_shnum; 
  printf("%d\n", numShdr); 
  int index = 0; 
  for (index; index < numShdr; index++) 
  {
  	printf("%d %s\n", index, strs + shdrs[index].sh_name); 
  }

  //Part4
  int numShdr = ehdr->e_shnum;
  int index = 0;
  for (index; index < numShdr; index++)
    {
      char* currentSection = strs + shdrs[index].sh_name;
      if (strcmp(currentSection, ".data") == 0) {
	printf("%d\n", index);
      }
    }
  */

  Elf64_Shdr *dynsym_shdr = section_by_name(ehdr, ".dynsym");
  Elf64_Sym *syms = AT_SEC(ehdr, dynsym_shdr);
  char *strs = AT_SEC(ehdr, section_by_name(ehdr, ".dynstr"));
  int i, count = dynsym_shdr->sh_size / sizeof(Elf64_Sym);
  for (i = 0; i < count; i++) {
    printf("%s\n", strs + syms[i].st_name);
  }

  return 0;
}

/* Just do a little bit of error-checking:
   Make sure we're dealing with an ELF file. */
static void check_for_shared_object(Elf64_Ehdr *ehdr) 
{
  if ((ehdr->e_ident[EI_MAG0] != ELFMAG0)
      || (ehdr->e_ident[EI_MAG1] != ELFMAG1)
      || (ehdr->e_ident[EI_MAG2] != ELFMAG2)
      || (ehdr->e_ident[EI_MAG3] != ELFMAG3))
    fail("not an ELF file", 0);

  if (ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    fail("not a 64-bit ELF file", 0);

  if (ehdr->e_type != ET_DYN)
    fail("not a shared-object file", 0);
}

static void fail(char *reason, int err_code) 
{
  fprintf(stderr, "%s (%d)\n", reason, err_code);
  exit(1);
}
