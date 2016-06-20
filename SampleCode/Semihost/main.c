/******************************************************************************
 * @file     main.c
 * @version  V1.00
 * $Revision: 5 $
 * $Date: 14/10/06 9:47a $
 * @brief    Show how to print and get character with IDE console window.
 *
 * @note
 * Copyright (C) 2013 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "NUC472_442.h"


/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/




void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    // TODO: Configure system clock


    /* Lock protected registers */
    SYS_LockReg();

}

int main()
{
    int8_t item;

    SYS_Init();

    printf("\n Start SEMIHOST test: \n");

    while(1) {
        item = getchar();
        printf("%c\n",item);
    }

}

/*** (C) COPYRIGHT 2013 Nuvoton Technology Corp. ***/
