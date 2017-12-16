.set MAGIC_NUMBER,     0x1BADB002
.set FLAG_PAGE_ALIGN,  1 << 0 //Aligns boot modules on 4KiB page boundaries
.set FLAG_MEMORY_INFO, 1 << 1 //Boot loader passes memory info 
.set FLAG_GRAPHICS,    1 << 2 //Boot loader passes video table info
.set FLAGS,            FLAG_PAGE_ALIGN
.set CHECKSUM,         -(MAGIC_NUMBER + FLAGS)

.section .multiboot_header

.long MAGIC_NUMBER
.long FLAGS
.long CHECKSUM
