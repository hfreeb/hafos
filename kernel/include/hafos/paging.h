#pragma once

void paging_install(void);

void *frame_allocate(void);

void frame_deallocate(void *addr_base);
