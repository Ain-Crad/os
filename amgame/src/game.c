#include <game.h>

// Operating system is a C program!
int main(const char *args) {
    ioe_init();

    printf("Message%d: %s%ld\n", 1, "hello");
    puts("mainargs = \"");
    puts(args); // make run mainargs=xxx
    puts("\"\n");

    splash();

    puts("Press any key to see its key code...\n");
    while (1) {
        print_key();
    }
    return 0;
}
