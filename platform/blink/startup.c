extern unsigned __data_start[],
                __data_end[],
                __bss_start[],
                __bss_end[],
                __etext[],
                main();

/* __reset - the system starts here. */
void __reset(void) {
    unsigned *p, *q;
    /* Copy data segment and zero out bss. */
    p = __data_start;
    q = __etext;
    while (p < __data_end) *p++ = *q++;
    p = __bss_start;
    while (p < __bss_end) *p++ = 0;
    main();
};

void *__vectors[] __attribute((section(".vectors"))) = { 
    __reset,
};
