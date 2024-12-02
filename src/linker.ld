OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
{
    . = 0x100000;
    .text : {*(.text)}
    .data : {*(.data)}
    .bss : {*(.bss)}
    .align4KB : {*(.align4KB)}
    _end = .;
}

