MEMORY
{
	ROM (rx) 		: ORIGIN = 0x08292000, 	LENGTH = 0x00032000
	RAM (rwx) 		: ORIGIN = 0x001E7000, 	LENGTH = 0x00019000	
	RETSRAM (rwx) 	: ORIGIN = 0x04301F00, 	LENGTH = 256
}

SECTIONS
{
    . = 0x08292000;
	.initdata :
	{ 
		LONG( 0xCAFECAFE ); 				/* APP MAGIC 	*/
		LONG( 0x100 ); 						/* API VERSION	*/
		LONG( 1024 ); 						/* APP STACK 	*/
		LONG((ABSOLUTE(app_entry + 1)));	/* APP ENTRY 	*/

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
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP( *(SORT(.preinit_array.*)) )
        KEEP( *(.preinit_init_array*) )
        PROVIDE_HIDDEN (__preinit_array_end = .);
	
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP( *(SORT(.init_array.*)) )
        KEEP( *(.init_array*) )
        PROVIDE_HIDDEN (__init_array_end = .);
        
/*
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP( *(SORT(.fini_array.*)) )
        KEEP( *(.fini_array*) )
        PROVIDE_HIDDEN (__fini_array_end = .);        
*/

        *( .text .text.* i.* )
        *( .glue_7t )
        *( .glue_7 )
        *( .ctors )
        *( .dtors )
        *( .gnu.linkonce.t.* )
        *( .gnu.linkonce.r.* )
        *( .gcc_except_table )
        *( .rodata .rodata* .constdata* .conststring*  )
        *( .note.gnu.build-id )
        
	} AT > ROM	

	.rodata : ALIGN(4)
	{
		. = ALIGN(4);
	    *(.rodata)
	} AT > ROM

	.ARM.exidx : ALIGN(4)
	{ 
	    __exidx_start = .;
	    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
	    __exidx_end = .;
	} AT > ROM	
	
/***** RAM *****/

	_api_load = LOADADDR(.api);	
	.api : ALIGN(16)
	{
		_api_start = .;		
		*(.api_ram_code.*) 	/* API FUNCTIONS */
		LONG( 0xDEADDEAD ); /* mean API EOF */
		LONG( 0xDEADDEAD );	/* mean API EOF */
		_api_end = .;
	} > RAM  AT > ROM

	_data_load = LOADADDR(.data);	
	.data : ALIGN(4)
	{
		_data_start = .;		
		*(.data.*)
		*(.data)
		_edata = .;
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