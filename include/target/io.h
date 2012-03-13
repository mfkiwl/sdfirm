#ifndef __IO_H_INCLUDE__
#define __IO_H_INCLUDE__

#include <target/config.h>

#define O_RDONLY	0x01
#define O_WRONLY	0x02
#define O_RDWR		0x03
#define O_DIRECT	0x08

typedef void (*io_cb)(void);

#ifdef CONFIG_INPUT_UART
#include <target/uart.h>
#define io_getchar()	uart_getchar()
#endif
#ifdef CONFIG_INPUT_KBD
#include <target/kbd.h>
#define io_getchar()	kbd_getchar()
#endif
#ifdef CONFIG_INPUT_NONE
#define io_getchar()	(0)
#endif

#ifdef CONFIG_OUTPUT_UART_SYNC
#include <target/uart.h>
#define io_putchar(b)	uart_putchar(b)
#endif
#ifdef CONFIG_OUTPUT_LCD
#include <target/lcd.h>
#define io_putchar(b)	lcd_putchar(b)
#endif
#ifdef CONFIG_OUTPUT_NONE
#define io_putchar(b)
#endif

#endif /* __IO_H_INCLUDE__ */

