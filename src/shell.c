#include "shell.h"
#include "cmd.h"
#include "string.h"
#include "uart.h"

void run_shell()
{
    while (1) {
        char buf[SHELL_BUF_SIZE];
        uart_puts("# ");
        read_user_input(buf);
        exec_command(buf);
    }
}

void read_user_input(char *buf)
{
    int idx = 0;

    while (1) {
        char c = uart_getc();

        if (c == '\n') {
            uart_putc(c);
            buf[idx] = '\0';
            break;
        } else if (c >= 32 && c <= 126) {
            uart_putc(c);
            buf[idx++] = c;
        } else if (c == 127) {
            if (idx > 0) {
                buf[idx--] = 0;
                uart_putc('\b');
                uart_putc(' ');
                uart_putc('\b');
            }
        } else {
            continue;
        }
    }
}

int exec_command(const char *command)
{
    int i = 0;
    while (1) {
        if (!strcmp(commands[i].name, "NULL")) {
            uart_puts("Command not found\n");
            return -1;
        } else if (!strcmp(commands[i].name, command)) {
            commands[i].func();
            return 0;
        }
        i++;
    }
    return 0;
}
