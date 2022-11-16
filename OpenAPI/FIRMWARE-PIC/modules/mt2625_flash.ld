/********************************************************  
memory map 
  Start Address	Size	CM4 view Devices
    0x0000_0000	64MB	EMI	
    0x0400_0000	64KB	CM4 TCM	
	0x0401_0000	32KB	CM4 TCM / cache	
	0x0401_8000	32KB	Reserved	
	0x0402_0000	896KB	Reserved	
	0x0410_0000	1MB   	Boot ROM CM4
	0x0420_0000	32KB	SYSRAM
	0x0430_0000	8KB	    RTC Retention SRAM
	0x0440_0000	1MB	    Reserved
	0x0800_0000	128MB	SFC0
 	0x1000_0000	128MB	SW virtual memory management
	0x1800_0000	128MB	SW virtual memory management
*********************************************************/
 
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
 
/* Memory Spaces Definitions:4M FLASH,4M PSRAM,32K SYSRAM,64K TCM, 8K retention SRAM */
MEMORY
{
    ROM_HEAD_1(rx)        : ORIGIN = 0x08000000, LENGTH = 4K
    ROM_HEAD_2(rx)        : ORIGIN = 0x08001000, LENGTH = 4K
    ROM_BL(rx)            : ORIGIN = 0x08002000, LENGTH = 64K
    ROM_RTOS(rx)          : ORIGIN = 0x08012000, LENGTH = 3524K
    ROM_NVDM_RESERVED(rx) : ORIGIN = 0x08383000, LENGTH = 500K
    RAM(rwx)              : ORIGIN = 0x00000000, LENGTH = 2048K 
    RAM_MD (rwx)          : ORIGIN = 0x00200000, LENGTH = 2048K
    VRAM (rwx)            : ORIGIN = 0x10000000, LENGTH = 2048K 
    VRAM_MD (rwx)         : ORIGIN = 0x10200000, LENGTH = 2048K    
    TCM (rwx)             : ORIGIN = 0x04000000, LENGTH = 32K
    TCM_MD (rwx)          : ORIGIN = 0x04008000, LENGTH = 32K
    SYSRAM (rwx)          : ORIGIN = 0x04200000, LENGTH = 32K
    RETSRAM (rwx)         : ORIGIN = 0x04300000, LENGTH = 2K
    RETSRAM_MD (rwx)      : ORIGIN = 0x04300800, LENGTH = 6K
}
 
 /* Highest address of the stack */
_stack_end = ORIGIN(TCM) + LENGTH(TCM);    /* end of TCM */

/* Generate a link error if stack don't fit into TCM */
_stack_size = 0x1000; /* required amount of stack 4KB*/
 
/* stack start */
_stack_start = _stack_end - _stack_size;
 


/* Entry Point */
ENTRY(Reset_Handler)

SECTIONS
{
    . = ORIGIN(ROM_RTOS);
    
    /*AP PART*/
    .text :
    {
        _text_start = .;
        Image$$TEXT$$Base = .;
        
        KEEP(*(.reset_handler))
        KEEP(*(.init))
        KEEP(*(.fini))
        
        *(.text)
        *(.text*)
        *(.rodata)
        *(.rodata*)
        
        . = ALIGN(4);
        Image$$TEXT$$Limit = .;
        _text_end = .;
        
    } > ROM_RTOS  AT> ROM_RTOS
   

    . = ALIGN(32);
    _ram_code_load = LOADADDR(.cached_ram_text);

    .cached_ram_text :
    {
        Image$$RAM_TEXT$$Base = .;
        _ram_code_start = .;

        *(.ram_code)
        *(.ram_rodata)
        
        
        Image$$RAM_TEXT$$Limit = .;
        _ram_code_end = .;
    } > VRAM   AT> ROM_RTOS
  

    . = ALIGN(4);
    _data_load = LOADADDR(.cached_ram_data);

     .cached_ram_data :
    {       
        _data_start = .;
        Image$$CACHED_DATA$$RW$$Base = .;
        
        
        *(.data)
        *(.data*)
        
        . = ALIGN(4);
        /* preinit data */
        PROVIDE (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE (__init_array_end = .);
        
        . = ALIGN(4);
        /* finit data */
        PROVIDE (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE (__fini_array_end = .);
        
        _data_end = .;       
       Image$$CACHED_DATA$$RW$$Limit = .;
    
        
    } > VRAM  AT> ROM_RTOS
    
    
    . = ALIGN(4);
    .bss (NOLOAD) : 
    {
          _bss_start = .;
        Image$$CACHED_DATA$$ZI$$Base = .;
        
        *(.bss)
        *(.bss*)
        *(COMMON)
        
        . = ALIGN(4);
        _bss_end = .;
        *(.ram_non_init_data)
        . = ALIGN(4);
        Image$$CACHED_DATA$$ZI$$Limit = .;
        
    } > VRAM  AT> ROM_RTOS
    
     
       . = ALIGN(32);
     _ram_noncached_rw_load = LOADADDR(.noncached_ram_data);
     . = . - ORIGIN(VRAM);

    .noncached_ram_data . :
    {
        Image$$NONCACHED_DATA$$Base = .;
        _ram_noncached_rw_start = .;
        
        *(.noncached_rwdata)
        
        
        Image$$NONCACHED_DATA$$Limit = .;
        _ram_noncached_rw_end = .; 
    } > RAM  AT> ROM_RTOS
     
    . = ALIGN(4);
    .noncached_ram_bss . (NOLOAD) :
    {                
        Image$$NONCACHED_ZI$$Base = .;
        _ram_noncached_zi_start = .;
        
        *(.noncached_zidata)
        
        _ram_noncached_zi_end = .;
        *(.noncached_non_init_data)
        . = ALIGN(4);
        Image$$NONCACHED_ZI$$Limit = .;
       
    } > RAM  AT> ROM_RTOS
    
    ASSERT(_ram_noncached_zi_end <= (ORIGIN(RAM) + LENGTH(RAM)),"PSRAM expired")
    
    . = ALIGN(4);
      _sysram_data_load = LOADADDR(.sysram_data);

     .sysram_data  :
    {       
        _sysram_data_start = .;
        Image$$SYSRAM_DATA$$RW$$Base = .;
        *(.sysram_rodata)
        *(.sysram_rwdata)
        _sysram_data_end = .;          
       Image$$SYSRAM_DATA$$RW$$Limit = .;
    
        
    } > SYSRAM  AT> ROM_RTOS
    

    . = ALIGN(4);
    .sysram_bss (NOLOAD) : 
    {
        _sysram_bss_start = .;
        Image$$SYSRAM_DATA$$ZI$$Base = .;
        
       *(.sysram_zidata)
        
        . = ALIGN(4);
        _sysram_bss_end = .;
        Image$$SYSRAM_DATA$$ZI$$Limit = .;
        
    } > SYSRAM  AT> ROM_RTOS
    
    ASSERT(_sysram_bss_end <= (ORIGIN(SYSRAM) + LENGTH(SYSRAM)),"AP SYSRAM expired")

       . = ALIGN(4);
      _retsram_data_load = LOADADDR(.retsram_data);

     .retsram_data  :
    {       
        _retsram_data_start = .;
        Image$$RETSRAM_DATA$$RW$$Base = .;
        *(.retsram_rodata)
        *(.retsram_rwdata)
        _retsram_data_end = .;          
       Image$$RETSRAM_DATA$$RW$$Limit = .;
    
        
    } > RETSRAM  AT> ROM_RTOS
    

    . = ALIGN(4);
    .retsram_bss (NOLOAD) : 
    {
        _retsram_bss_start = .;
        Image$$RETSRAM_DATA$$ZI$$Base = .;
        
       *(.retsram_zidata)
        
        . = ALIGN(4);
        _retsram_bss_end = .;
        Image$$RETSRAM_DATA$$ZI$$Limit = .;
        
    } > RETSRAM  AT> ROM_RTOS
    
    ASSERT(_retsram_bss_end <= (ORIGIN(RETSRAM) + LENGTH(RETSRAM)),"AP retsram expired")
  
   . = ALIGN(4);
    _tcm_text_load = LOADADDR(.tcm);

    .tcm :
    {
        
        _tcm_text_start = .;
        Image$$TCM$$RO$$Base = .;
        Image$$VECTOR$$TABLE$$Base = .;
         KEEP(*(.isr_vector))
        *(.tcm_code)
        *(.tcm_rodata)
        Image$$TCM$$RO$$Limit = .;
        Image$$TCM$$RW$$Base = .;
        
        *(.tcm_rwdata)
        . = ALIGN(4);
        Image$$TCM$$RW$$Limit = .;
        _tcm_text_end = .;
    }> TCM  AT> ROM_RTOS

    . = ALIGN(4);
    .tcm_bss (NOLOAD) :
    {
        _tcm_zi_start = .;
        Image$$TCM$$ZI$$Base = .;
        
        *(.tcm_zidata)
        
        _tcm_zi_end = .;
        Image$$TCM$$ZI$$Limit = .;
        
        
    }> TCM AT> ROM_RTOS  
    
    /* use to check if the stack exceeds the total TCM size*/
    .stack :
    {
        . = ALIGN(4);
        
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _stack_size;
        . = ALIGN(4);
        
    } > TCM

   /*MD PART*/

    .md_text :
    {
        _md_text_start = .;
        Image$$MD_TEXT$$Base = .;
       
        *(.md_text)
        *(.md_rodata)
        
        . = ALIGN(4);
        Image$$MD_TEXT$$Limit = .;
        _md_text_end = .;
        
    } > ROM_RTOS  AT> ROM_RTOS

   . = ALIGN(32);
    _md_ram_code_load = LOADADDR(.md_cached_ram_text);

    .md_cached_ram_text :
    {
        Image$$MD_CACHED_RAM_TEXT$$Base = .;
        _md_ram_code_start = .;

        *(.md_ram_code)
        *(.md_ram_rodata)
        
        
        Image$$MD_CACHED_RAM_TEXT$$Limit = .;
        _md_ram_code_end = .;
    } > VRAM_MD   AT> ROM_RTOS
    
    . = ALIGN(4);
    .gki_stack (NOLOAD) :
    {
          _gki_start = .;
        Image$$GKI_STACK$$Base = .;

        *(.gki_stack*)

        . = ALIGN(4);
        _gki_end = .;
        Image$$GKI_STACK$$Limit = .;

    } > VRAM_MD  AT> ROM_RTOS

    . = ALIGN(4);
    _md_data_load = LOADADDR(.md_cached_ram_data);

     .md_cached_ram_data :
    {       
        _md_data_start = .;
        Image$$MD_CACHED_RAM_DATA$$RW$$Base = .;

         *(.md_ram_rwdata)
          
        _md_data_end = .;       
       Image$$MD_CACHED_RAM_DATA$$RW$$Limit = .;
    
        
    } > VRAM_MD  AT> ROM_RTOS
    
    
    . = ALIGN(4);
    .md_cached_ram_bss (NOLOAD) : 
    {
          _md_bss_start = .;
        Image$$MD_CACHED_RAM_DATA$$ZI$$Base = .;
        *(.md_ram_zidata)
        
        . = ALIGN(4);
        _md_bss_end = .;
        *(.md_ram_non_init_data)
        . = ALIGN(4);
        Image$$MD_CACHED_RAM_DATA$$ZI$$Limit = .;
        
    } > VRAM_MD  AT> ROM_RTOS
    
     
       . = ALIGN(32);
     _md_ram_noncached_rw_load = LOADADDR(.md_noncached_ram_data);
     . = . - ORIGIN(VRAM_MD);

    .md_noncached_ram_data . :
    {
        Image$$MD_NONCACHED_RAM_DATA$$Base = .;
        _md_ram_noncached_rw_start = .;
        
        *(.md_noncached_ram_rwdata)
           
        Image$$MD_NONCACHED_RAM_DATA$$Limit = .;
        _md_ram_noncached_rw_end = .; 
    } > RAM_MD  AT> ROM_RTOS
     
    . = ALIGN(4);
    .md_noncached_ram_bss . (NOLOAD) :
    {                
        Image$$MD_NONCACHED_RAM_ZI$$Base = .;
        _md_ram_noncached_zi_start = .;
        
        *(.md_noncached_ram_zidata)
        
        _md_ram_noncached_zi_end = .;
        *(.md_noncached_ram_non_init_data)
        . = ALIGN(4);
        Image$$MD_NONCACHED_RAM_ZI$$Limit = .;
       
    } > RAM_MD  AT> ROM_RTOS
    
    ASSERT(_ram_noncached_zi_end <= (ORIGIN(RAM_MD) + LENGTH(RAM_MD)),"MD PSRAM expired")

     . = ALIGN(4);
      _md_retsram_data_load = LOADADDR(.md_retsram_data);

     .md_retsram_data  :
    {       
        _md_retsram_data_start = .;
        Image$$MD_RETSRAM_DATA$$RW$$Base = .;
        *(.md_retsram_rodata)
        *(.md_retsram_rwdata)
        _md_retsram_data_end = .;          
       Image$$MD_RETSRAM_DATA$$RW$$Limit = .;
    
        
    } > RETSRAM_MD  AT> ROM_RTOS
    

    . = ALIGN(4);
    .md_retsram_bss (NOLOAD) : 
    {
        _md_retsram_bss_start = .;
        Image$$MD_RETSRAM_DATA$$ZI$$Base = .;
        
       *(.md_retsram_zidata)
        
        . = ALIGN(4);
        _md_retsram_bss_end = .;
        Image$$MD_RETSRAM_DATA$$ZI$$Limit = .;
        
    } > RETSRAM_MD  AT> ROM_RTOS
    
    ASSERT(_md_retsram_bss_end <= (ORIGIN(RETSRAM_MD) + LENGTH(RETSRAM_MD)),"MD retsram expired")

    . = ALIGN(4);
    _md_tcm_text_load = LOADADDR(.md_tcm);

    .md_tcm :
    {
        
        _md_tcm_text_start = .;
        Image$$MD_TCM$$RO$$Base = .;
        *(.md_tcm_code)
        *(.md_tcm_rodata)
        Image$$MD_TCM$$RO$$Limit = .;
        Image$$MD_TCM$$RW$$Base = .;  
        *(.md_tcm_rwdata)
        . = ALIGN(4);
        Image$$MD_TCM$$RW$$Limit = .;
        _md_tcm_text_end = .;
    }> TCM_MD  AT> ROM_RTOS

    . = ALIGN(4);
    .md_tcm_bss (NOLOAD) :
    {
        _md_tcm_zi_start = .;
        Image$$MD_TCM$$ZI$$Base = .;
        
        *(.md_tcm_zidata)
        
        _md_tcm_zi_end = .;
        Image$$MD_TCM$$ZI$$Limit = .;
        
        
    }> TCM_MD AT> ROM_RTOS

    ASSERT(_md_tcm_zi_end <= (ORIGIN(TCM_MD) + LENGTH(TCM_MD)),"MD TCM_MD expired")

    /*linker symbol*/
    Image$$STACK$$ZI$$Base = _stack_end - _stack_size;
    Image$$STACK$$ZI$$Limit = _stack_end;
    
    /* provide nvdm start and length for NVDM management */
    Image$$NVDM$$ZI$$Base = ORIGIN(ROM_NVDM_RESERVED);
    Image$$NVDM$$ZI$$Length = LENGTH(ROM_NVDM_RESERVED);
    
    
}
