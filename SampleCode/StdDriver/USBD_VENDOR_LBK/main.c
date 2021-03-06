/**************************************************************************//**
 * @file     main.c
 * @version  V1.00
 * $Date: 14/10/02 6:51p $
 * @brief    A USB device vendor class sample program. This sample code needs
 *           to test with USBH_VENDOR_LBK.
 *
 * @note
 * Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/
#include <stdio.h>
#include "NUC472_442.h"
#include "vendor_lbk.h"

#define PLLCON_SETTING      CLK_PLLCON_84MHz_HXT
#define PLL_CLOCK           84000000


static uint32_t  g_BulkLbkBuff[BULK_BUFF_SIZE/4];
static uint32_t  g_IsoLbkBuff[ISO_BUFF_SIZE/4];


/*--------------------------------------------------------------------------*/
void SYS_Init(void)
{
    /*---------------------------------------------------------------------------------------------------------*/
    /* Init System Clock                                                                                       */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable External XTAL (4~24 MHz) */
    CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

    /* Waiting for 12MHz clock ready */
    CLK_WaitClockReady( CLK_STATUS_HXTSTB_Msk);

    /* Switch HCLK clock source to HXT */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HXT,CLK_CLKDIV0_HCLK(1));

    /* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware.*/
    CLK->PLLCTL |= CLK_PLLCTL_PD_Msk;

    /* Set PLL frequency */
    CLK->PLLCTL = CLK_PLLCTL_84MHz_HXT;

    /* Waiting for clock ready */
    CLK_WaitClockReady(CLK_STATUS_PLLSTB_Msk);

    /* Switch HCLK clock source to PLL */
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_PLL,CLK_CLKDIV0_HCLK(1));

    /* Enable IP clock */
    CLK_EnableModuleClock(UART0_MODULE);
    CLK_EnableModuleClock(USBD_MODULE);

    /* Select IP clock source */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UARTSEL_HXT, CLK_CLKDIV0_UART(1));

    /* Enable USB PHY */
    SYS->USBPHY = 0x100;  // USB device

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
    SystemCoreClockUpdate();


    /*---------------------------------------------------------------------------------------------------------*/
    /* Init I/O Multi-function                                                                                 */
    /*---------------------------------------------------------------------------------------------------------*/
    /* Set GPG multi-function pins for UART0 RXD and TXD */
    SYS->GPG_MFPL |= SYS_GPG_MFPL_PG1MFP_UART0_RXD | SYS_GPG_MFPL_PG2MFP_UART0_TXD ;

    /* Lock protected registers */
    SYS_LockReg();
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main (void)
{
    /* Init System, IP clock and multi-function I/O
       In the end of SYS_Init() will issue SYS_LockReg()
       to lock protected register. If user want to write
       protected register, please issue SYS_UnlockReg()
       to unlock protected register if necessary */
    SYS_Init();

    /* Init UART to 115200-8n1 for print message */
    UART_Open(UART0, 115200);

    printf("NUC472/NUC442 USB Vendor LBK\n");

    USBD_Open(&gsInfo, NULL, NULL);

    USBD_SetVendorRequest(Vendor_ClassRequest);

    /* Endpoint configuration */
    LBK_Init();

    /* Enable USBD interrupt */
    NVIC_EnableIRQ(USBD_IRQn);

    /* Start transaction */
    USBD_Start();

    while(1)
    {
        if (LBK_BulkInXferDone())
        {
            LBK_BulkInPushData((uint32_t)g_BulkLbkBuff, BULK_BUFF_SIZE);
        }

        if (LBK_IsoInXferDone())
        {
            LBK_IsoInPushData((uint32_t)g_IsoLbkBuff, EPC_MAX_PKT_SIZE);
        }

        if (LBK_HasBulkOutReq())
        {
            LBK_BulkOut((uint32_t)g_BulkLbkBuff, BULK_BUFF_SIZE);
        }

        if (LBK_HasIsoOutReq())
        {
            LBK_IsoOut((uint32_t)g_IsoLbkBuff, EPD_MAX_PKT_SIZE);
        }
    }
}



/*** (C) COPYRIGHT 2014 Nuvoton Technology Corp. ***/

