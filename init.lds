ENTRY(_start)
OUTPUT_FORMAT(binary)

SECTIONS
{
    . = 0x8200;

    .text : {
        *(.text)
    }

    _init_end = .;
}