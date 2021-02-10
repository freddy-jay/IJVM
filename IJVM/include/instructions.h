#ifndef ijvm_instructions
#define ijvm_instructions
#include "ijvm.h"

bool op_bipush();
bool op_dup();
bool op_err();
bool op_goto();
bool op_halt();
bool op_iadd();
bool op_iand();
bool op_ifeq();
bool op_iflt();
bool op_icmpeq();
bool op_iinc(bool wide);
bool op_iload(bool wide);
bool op_in();
bool op_invokevirtual();
bool op_ior();
bool op_ireturn();
bool op_istore(bool wide);
bool op_isub();
bool op_ldc_w();
bool op_nop();
bool op_out();
bool op_pop();
bool op_swap();
bool op_wide();

#endif
