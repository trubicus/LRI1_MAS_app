#include "platform.h"
#include "xil_printf.h"
#include "xiicps.h"
#include "camera.h"
#include "xgpio.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define GPIO_EXAMPLE_DEVICE_ID  XPAR_GPIO_0_DEVICE_ID
#define GPIO_CHANNEL 1
#define GPIO_PINOUT 0x00
#define VSYNC_MASK (1 << 10)
#define HREF_MASK (1 << 9)
#define PCLK_MASK (1 << 8)

#define PORT 1234

int main() {
	XGpio Gpio;
	int result;

	init_platform();

	xil_printf("Start Initialize Camera Module\n\r");
	result = InitCamera();
	if (result != XST_SUCCESS) {
		xil_printf("Camera Module Initialization failed\n\r");
		cleanup_platform();
		return (-1);
	}
	xil_printf("Reset Camera Module\n\r");

	xil_printf("Camera Module default register dump\n\r");
	DumpReg();

	//xil_printf("Set RGB565 mode\n\r");
	//InitRGB565();
	xil_printf("Set YUV mode\n\r");
	InitYUV();
	//xil_printf("Set QVGA mode\n\r");
	//InitQVGA();
	xil_printf("Load default Camera registers\n\r");
	//InitDefaultReg();
	InitDefaultRegValue();

	//xil_printf("Camera Test\n");
	//ShowQVGA_ColorBar();

	xil_printf("\n\rCamera Module register dump after initialization\n\r");
	DumpReg();

	result = XGpio_Initialize(&Gpio, GPIO_EXAMPLE_DEVICE_ID);
	if (result != XST_SUCCESS) {
		xil_printf("Gpio Initialization Failed\r\n");
		return XST_FAILURE;
	}

	XGpio_SetDataDirection(&Gpio, GPIO_CHANNEL, ~GPIO_PINOUT);
	int z = 0, rows = 0, r = 0;
	u32 tt, tt1;
	//u8 Y[480];

	//while(CAMERA_isVSYNdown(READ_CAM));
	ll: while (!(XGpio_DiscreteRead(&Gpio, GPIO_CHANNEL) & VSYNC_MASK));
	//while(CAMERA_isVSYNup(READ_CAM));
	while ((XGpio_DiscreteRead(&Gpio, GPIO_CHANNEL) & VSYNC_MASK));

	for (rows = 0; rows < 480; rows++) {  //288
		//Wait line to start
		do {
			tt = XGpio_DiscreteRead(&Gpio, GPIO_CHANNEL);
			//if (CAMERA_isVSYNup(tt)) {
			if ((tt & VSYNC_MASK)) {
				xil_printf("VSYNC error [%d]\n", rows);
				break;
			}
			//} while (CAMERA_isHREFdown(tt));
		} while (!(tt & HREF_MASK));

		//Y[z] =(u8)(tt);
		//z++;
		for (r = 0; r < 1280; r++) { //352
			do {
				tt = XGpio_DiscreteRead(&Gpio, GPIO_CHANNEL);
				//} while (CAMERA_isPCLKdown(tt)); //while(CAMERA_isPCLKup(tt));
			} while (!(tt & PCLK_MASK));
			//---------
			do {
				tt1 = XGpio_DiscreteRead(&Gpio, GPIO_CHANNEL);
				//if (CAMERA_isHREFdown(tt1) && r != 1279) {
				if (!(tt1 & HREF_MASK) && r != 1279) {
					continue;
				}
			} while ((tt1 & PCLK_MASK));

			//Write data
			//HMY[z] =(Xuint8)(tt>>3);
			u8 vani = (u8) (tt);
			xil_printf("%u ", vani);
			z++;
			//}while (CAMERA_isHREFup(READ_CAM));
		}
		while ((XGpio_DiscreteRead(&Gpio, GPIO_CHANNEL) & HREF_MASK))
			;
		xil_printf("\n\rgotov\n\r");
	}

	xil_printf("%d\n\r", z);
	xil_printf("OK !!!\n\r");

	cleanup_platform();
	return 0;
}
