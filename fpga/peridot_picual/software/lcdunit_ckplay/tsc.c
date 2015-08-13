#include <system.h>
#include <altera_avalon_spi.h>
#include "tsc.h"
#include "nd_lib/nd_egl.h"

void tsc_SensorTest()
{
	int x, y, z1, z2, p, a, t;
	int i;

	for (i = 480; i > 0; --i) {
		tsc_GetSensors(&x, &y, &z1, &z2, &a, &t);

		if (z1 > (z2 / 8)) {
			x = (x * 240 / 2048);
			y = (y * 320 / 2048);
			nd_GsEglColor(0xffff, 0x0000, 256);
			nd_GsEglCircle(x, y, 3);
		}

		z1 = z1 * 300 / 4096;
		nd_GsEglColor(0xf800, 0x0000, 256);
		nd_GsEglBoxfill(1, 0, 8, z1);
		nd_GsEglColor(0x0000, 0x0000, 256);
		nd_GsEglBoxfill(1, z1, 8, 299);

		z2 = z2 * 300 / 4096;
		nd_GsEglColor(0x07e0, 0x0000, 256);
		nd_GsEglBoxfill(11, 0, 18, z2);
		nd_GsEglColor(0x0000, 0x0000, 256);
		nd_GsEglBoxfill(11, z2, 18, 299);

		a = a * 300 / 4096;
		nd_GsEglColor(0x001f, 0x0000, 256);
		nd_GsEglBoxfill(221, 0, 228, a);
		nd_GsEglColor(0x0000, 0x0000, 256);
		nd_GsEglBoxfill(221, a, 228, 299);

		t = t * 300 / 4096;
		nd_GsEglColor(0xf81f, 0x0000, 256);
		nd_GsEglBoxfill(231, 0, 238, t);
		nd_GsEglColor(0x0000, 0x0000, 256);
		nd_GsEglBoxfill(231, t, 238, 299);

		/*
		p = x / 2048 * z2 / z1 * 1024;
		nd_GsEglColor(0xffe0, 0x0000, 256);
		nd_GsEglBoxfill(0, 301, p, 309);
		nd_GsEglColor(0x0000, 0x0000, 256);
		nd_GsEglBoxfill(p, 301, 238, 309);
		//-*/

		nd_GsEglColor(0x07ff, 0x0000, 256);
		nd_GsEglBoxfill(i / 2, 311, 238, 319);

		nd_GsEglPage((nd_u32)nd_GsEglDrawBuffer, (nd_u32)nd_GsEglDrawBuffer, 0);
	}
}

void tsc_GetSensors(int *x, int *y, int *z1, int *z2, int *aux, int *temp)
{
	alt_u8 bytes[2];
	if(x) {
		alt_avalon_spi_command(TOUCHPANEL_BASE, 0, 1, (const alt_u8 *)"\xd0", 2, bytes, 0);
		*x = (bytes[0] << 4) | (bytes[1] >> 4);
	}
	if(y) {
		alt_avalon_spi_command(TOUCHPANEL_BASE, 0, 1, (const alt_u8 *)"\x90", 2, bytes, 0);
		*y = (bytes[0] << 4) | (bytes[1] >> 4);
	}
	if(z1) {
		alt_avalon_spi_command(TOUCHPANEL_BASE, 0, 1, (const alt_u8 *)"\xb0", 2, bytes, 0);
		*z1 = (bytes[0] << 4) | (bytes[1] >> 4);
	}
	if(z2) {
		alt_avalon_spi_command(TOUCHPANEL_BASE, 0, 1, (const alt_u8 *)"\xc0", 2, bytes, 0);
		*z2 = (bytes[0] << 4) | (bytes[1] >> 4);
	}
	if(aux) {
		alt_avalon_spi_command(TOUCHPANEL_BASE, 0, 1, (const alt_u8 *)"\xe4", 2, bytes, 0);
		*aux = (bytes[0] << 4) | (bytes[1] >> 4);
	}
	if(temp) {
		alt_avalon_spi_command(TOUCHPANEL_BASE, 0, 1, (const alt_u8 *)"\x84", 2, bytes, 0);
		*temp = (bytes[0] << 4) | (bytes[1] >> 4);
	}
}

