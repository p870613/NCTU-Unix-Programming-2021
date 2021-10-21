#ifndef _LOAD_H
#define _LOAD_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#define ELF_ENTRY_POINT_OFFSET 0X18
#define ELF_ENTRY_POINT_SIZE 0X08
#define E_TYPE_OFFSET 0X10
#define E_TYPE_SIZE 0X02
#define E_PHNUM_OFFSET 0X38
#define E_PHNUM_SIZE 0X02
#define E_PHOFFSET_OFFSET 0X20
#define E_PHOFFSET_SIZE 0X08
#define E_PHSIZE_SIZE 0X02
#define E_PHSIZE_OFFSET 0X36
#define E_SHNUM_OFFSET 0X3C
#define E_SHNUM_SIZE 0X02
#define E_SHOFFSET_OFFSET 0X28
#define E_SHOFFSET_SIZE 0X08
#define E_SHSIZE_SIZE 0X02
#define E_SHSIZE_OFFSET 0X3A
#define E_SHSTRNDX_OFFSET 0X3E
#define E_SHSTRNDX_SIZE 0X02

#define SH_NAME_OFFSET 0X00
#define SH_NAME_SIZE 0x04
#define SH_OFFSET_OFFSET 0X18
#define SH_OFFSET_SIZE 0x08
#define SH_SIZE_OFFSET 0X20
#define SH_SIZE_SIZE 0x08
#define SH_ADDRESS_OFFSET 0X10
#define SH_ADDRESS_SIZE 0X08
#include "runcmd.h"
int load_program(int argc, char argv[][100]);
#endif
