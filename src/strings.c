/* STRINGS.C */
/* Copyright (c) @asdasda3456 2024 - 2024 */

#include <stdint.h>
#include <cpuinfo.h>
#include <osfunc.h>
#include <shell.h>
#include <fs/vfs.h>

#include <stdlib/stdio.h>

int strcmp(const char *first, const char *second)
{
	uint8_t i = 0;
	uint8_t j = 0;

	while (first[i] != '\0') {
		i++;
	}
	while (second[j] != '\0') {
		if (second[j] != first[j]) {
			return 0;
		}
		j++;
	}
	if (i == j) {
		return 1;
	} else {
		return 0;
	}
}

char *strstr(const char *haystack, const char *needle)
{
	if (*needle == '\0') {
		return (char *)haystack;
	}

	for (; *haystack != '\0'; haystack++) {
		const char *h = haystack;
		const char *n = needle;

		while (*h == *n && *n != '\0') {
			h++;
			n++;
		}

		if (*n == '\0') {
			return (char *)haystack;
		}
	}

	return NULL;
}

// 0 is true
// -1 is false

int strncmp(const char* str1,const char* str2, size_t n ){
        int val=0;
	
	if (*str1+n > *str2+n) {
		val = 1;
	}

	else if (*str1+n < *str2+n) {
		val = -1;
	}
	else {
		val = 0;
	}

	return val;
}


char* strncpy(char* dest, const char* src, size_t n)
{
	size_t i;
	for (i = 0; i < n && src[i] != '\0'; i++) {
		dest[i] = src[i];
	}

	for (; i < n; i++) {
		dest[i] = '\0';
	}

	return dest;
}

size_t strlen(const char* str)
{
	size_t length = 0;

	while (str[length] != '\0') {
		length++;
	}

	return length;
}

void strcpy(char* dest, const char* src)
{
	while (*src) {
		*dest++ = *src++;
	}
	*dest = '\0';
}

void strcat(char* dest, const char* src)
{
	while (*dest) dest++;
	while (*src) *dest++ = *src++;
	*dest = '\0';
}

char *strchr(const char *str, int c) {
    while (*str != '\0') {
        if (*str == (char)c) {
            return (char *)str;
        }
        str++;  
    }
    
    if (c == '\0') {
        return (char *)str;
    }

    return NULL;
}

void splitter(const char *tex)
{
	run_term(tex);
}
