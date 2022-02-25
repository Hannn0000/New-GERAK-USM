#include <stdio.h>
#include "NUC1xx.h"
#include "Driver\DrvSYS.h"
#include "Driver\DrvGPIO.h"
#include "LCD_Driver.h"
#include "DrvUART.h"

void RFID_SCAN(void);
void CHECK_IN_DIS(void);
void CHECK_OUT_DIS(void);
void WelcomeUSM(void);
void servo_open();
void servo_close();
void LCD_OFF(void);
void BUZZER_PASS(void);
void BUZZER_INVALID(void);
void INVALID_DIS(void);

uint16_t rfid_scan0, rfid_scan1, ir_scan;

int main()
{
	UNLOCKREG(); // Unlock the protected registers
	DrvSYS_SetOscCtrl(E_SYS_XTL12M, 1); //Enable the 12MHz oscillator oscillation
	DrvSYS_Delay(5000); // Waiting for 12M Xtal stable 


	DrvSYS_SelectHCLKSource(0); // HCLK clock source. 0: external 12MHz
	LOCKREG();
	DrvSYS_SetClockDivider(E_SYS_HCLK_DIV, 0);

 	DrvGPIO_InitFunction(E_FUNC_UART0); //Set UART pins

	//LCD Initialize
	Initial_pannel();
	clr_all_pannal();
	
	
//Declaration of Inputs and Outputs
	DrvGPIO_Open(E_GPC,12,E_IO_OUTPUT); 	//LED
	DrvGPIO_Open(E_GPC,13,E_IO_OUTPUT);		//LED
	DrvGPIO_Open(E_GPB,1,E_IO_INPUT); 		//RFID input 0
	DrvGPIO_Open(E_GPB,2,E_IO_INPUT); 		//RFID input 1
	DrvGPIO_Open(E_GPB,3,E_IO_INPUT); 		//IR input
	DrvGPIO_Open(E_GPD,14,E_IO_OUTPUT); 	//LCD Screen Light
	DrvGPIO_Open(E_GPA,14,E_IO_OUTPUT); 	//LCD Words
	DrvGPIO_Open(E_GPC,0,E_IO_OUTPUT);		//Servo
	DrvGPIO_Open(E_GPB,11,E_IO_OUTPUT); 	//Buzzer
	
	while(1)
	{
		ir_scan = DrvGPIO_GetBit (E_GPB, 3);  //IR Input
		DrvGPIO_SetBit(E_GPB,11);
		DrvGPIO_SetBit(E_GPC,12);  						//LED off
		DrvGPIO_SetBit(E_GPC,13);
		DrvGPIO_ClrBit(E_GPA,0);
		
		
		if(ir_scan == 0)
	{
		WelcomeUSM();
	}
		RFID_SCAN();
		DrvSYS_Delay(5000000);
		LCD_OFF();
	}
}

void RFID_SCAN(void)
{
	rfid_scan0 = DrvGPIO_GetBit (E_GPB, 1);
	rfid_scan1 = DrvGPIO_GetBit (E_GPB, 2);
	
	if(rfid_scan0 == 0)
	{
		if(rfid_scan1 == 1)
		{
			BUZZER_PASS();
			CHECK_IN_DIS();
			servo_open();
			DrvSYS_Delay(50000000);
			servo_close();
		}
		
	}
	else 
	{
		if(rfid_scan1 == 0) //check out
		{
			BUZZER_PASS();
			CHECK_OUT_DIS();
			servo_open();
			DrvSYS_Delay(50000000);
			servo_close();
		}
		else
		{
			//DISPLAY
			BUZZER_INVALID();
			INVALID_DIS();
			
			
		}
	}

}


void WelcomeUSM(void)
{
	DrvGPIO_ClrBit(E_GPD,14);						//screen light		
	print_lcd(0,"****************");
	print_lcd(1,"*  WELCOME TO  *");
	print_lcd(2,"*      USM     *");
	print_lcd(3,"****************");
	DrvSYS_Delay(50000000);
	DrvSYS_Delay(50000000);
	print_lcd(0,"****************");
	print_lcd(1,"* PLEASE SCAN  *");
	print_lcd(2,"*  YOUR CARD   *");
	print_lcd(3,"****************");
	DrvSYS_Delay(50000000);
	DrvSYS_Delay(50000000);
	DrvSYS_Delay(50000000);
}

void CHECK_IN_DIS(void)
{
	DrvGPIO_ClrBit(E_GPD,14);
	clr_all_pannal();
	print_lcd(0,"****************");
	print_lcd(1,"*   CHECK IN   *");
	print_lcd(2,"*     ~USM~    *");
	print_lcd(3,"****************");
	DrvSYS_Delay(500000000);
	clr_all_pannal();
}

void CHECK_OUT_DIS(void)
{
	DrvGPIO_ClrBit(E_GPD,14);
	clr_all_pannal();
	print_lcd(0,"****************");
	print_lcd(1,"*   CHECK OUT  *");
	print_lcd(2,"*     ~USM~    *");
	print_lcd(3,"****************");
	DrvSYS_Delay(500000000);
	clr_all_pannal();
}

void INVALID_DIS(void)
{
	DrvGPIO_ClrBit(E_GPD,14);
	clr_all_pannal();
	print_lcd(0,"****************");
	print_lcd(1,"*    INVALID   *");
	print_lcd(2,"*    ACCESS    *");
	print_lcd(3,"****************");
	DrvSYS_Delay(500000000);
	clr_all_pannal();
}

void servo_open(void)
{
	int c;
	for(c=0; c<60; c++)
	{
		DrvGPIO_SetBit(E_GPC,0);
		DrvSYS_Delay(500);
		DrvGPIO_ClrBit(E_GPC,0);
		DrvSYS_Delay(19300);
	}
}

//* Servo Rotate To 90 Degree *//
void servo_close(void)
{
	int c;
	for(c=0; c<60; c++)
	{
		DrvGPIO_SetBit(E_GPC,0);
		DrvSYS_Delay(1500);
		DrvGPIO_ClrBit(E_GPC,0);
		DrvSYS_Delay(18500);
	}
}

void LCD_OFF(void)
{
	DrvGPIO_SetBit(E_GPD,14);
	clr_all_pannal();
}

void BUZZER_PASS(void)
{
	DrvGPIO_ClrBit(E_GPB,11);
	DrvSYS_Delay(15000);
	DrvGPIO_SetBit(E_GPB,11);
}

void BUZZER_INVALID(void)
{
	int i;
	for(i=0; i<2; i++)
	{
		DrvGPIO_ClrBit(E_GPB,11);
		DrvSYS_Delay(15000);
	}
	DrvGPIO_SetBit(E_GPB,11);
}
