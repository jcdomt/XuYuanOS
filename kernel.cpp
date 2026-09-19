extern "C" void kernel_main()
{
    volatile unsigned int *uart =
        (volatile unsigned int *)0x09000000;

    *uart = 'H';
    *uart = 'e';
    *uart = 'l';
    *uart = 'l';
    *uart = 'o';
    *uart = '\n';

    while (true) {
        asm volatile("wfe");
    }
}