#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <elf.h>
#include "decode.h"

/* Given the in-memory ELF header pointer as `ehdr` and a section
   header pointer as `shdr`, returns a pointer to the memory that
   contains the in-memory content of the section */

#define AT_SEC(ehdr, shdr) ((void *)(ehdr) + (shdr)->sh_offset)

/*********************************************************
* Gets the section header of the given index
*********************************************************/
Elf64_Shdr *section_by_index(Elf64_Ehdr *ehdr, int idx) {
  Elf64_Shdr* shdrs = (void*)ehdr + ehdr->e_shoff;
  return &shdrs[idx];
}

/*********************************************************
* Gets the section header of the given name
*********************************************************/
Elf64_Shdr *section_by_name(Elf64_Ehdr *ehdr, char *name) {
  Elf64_Shdr* shdrs = (void*)ehdr + ehdr->e_shoff;
  char* strs = AT_SEC(ehdr, shdrs + ehdr->e_shstrndx);
  int index = 0;
  for(index; index < ehdr->e_shnum; index++) {
    char* currentSection = strs + shdrs[index].sh_name;
    if (strcmp(currentSection, name) == 0) {
      return &shdrs[index];
    }
  }
  return NULL;
}

/************************************************************
* Gets the name of function being called by the jump address
************************************************************/
char *getMethodName(Elf64_Ehdr *ehdr,Elf64_Addr addr) {
	instruction_t *ins = malloc(sizeof(instruction_t));
	Elf64_Shdr * plt_shdr = section_by_name(ehdr,".plt");
	code_t *code = AT_SEC(ehdr, plt_shdr) + (addr - plt_shdr->sh_addr);
	decode(ins,code,addr);
	
	Elf64_Shdr *rela_dyn_shdr = section_by_name(ehdr, ".rela.plt");
  	Elf64_Rela *relas = AT_SEC(ehdr, rela_dyn_shdr);
  	int count = rela_dyn_shdr->sh_size / sizeof(Elf64_Rela);
  	Elf64_Shdr *dynsym_shdr = section_by_name(ehdr, ".dynsym");
  	Elf64_Sym *syms = AT_SEC(ehdr, dynsym_shdr);
  	char *strs = AT_SEC(ehdr, section_by_name(ehdr,".dynstr"));
  	int i;
  	for (i = 0; i < count; i++) {
  		if(relas[i].r_offset == ins->addr) {
  			return strs + syms[ELF64_R_SYM(relas[i].r_info)].st_name;
  		}
  	}
  	
	return "NO METHOD EXIST AQUIRABLE WITH YOUR INFO";
}

/**********************************************************
* Gets the name of the variable being reference
**********************************************************/
char *getVariableName(Elf64_Ehdr *ehdr,Elf64_Addr addr) {
	Elf64_Shdr *rela_dyn_shdr = section_by_name(ehdr, ".rela.dyn");
  	Elf64_Rela *relas = AT_SEC(ehdr, rela_dyn_shdr);
  	int count = rela_dyn_shdr->sh_size / sizeof(Elf64_Rela);
  	Elf64_Shdr *dynsym_shdr = section_by_name(ehdr, ".dynsym");
  	Elf64_Sym *syms = AT_SEC(ehdr, dynsym_shdr);
  	char *strs = AT_SEC(ehdr, section_by_name(ehdr,".dynstr"));
  	int i;
  	for (i = 0; i < count; i++) {
  		if(relas[i].r_offset == addr) {
  			return strs + syms[ELF64_R_SYM(relas[i].r_info)].st_name;
  		}
  	}

  	return "NO VARIABLE EXIST AQUIRABLE WITH YOUR INFO";
}

/***********************************************************************
* This method determines if the given variable is protected (Global)
***********************************************************************/
int isProtected(Elf64_Ehdr *ehdr, Elf64_Addr addr) {
	if (strlen(getVariableName(ehdr,addr)) > 10) {
		if( getVariableName(ehdr,addr)[0] == 'p' &&
  			getVariableName(ehdr,addr)[1] == 'r' &&
  			getVariableName(ehdr,addr)[2] == 'o' &&
  			getVariableName(ehdr,addr)[3] == 't' &&
  			getVariableName(ehdr,addr)[4] == 'e' &&
  			getVariableName(ehdr,addr)[5] == 'c' &&
  			getVariableName(ehdr,addr)[6] == 't' &&
  			getVariableName(ehdr,addr)[7] == 'e' &&
  			getVariableName(ehdr,addr)[8] == 'd' &&
  			getVariableName(ehdr,addr)[9] == '_') {  
				return 1;
  			}
  			else { 
  				return 0;
  			}
  	} else {
  		return 0;
  	}
}

/***********************************************************************
* This method determines if the given method is open_it
***********************************************************************/
int isOpenFunction(Elf64_Ehdr *ehdr, Elf64_Addr addr) {
	if (strlen(getMethodName(ehdr,addr)) == 7) {
		if( getMethodName(ehdr,addr)[0] == 'o' &&
  			getMethodName(ehdr,addr)[1] == 'p' &&
  			getMethodName(ehdr,addr)[2] == 'e' &&
  			getMethodName(ehdr,addr)[3] == 'n' &&
  			getMethodName(ehdr,addr)[4] == '_' &&
  			getMethodName(ehdr,addr)[5] == 'i' &&
  			getMethodName(ehdr,addr)[6] == 't') {  
				return 1;
  			}
  			else { 
  				return 0;
  			}
  	} else {
  		return 0;
  	}
}

/***********************************************************************
* This method determines if the given method is close_it
***********************************************************************/
int isCloseFunction(Elf64_Ehdr *ehdr, Elf64_Addr addr) {
	if (strlen(getMethodName(ehdr,addr)) == 8) {
		if( getMethodName(ehdr,addr)[0] == 'c' &&
  			getMethodName(ehdr,addr)[1] == 'l' &&
  			getMethodName(ehdr,addr)[2] == 'o' &&
  			getMethodName(ehdr,addr)[3] == 's' &&
  			getMethodName(ehdr,addr)[4] == 'e' &&
  			getMethodName(ehdr,addr)[5] == '_' &&
  			getMethodName(ehdr,addr)[6] == 'i' &&
  			getMethodName(ehdr,addr)[7] == 't') {  
				return 1;
  			}
  			else { 
  				return 0;
  			}
  	} else {
  		return 0;
  	}
}

/******************************************************************************************
* This method recursively looks through all the instructions of the method being searched
******************************************************************************************/
void recursion(Elf64_Ehdr *ehdr,instruction_t *ins, code_t *code_ptr, Elf64_Addr code_addr,
	int isOpen, int isClose) {

  switch(ins->op) {
  	case 0: // MOV_ADDR_TO_REG_OP: VARIABLE
  		//The instruction moves a constant address into a register,
  		//possibly to access a global variable. The ins->addr field
  		//is set to the run-time address, and it’s a reference to a
  		//global variable if that address corresponds to a variable
  		//that is registered as a dynamic symbol. (GLOBAL VARIABLE)
  		// IF protected in between open and close
  		//printf("MOV_ADDR_TO_REG_OP: VARIABLE: %s\n", getVariableName(ehdr,ins->addr));

  		// if protected is not between open and closed
  		if(isProtected(ehdr, ins->addr) && isClose) {
  			replace_with_crash(code_ptr, ins);
  			return;
  		}

  		break;
  	case 1: // JMP_TO_ADDR_OP
  		//The instruction jumps to a constant address,
  		//possibly as a tail call to an global function.
  		//The ins->addr field is set to the destination of the jump
  		//as a run-time address, and it’s a call to a global function
  		//if that address corresponds to a function that is registered
  		//as a dynamic symbol. The instruction after the jump need not
  		//be considered, unless it is reached through some other jump.
  		// (GLOBAL FUNCTION)
  		//printf("JMP_TO_ADDR_OP\n");
  		// This gets the code and instruction of where the jump takes you

  		

  		printf(""); // Code does not compile with out this
  		instruction_t *ins2 = malloc(sizeof(instruction_t));
		Elf64_Shdr * plt_shdr2 = section_by_name(ehdr,".plt");
		code_t *code2 = AT_SEC(ehdr, plt_shdr2) + (ins->addr - plt_shdr2->sh_addr);
		decode(ins2,code2,ins->addr);

		// Jump to the next instruction
		recursion(ehdr,ins2,code2,ins->addr,isOpen,isClose);
		return;

  		break;
  	case 2: // MAYBE_JMP_TO_ADDR_OP
  		//The instruction conditionally jumps to a constant address,
  		//most likely due to an if in the original program. The ins->addr
  		//field is set to the destination of the jump as a run-time address.
  		//This jump is never a call to a function, but the code at the target 
  		//of the jump may go on to call a function or access a variable. If
  		//the jump is not taken, the immediately following instructions might
  		//access a variable or call a function. (IF)

  		//printf("MAYBE_JMP_TO_ADDR_OP\n");
  		// This gets the code and instruction of where the jump takes you
  		printf(""); // Code does not compile with out this
  		instruction_t *ins1 = malloc(sizeof(instruction_t));
		Elf64_Shdr * plt_shdr = section_by_name(ehdr,".plt");
		code_t *code1 = AT_SEC(ehdr, plt_shdr) + (ins->addr - plt_shdr->sh_addr);
		decode(ins1,code1,ins->addr);
		//printf("OP_CODE:%d\n", ins1->op);

		// Jump to the next instruction
		recursion(ehdr,ins1,code1,ins->addr,isOpen,isClose);
		// Continue from where you left off like if you never jumped, your old stuff was saved

  		break;
  	case 3: // CALL_OP
  		//The instruction calls a function. The ins->addr field is set to
  		//the destination of the call as a run-time address, and it’s a call
  		//to a global function if that address corresponds to a function that
  		//is registered as a dynamic symbol. (FUNCTION CALL)
  		
  		//printf("CALL_OP: %s\n", getMethodName(ehdr,ins->addr));
  		// if closed and call to open it
  		if(isOpenFunction(ehdr,ins->addr) && !isOpen) {
  			isOpen = 1;
  			isClose = 0;
  		}

  		// Fail, open it when its open
  		else if(isOpenFunction(ehdr,ins->addr) && isOpen) {
  			replace_with_crash(code_ptr, ins);
  			return;
  		}

  		// if open close it
  		else if(isCloseFunction(ehdr,ins->addr) && isOpen) {
  			isClose = 1;
  			isOpen = 0;
  		}

  		// Fail, close it when already closed
  		 else if(isCloseFunction(ehdr,ins->addr) && isClose) {
  			replace_with_crash(code_ptr, ins);
  			return;
  		}

  		break;
  	case 4: // RET_OP
  		//The instruction returns from the current function, so the
  		//instructions after the return need not be considered, unless they
  		//are reached through an earlier jump. The ins->addr field is not set.

  		//printf("RET_OP\n");
  		// Did not close before you were done
  		if(isOpen) {
  			replace_with_crash(code_ptr, ins);
  			return;
  		}
  		
  		break;
  	case 5: // OTHER_OP
  		//The instruction is a move, arithmetic operation, or comparison
  		//operation that involves only registers and non-address constants.
  		//Your enforce can assume that these operations do not somehow synthesize
  		//the address of a global variable or function, so it can effectively
  		//ignore them. The ins->addr field is not set.
  		//printf("OTHER_OP\n");
  		// DONE NOTHING TO DO HERE
  		break;
  	default :
  		//YOU FUCKED UP !!!
  		printf("THIS SUCKS!!!!!!!!!!\n");
  }

  // if RET_OP instruction then return because you are done
  if(ins->op == 4) {
    return;
  } 

  // Gets and jump to the next instruction
  else {
    instruction_t *data = malloc(sizeof(instruction_t));	//Make space for next instruction
    decode(data,code_ptr + ins->length,code_addr + ins->length);			//Get next Instruction
    recursion(ehdr,data,code_ptr + ins->length,code_addr + ins->length,isOpen,isClose);	//Recursion call
  }
}

void enforce(Elf64_Ehdr *ehdr) {

  //Checking the Methods called
  Elf64_Shdr *dynsym_shdr = section_by_name(ehdr, ".dynsym");
  Elf64_Sym *syms = AT_SEC(ehdr, dynsym_shdr);
  int i, count = dynsym_shdr->sh_size / sizeof(Elf64_Sym);
  for (i = 0; i < count; i++) {
    int isFunction = ELF64_ST_TYPE(syms[i].st_info) == STT_FUNC;
    int size = syms[i].st_size;

    // if it's a method you care about check that method
    if (isFunction && size != 0) {
      Elf64_Shdr *shdr = section_by_index(ehdr, syms[i].st_shndx);
      void *code = AT_SEC(ehdr,shdr) + (syms[i].st_value - shdr->sh_addr);
      instruction_t *data = malloc(sizeof(instruction_t));
      Elf64_Addr addr = syms[i].st_value;
      decode(data,code,addr);
      recursion(ehdr,data,code,addr,0,1);
    }
  }
}

static void fail(char *reason, int err_code) {
  fprintf(stderr, "%s (%d)\n", reason, err_code);
  exit(1);
}

static void check_for_shared_object(Elf64_Ehdr *ehdr) {
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

int main(int argc, char **argv) {
  int fd_in, fd_out;
  size_t len;
  void *p_in, *p_out;
  Elf64_Ehdr *ehdr;

  if (argc != 3)
    fail("expected two file names on the command line", 0);

  /* Open the shared-library input file */
  fd_in = open(argv[1], O_RDONLY);
  if (fd_in == -1)
    fail("could not open input file", errno);

  /* Find out how big the input file is: */
  len = lseek(fd_in, 0, SEEK_END);

  /* Map the input file into memory: */
  p_in = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd_in, 0);
  if (p_in == (void*)-1)
    fail("mmap input failed", errno);

  /* Since the ELF file starts with an ELF header, the in-memory image
     can be cast to a `Elf64_Ehdr *` to inspect it: */
  ehdr = (Elf64_Ehdr *)p_in;

  /* Check that we have the right kind of file: */
  check_for_shared_object(ehdr);

  /* Open the shared-library output file and set its size: */
  fd_out = open(argv[2], O_RDWR | O_CREAT, 0777);
  if (fd_out == -1)
    fail("could not open output file", errno);
  if (ftruncate(fd_out, len))
    fail("could not set output file file", errno);

  /* Map the output file into memory: */
  p_out = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd_out, 0);
  if (p_out == (void*)-1)
    fail("mmap output failed", errno);

  /* Copy input file to output file: */
  memcpy(p_out, p_in, len);

  /* Close input */
  if (munmap(p_in, len))
    fail("munmap input failed", errno);    
  if (close(fd_in))
    fail("close input failed", errno);

  ehdr = (Elf64_Ehdr *)p_out;

  enforce(ehdr);

  if (munmap(p_out, len))
    fail("munmap input failed", errno);    
  if (close(fd_out))
    fail("close input failed", errno);

  return 0;
}