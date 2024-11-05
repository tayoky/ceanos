#ifndef TIMER_H
#define TIMER_H

void timer_init();
void onIrq0(struct InterruptRegisters *regs);
void sleep(uint32_t milliseconds);
void wait(uint32_t sec);

#endif //timer_h
