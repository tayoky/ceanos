#include <stdint.h>
#include <util.h>
#include <idt/idt.h>
#include <drivers/video/vga/vga.h>
#include <timer.h>

uint64_t ticks;
const uint32_t freq = 100;

void onIrq0(struct InterruptRegisters *regs)
{
    ticks += 1;
}

void timer_init()
{
    ticks = 0;
    irq_install_handler(0,&onIrq0);

    //119318.16666 Mhz
    uint32_t divisor = 1193180/freq;

    //0011 0110
    outPortB(0x43,0x36);
    outPortB(0x40,(uint8_t)(divisor & 0xFF));
    outPortB(0x40,(uint8_t)((divisor >> 8) & 0xFF));

    print("timer enabled\n");
}

void sleep(uint32_t milliseconds)
{
    uint64_t target_ticks = ticks + (milliseconds / 10); // 100 Hz => 10 ms per tick

    while (ticks < target_ticks) {
    }
}

void wait(uint32_t sec)
{
    uint64_t target_ticks = ticks + (sec * 1000);

    while (ticks < target_ticks) {
    }
}

