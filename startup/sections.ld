/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Default linker script for Cortex-M (it includes specifics for 
 * STM32F[34]xx).
 * 
 * To make use of the multi-region initialisations, define
 * OS_INCLUDE_STARTUP_INIT_MULTIPLE_RAM_SECTIONS for the _startup.c file.
 */
MEMORY
{
	FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 64K
	RAM (xrw) : ORIGIN = 0x20000000, LENGTH = 20K
/*
	CCMRAM (xrw) : ORIGIN = 0x00000000, LENGTH = 0
	FLASHB1 (rx) : ORIGIN = 0x00000000, LENGTH = 0
	EXTMEMB0 (rx) : ORIGIN = 0x00000000, LENGTH = 0
	EXTMEMB1 (rx) : ORIGIN = 0x00000000, LENGTH = 0
	EXTMEMB2 (rx) : ORIGIN = 0x00000000, LENGTH = 0
	EXTMEMB3 (rx) : ORIGIN = 0x00000000, LENGTH = 0
*/
	MEMORY_ARRAY (xrw)  : ORIGIN = 0x00000000, LENGTH = 0
}

/*
 * The '__stack' definition is required by crt0, do not remove it.
 */
__stack = ORIGIN(RAM) + LENGTH(RAM);

_estack = __stack; 	/* STM specific definition */

/*
 * Default stack sizes.
 * These are used by the startup in order to allocate stacks 
 * for the different modes.
 */

__Main_Stack_Size = 1024 ;

PROVIDE ( _Main_Stack_Size = __Main_Stack_Size ) ;

__Main_Stack_Limit = __stack  - __Main_Stack_Size ;

/* "PROVIDE" allows to easily override these values from an 
 * object file or the command line. */
PROVIDE ( _Main_Stack_Limit = __Main_Stack_Limit ) ;

/*
 * There will be a link error if there is not this amount of 
 * RAM free at the end. 
 */
_Minimum_Stack_Size = 256 ;

/*
 * Default heap definitions.
 * The heap start immediately after the last statically allocated 
 * .sbss/.noinit section, and extends up to the main stack limit.
 */
PROVIDE ( _Heap_Begin = _end_noinit ) ;
PROVIDE ( _Heap_Limit = __stack - __Main_Stack_Size ) ;

/* 
 * The entry point is informative, for debuggers and simulators,
 * since the Cortex-M vector points to it anyway.
 */
ENTRY(_start)


/* Sections Definitions */

SECTIONS
{
	.text :
	{
		KEEP(*(.entry))            /* Startup code */
		*(.text)                   /* code */
		*(.text.*)                 /* remaining code */
		*(.rodata)                 /* read-only data (constants) */
		*(.rodata.*)
		*(.glue_7)
		*(.glue_7t)
		*(.vfp11_veneer)
		*(.v4_bx)
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} >FLASH

	/* for exception handling/unwind - some Newlib functions (in
	common with C++ and STDC++) use this. */
	.ARM.extab :
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > FLASH

		__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > FLASH
	__exidx_end = .;

	. = ALIGN(4);
	_etext = .;
	/* This is used by the startup in order to initialize the .data secion*/
	_sidata = _etext;

	/* This is the initialized data section
	The program executes knowing that the data is in the RAM
	but the loader puts the initial values in the FLASH (inidata).
	It is one task of the startup to copy the initial values from FLASH to RAM. */
	.data  : AT ( _sidata )
	{
		. = ALIGN(4);
		/* This is used by the startup in order to initialize the .data secion */
		_sdata = . ;

		*(.data)
		*(.data.*)

		. = ALIGN(4);
		/* This is used by the startup in order to initialize the .data secion */
		_edata = . ;
	} >RAM


	/* This is the uninitialized data section */
	.bss :
	{
		. = ALIGN(4);
		/* This is used by the startup in order to initialize the .bss secion */
		_sbss = .;
		__bss_start__ = _sbss;
		*(.bss)
		*(.bss.*)
		*(COMMON)

		. = ALIGN(4);
		/* This is used by the startup in order to initialize the .bss section */
		_ebss = . ;
		__bss_end__ = _ebss;
	} >RAM

	PROVIDE ( end = _ebss );
	PROVIDE ( _end = _ebss );
	PROVIDE ( _exit = _ebss );
	PROVIDE (_stackend = ORIGIN(RAM) + LENGTH(RAM) - _Minimum_Stack_Size);

	/* This is the user stack section
	This is just to check that there is enough RAM left for the User mode stack
	It should generate an error if it's full. */
	._usrstack :
	{
		. = ALIGN(4);
		_susrstack = . ;
		. = . + _Minimum_Stack_Size ;
		. = ALIGN(4);
		_eusrstack = . ;
	} >RAM

	/* after that it's only debugging information. */
	/* remove the debugging information from the standard libraries */
/*
	DISCARD :
	{
		libc.a ( * )
		libm.a ( * )
		libgcc.a ( * )
	}
*/

	/* Stabs debugging sections.  */
	.stab          0 : { *(.stab) }
	.stabstr       0 : { *(.stabstr) }
	.stab.excl     0 : { *(.stab.excl) }
	.stab.exclstr  0 : { *(.stab.exclstr) }
	.stab.index    0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment       0 : { *(.comment) }
	/* DWARF debug sections.
		Symbols in the DWARF debugging sections are relative to the beginning
		of the section so we begin them at 0.  */
    /* DWARF 1 */
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	/* GNU DWARF 1 extensions */
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	/* DWARF 1.1 and DWARF 2 */
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	/* DWARF 2 */
	.debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
	.debug_abbrev   0 : { *(.debug_abbrev) }
	.debug_line     0 : { *(.debug_line) }
	.debug_frame    0 : { *(.debug_frame) }
	.debug_str      0 : { *(.debug_str) }
	.debug_loc      0 : { *(.debug_loc) }
	.debug_macinfo  0 : { *(.debug_macinfo) }
	/* SGI/MIPS DWARF 2 extensions */
	.debug_weaknames 0 : { *(.debug_weaknames) }
	.debug_funcnames 0 : { *(.debug_funcnames) }
	.debug_typenames 0 : { *(.debug_typenames) }
	.debug_varnames  0 : { *(.debug_varnames) }
}
