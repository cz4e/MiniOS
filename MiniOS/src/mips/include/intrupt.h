#ifndef _MIPS_INTRUPT_H_
#define _MIPS_INTRUPT_H_

#define     PC_CMOS_REG         ((void *)0x0070)
#define     PC_CMOS_DATA        ((void *)0x0071)
#define     PC_8259_RS_M        ((void *)0x0020)
#define PC_8259_MASK_M              ((void *)0x0021)
#define PC_8259_RS_S                ((void *)0x00A0)
#define PC_8259_MASK_S              ((void *)0x00A1)
/*  DMA端口         */

/*  键盘、鼠标端口  */
#define PC_KB_DATA                  ((void *)0x0060)
#define PC_KB_STAT_CMD              ((void *)0x0064)
/*  硬盘系统端口    */
#define PC_ATA_CONTROLOR1           ((void *)0x01F0)
#define PC_ATA_CTRL1_REG            ((void *)0x03F6)
#define PC_ATA_CONTROLOR2           ((void *)0x0170)
#define PC_ATA_CTRL2_REG            ((void *)0x0376)

/*  VGA系统端口     */
#define PC_CON_REG_IDX              ((void *)0x03D4)
#define PC_CON_REG_DATA             ((void *)0x03D5)

/*  串行口端口      */
#define PC_UART0                    ((void *)0x03F8)
#define PC_UART1                    ((void *)0x02F8)
#define PC_UART2                    ((void *)0x03E8)
#define PC_UART3                    ((void *)0x03F8)

/*  PCI端口         */
#define PC_CONFIG_ADDRESS           ((void *)0x0CF8)
#define PC_DATA_ADDRESS             ((void *)0x0CFC)

/*  中断分配        */
#define IRQ_BASE                    0x20
#define IRQ_CLOCK                   (IRQ_BASE       )
#define IRQ_KEYBOARD                (IRQ_BASE + 1   )
#define IRQ_COM2                    (IRQ_BASE + 3   )
#define IRQ_COM1                    (IRQ_BASE + 4   )
#define IRQ_IDE_MASTER              (IRQ_BASE + 14  )
#define IRQ_IDE_SLAVRY              (IRQ_BASE + 15  )


#endif