// The Tofita Kernel
// Copyright (C) 2020-2023 Oleh Petrenko
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <stdint.h>
#include <stddef.h>

#ifdef bit64
	#define USER32_DLL __declspec(dllexport)
#else
	#define USER32_DLL __declspec(dllexport) __stdcall
#endif

#include "../ntdll.dll/ntdll.hpp"

#define HEXA_NO_DEFAULT_INCLUDES
#define HEXA_MAIN mainHexa
#ifdef bit64
	#define KERNEL32_DLL __declspec(dllimport)
#else
	#define KERNEL32_DLL __declspec(dllimport) __stdcall
#endif
KERNEL32_DLL void *HeapAlloc(uint32_t heap, uint32_t flags, size_t bytes) asm("HeapAlloc");
#define HEXA_NEW(z) HeapAlloc(0,0,z)
#define strlen(z) 0
void *tmemcpy(void *dest, const void *src, uint64_t count) {
	uint8_t *dst8 = (uint8_t *)dest;
	const uint8_t *src8 = (const uint8_t *)src;

	while (count--) {
		*dst8++ = *src8++;
	}

	return dest;
}
#define memcpy(z,u,x) tmemcpy(z,u,x)

#define wprintf(z,...) {}
#define HEAP_ZERO_MEMORY 0
#define fflush(z) {}
#define HEXA_UNREACHABLE(z) {}
#define stdout ((void*)0)
#define GetProcessHeap() 0

int64_t _fltused = 0;

#ifdef bit64
#else
int64_t __alldiv() asm("__alldiv");
int64_t __alldiv() { return 0; } // TODO

int64_t __allrem() asm("__allrem");
int64_t __allrem() { return 0; } // TODO

uint64_t __aulldiv() asm("__aulldiv");
uint64_t __aulldiv() { return 0; } // TODO

uint64_t __aullrem() asm("__aullrem");
uint64_t __aullrem() { return 0; } // TODO

void _memset() asm("_memset");
void _memset() { } // TODO TODO TODO

void _memcpy() asm("_memcpy");
void _memcpy() { } // TODO TODO TODO
#endif

#ifdef bit64
	#include "user32.64.c"
#else
	#include "user32.32.c"
#endif

#ifdef bit64
// TODO why separate fun? same arguments?
__attribute__((fastcall)) void _DllMainCRTStartup() {
#else
__attribute__((stdcall)) void _DllMainCRTStartup(void *, void *, void *) {
#endif
	HEXA_MAIN(0, nullptr);
}
