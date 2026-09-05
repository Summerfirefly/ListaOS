ENTRY(_start)
OUTPUT_FORMAT(binary)

SECTIONS
{
    . = 0x8200;

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    _init_end = .;
}
