#ifndef __CPU_TIM_H
#define __CPU_TIM_H

#include "include.h"
#include "cpu/cpu.h"

class CPU_TIM{
    // Yeah, I got Timbs.
    public:
    byte &DIV, &TIMA, &TMA, &TAC;
    CPU_TIM(CPU& parent);
};

#endif