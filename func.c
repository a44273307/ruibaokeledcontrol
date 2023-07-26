#include <stdio.h>
void* memcpy(void* destination, const void* source, size_t num) 
{
size_t i;   
	char* dest = (char*)destination;
    const char* src = (const char*)source;

    for ( i = 0; i < num; i++) {
        dest[i] = src[i];
    }

    return destination;
}