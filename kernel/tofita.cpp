// The Tofita Kernel
// Copyright (C) 2020  Oleg Petrenko
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

extern "C" {

namespace efi {
#include <efi.hpp>
}

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#include "../boot/shared/boot.hpp"

// Avoids PIT-triggered rendering
// This is not a best solution
uint8_t haveToRender = 1;

// Forward for global usage
function quakePrintf(const char8_t *c, ...);

void *memset(void *dest, int32_t e, uint64_t len) {
	uint8_t *d = (uint8_t *)dest;
	for (uint64_t i = 0; i < len; i++, d++) {
		*d = e;
	}
	return dest;
}

void ___chkstk_ms() {};

#include "util/Math.cpp"

#include "../devices/cpu/cpu.hpp"
#include "../devices/cpu/amd64.cpp"
#include "../devices/serial/log.cpp"
#include "allocator.cpp"
#include "../devices/cpu/cpuid.cpp"
#include "../devices/cpu/spinlock.cpp"
#include "../devices/cpu/interrupts.cpp"
#include "../devices/cpu/rdtsc.cpp"
#include "../devices/cpu/pages.cpp"
#include "../devices/cpu/physical.cpp"
#include "../devices/screen/framebuffer.cpp"
#include "../devices/ps2/keyboard.cpp"
#include "../devices/ps2/mouse.cpp"
#include "../devices/ps2/polling.cpp"
#include "../devices/cpu/fallback.cpp"
#include "../devices/acpi/acpi.cpp"
#include "ramdisk.cpp"
#include "formats/exe/exe.cpp"
#include "formats/cur/cur.cpp"
#include "formats/bmp/bmp.cpp"
#include "gui/blur.cpp"
#include "gui/quake.cpp"
#include "gui/text.cpp"
#include "gui/compositor.cpp"

// STB library
#define STBI_NO_SIMD
#define STBI_NO_STDIO
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "formats/stb_image/libc.cpp"
#include "formats/stb_image/stb_image.hpp"
#include "formats/stb_image/unlibc.cpp"

function (*keyDownHandler)(uint8_t) = null;
function handleKeyDown(uint8_t key) {
	if (keyDownHandler) keyDownHandler(key);
}

const KernelParams *paramsCache = null;
function kernelMain(const KernelParams *params) {
	serialPrintln(u8"<Tofita> GreenteaOS " STR(versionMajor) "." STR(versionMinor) " " versionName " kernel loaded and operational");
	serialPrint(u8"<Tofita> CR3 points to: ");
	serialPrintHex((uint64_t) params->pml4);
	serialPrint(u8"\n");
	paramsCache = params;
	PhysicalAllocator::init(&params->efiMemoryMap, params->physicalRamBitMaskVirtual, params->ramBytes / 4096);
	initAllocatorForBuffer(params->bufferSize, params->buffer);
	setFramebuffer(&params->framebuffer);
	setRamDisk(&params->ramdisk);

	if (sizeof(uint8_t*) == 4) serialPrintln(u8"<Tofita> void*: 4 bytes");
	if (sizeof(uint8_t*) == 8) serialPrintln(u8"<Tofita> void*: 8 bytes");

	#ifdef __cplusplus
		serialPrintln(u8"<Tofita> __cplusplus");
	#else
		serialPrintln(u8"<Tofita> !__cplusplus");
	#endif

	#if defined(__clang__)
		serialPrintln(u8"<Tofita> __clang__");
	#elif defined(__GNUC__) || defined(__GNUG__)
		serialPrintln(u8"<Tofita> __GNUC__");
	#elif defined(_MSC_VER)
		serialPrintln(u8"<Tofita> _MSC_VER");
	#endif

	disablePic();
	enableInterrupts();
	enablePS2Mouse();

	initText();
	initializeCompositor();

	quakePrintf(u8"GreenteaOS " STR(versionMajor) "." STR(versionMinor) " " versionName " loaded and operational\n");

	CPUID cpuid = getCPUID();

	uint32_t megs = Math::round((double)params->ramBytes/(1024.0*1024.0));
	quakePrintf(u8"[CPU] %s %s %d MB RAM\n", cpuid.vendorID, cpuid.brandName, megs);

	if (!ACPIParser::parse(params->acpiTablePhysical)) {
		quakePrintf(u8"ACPI is *not* loaded\n");
	} else {
		quakePrintf(u8"ACPI 2.0 is loaded and ready\n");
	}

	quakePrintf(u8"Enter 'help' for commands\n");

	{
		RamDiskAsset a = getRamDiskAsset(u8"hello.bmp");
		Bitmap32* bmp = bmp::loadBmp24(&a);
		setWallpaper(bmp, Center);
	}


	mouseX = _framebuffer->width / 2;
	mouseY = _framebuffer->height / 2;

	while (true) {
		// Poll PS/2 devices
		haveToRender = haveToRender || (pollPS2Devices() == PollingPS2SomethingHappened);

		if (mouseX > _framebuffer->width) mouseX = _framebuffer->width;
		if (mouseY > _framebuffer->height) mouseY = _framebuffer->height;
		if (mouseY < 0) mouseY = 0;
		if (mouseX < 0) mouseX = 0;

		if (haveToRender == 0) continue ;
		haveToRender = 0;

		composite();

		copyToScreen();
	}
}
}
