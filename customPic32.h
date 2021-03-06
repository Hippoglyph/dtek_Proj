
#define PIC32_R(a)		*(volatile unsigned*)(0xBF800000 + (a))

#define PORTF_SET 	PIC32_R(0x86158)// | 0x40;
#define PORTF_CLR 	PIC32_R(0x86154)//  &~0x40;

#define PORTD_SET 	PIC32_R(0x860D8)// | 0x40;
#define PORTD_CLR 	PIC32_R(0x860D4)//  &~0x40;

#define PORTG_SET 	PIC32_R(0x86198)// | 0x40;
#define PORTG_CLR 	PIC32_R(0x86194)//  &~0x40;

#define TRISF_SET	PIC32_R(0x86148)
#define TRISF_CLR	PIC32_R(0x86144)

#define TRISD_SET	PIC32_R(0x860C8)
#define TRISD_CLR	PIC32_R(0x860C4)

#define TRISG_SET	PIC32_R(0x86188)
#define TRISG_CLR	PIC32_R(0x86184)

#define OLED_VDD_DISABLE	(PORTF_SET = 0x40)
#define OLED_VDD_ENABLE		(PORTF_CLR = 0x40)
#define OLED_VBAT_DISABLE 	(PORTF_SET = 0x20)
#define OLED_VBAT_ENABLE 	(PORTF_CLR = 0x20)
#define OLED_SCLK_SET 		(PORTG_SET = 0x40)
#define OLED_SCLK_RESET 	(PORTG_CLR = 0x40)
#define OLED_SDIN_SET 		(PORTG_SET = 0x100)
#define OLED_SDIN_RESET 	(PORTG_CLR = 0x100)
#define OLED_DATA 			(PORTF_SET = 0x10)
#define OLED_COMMAND 		(PORTF_CLR = 0x10)
#define OLED_RESET_ENABLE	(PORTG_CLR = 0x200)
#define OLED_RESET_DISABLE	(PORTG_SET = 0x200)
#define OLED_SPI2BUF		(PIC32_R(0x05A20))
#define OLED_SPI2BUF_FULL	(PIC32_R(0x05A10) & 0x1)
#define OLED_SPI2BUF_EMTY	(PIC32_R(0x05A10) & 0x8)