/*
-Xlinker --defsym=VAR=?
*/

ROM_BASE = DEFINED(ROM_BASE) ? ROM_BASE : 0x08292000; 
ROM_SIZE = DEFINED(ROM_SIZE) ? ROM_SIZE : 0x00032000; 

RAM_BASE = DEFINED(RAM_BASE) ? RAM_BASE : 0x001E7000; 
RAM_SIZE = DEFINED(RAM_SIZE) ? RAM_SIZE : 0x00019000; 

APP_STACK = DEFINED(APP_STACK) ? APP_STACK : 1024; 
APP_STACK = APP_STACK > 8096   ? APP_STACK : 8096; 
APP_PRIO  = DEFINED(APP_PRIO)  ? APP_PRIO  : 4;

MEMORY
{
	ROM (rx) 		: ORIGIN = ROM_BASE, 	LENGTH = ROM_SIZE
	RAM (rwx) 		: ORIGIN = RAM_BASE, 	LENGTH = RAM_SIZE	
	RETSRAM (rwx) 	: ORIGIN = 0x04301F00, 	LENGTH = 256 /* last */
}

ENTRY(app_entry)

SECTIONS
{
    . = ROM_BASE;
	.initdata :
	{ 
		LONG(0xCAFECAFE); 					/* APP MAGIC 	*/
		LONG(0x100); 						/* API VERSION	*/
		LONG((ABSOLUTE(app_entry + 1)));	/* APP ENTRY 	*/		

		SHORT( APP_STACK ); 				/* APP STACK 	*/
		SHORT( APP_PRIO  ); 				/* APP PRIORITY */

		LONG((ABSOLUTE( _api_load )));
		LONG((ABSOLUTE( _api_start )));
		LONG((ABSOLUTE( _api_end )));

		LONG((ABSOLUTE( _data_load )));
		LONG((ABSOLUTE( _data_start )));
		LONG((ABSOLUTE( _data_end )));

		LONG((ABSOLUTE( _bss_start )));
		LONG((ABSOLUTE( _bss_end )));

	} AT > ROM	
	
	.text : ALIGN(4)
	{ 
        *( .text .text.* i.* )

        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP( *(SORT(.preinit_array.*)) )
        KEEP( *(.preinit_init_array*) )
        PROVIDE_HIDDEN (__preinit_array_end = .);
	
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP( *(SORT(.init_array.*)) )
        KEEP( *(.init_array*) )
        PROVIDE_HIDDEN (__init_array_end = .);
        
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP( *(SORT(.fini_array.*)) )
        KEEP( *(.fini_array*) )
        PROVIDE_HIDDEN (__fini_array_end = .);        

        *(.init)
        *(.fini)

        *crtbegin.o(.ctors)
        *crtbegin?.o(.ctors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
        *(SORT(.ctors.*))
        *(.ctors)

        *crtbegin.o(.dtors)
        *crtbegin?.o(.dtors)
        *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
        *(SORT(.dtors.*))
        *(.dtors)
        
        *( .rodata .rodata* .constdata* .conststring*  )	
	} > ROM	

    .ARM.extab : ALIGN(4)
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > ROM

    .ARM.exidx : ALIGN(4)
    {
    	__exidx_start = .;		
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    	__exidx_end = .;		
    } > ROM

	
	.eeprom (NOLOAD) : ALIGN(1024) /* FLASH PAGE SIZE = TODO */
	{ 
		_eeprom_start = .;
		*(.eeprom) 
		_eeprom_end = .;
	} > ROM


/***** RAM *****/


	_api_load = LOADADDR(.api);	
	.api : ALIGN(16)
	{
		_api_start = .;		
		*(.api_ram_code.*)	/* API FUNCTIONS */
		LONG( 0 ); 			/* EOF must, gcc remove segment ?!? */
		_api_end = .;
	} > RAM  AT > ROM

	_data_load = LOADADDR(.data);	
	.data : ALIGN(4)
	{
		_data_start = .;		
		*(.data.*)
		*(.data)
		_data_end = .;
	} > RAM  AT > ROM


	.bss : ALIGN(4)
	{
		_bss_start = .;
		. = ALIGN(4);
		*(.bss)
		*(.bss.*)
		_bss_end = .;
	} > RAM
	
	.sram (NOLOAD) : { *(.sram) } > RETSRAM

}