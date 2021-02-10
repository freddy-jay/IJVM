#include "instructions.h"
#include "ijvm.h"
#include "tools.h"
#include "stack.h"
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>

bool op_bipush() {
        printf("\nBIPUSH - 0x%x\n", OP_BIPUSH);
        increment_pc();

        word_t bipush_value;

        bipush_value = (char)get_instruction();
        increment_pc();
        printf("%d\n", bipush_value );
        push(bipush_value);

        assert(tos() == bipush_value);
        return true;
}
bool op_dup() {
        printf("\nOP_DUP - 0x%x\n", OP_DUP);
        increment_pc();

        word_t previous_top;

        previous_top = tos();
        push(tos());

        assert(previous_top == tos());
        return true;
}
bool op_err(){
        printf("\nOP_ERR - 0x%x\n", OP_ERR);

        set_state(false);

        return false;
}
bool op_goto(){
        printf("\nOP_GOTO - 0x%x\n", OP_GOTO);
        increment_pc();

        short operator1;

        operator1 = read_short();
        printf("%d\n", operator1);
        assert(operator1 < text_size());
        set_program_counter((get_program_counter()-1) + operator1);
        printf("GOTO: %d\n", get_program_counter());

        return true;
}
bool op_halt(){
        printf("\nHALT - 0x%x\n", OP_HALT);
        increment_pc();

        set_state(false);

        return false;
}
bool op_iadd(){
        printf("\nIADD - 0x%x\n", OP_IADD);
        increment_pc();

        word_t iadd;

        iadd = pop() + pop();
        push(iadd);

        assert(tos() == iadd);
        return true;
}
bool op_iand(){
        printf("\nIAND - 0x%x\n", OP_IAND);
        increment_pc();

        word_t iand;

        iand = pop() & pop();
        push(iand);

        assert(tos() == iand);
        return true;
}
bool op_ifeq(){
        printf("\nOP_IFEQ - 0x%x\n", OP_IFEQ);
        increment_pc();

        if(pop() == 0) {
                read_and_branch();
                return true;
        }

        increment_pc_short();

        return true;
}
bool op_iflt(){
        printf("\nOP_IFLT - 0x%x\n", OP_IFLT);
        increment_pc();

        if(pop() < 0) {
                read_and_branch();
                return true;
        }

        increment_pc_short();

        return true;
}
bool op_icmpeq(){
        printf("\nOP_ICMPEQ\n");
        increment_pc();

        if (pop() == pop()) {
                read_and_branch();
                return true;
        }

        increment_pc_short();

        return true;
}
bool op_iinc(bool wide) {
        printf("\nOP_IINC - 0x%x\n", OP_IINC);
        increment_pc();

        word_t operator1, operator2, result;

        if (wide == true) {
                operator1 = read_short();
                increment_pc_short();
                operator2 = read_short();
                increment_pc_short();
        } else {
                operator1 = get_instruction();
                increment_pc();
                operator2 = (char)get_instruction();
                increment_pc();
        }
        result = get_local_variable(operator1) + operator2;
        printf("local_var[%d] = %d + %d = %d\n",operator1,get_local_variable(operator1), operator2, result);
        ijvm_state->local_var[operator1] = result;
        assert(ijvm_state->local_var[operator1] == result);
        return true;
}
bool op_iload(bool wide) {
        printf("\nOP_ILOAD - 0x%x\n", OP_ILOAD);
        increment_pc();

        word_t index, local_var;

        if (wide == true) {
                index = read_short();
                increment_pc_short();
        } else {
                index = get_instruction();
                increment_pc();
        }

        if (get_in_method()) {
                local_var = get_frame_local_variable(index);
        }
        else {
                local_var = get_local_variable(index);
        }

        push(local_var);
        assert(tos() == local_var);
        return true;
}
bool op_in(){
        printf("\nOP_IN - 0x%x\n", OP_IN);
        increment_pc();

        word_t operator1, result;

        operator1 = fread(&result, sizeof(char), 1, ijvm_state->in_file);

        if (operator1 ==1) {
                push((char)result);
                assert(tos() == (char)result);
                return true;
        } else {
                push(0);
                assert(tos() == 0);
                return true;
        }
}
bool op_invokevirtual(){
        printf("\nOP_INVOKEVIRTUAL - 0x%x\n", OP_INVOKEVIRTUAL);
        increment_pc();

        word_t cp_index, method_area_pointer, old_pc, local_var_size, num_args;

        ijvm_state->method = true;

        cp_index = read_short();
        increment_pc_short();

        method_area_pointer = get_constant(cp_index);
        old_pc = get_program_counter();
        set_program_counter(method_area_pointer);

        printf("Moving to %d\n", get_program_counter());

        init_local_frame(old_pc);
        return true;
}
bool op_ior(){
        printf("\nIOR - 0x%x\n", OP_IOR);
        increment_pc();

        word_t result;

        result = pop() | pop();
        push(result);
        assert(tos() == result);
        return true;
}
bool op_ireturn(){
        printf("\nOP_IRETURN - 0x%x\n", OP_IRETURN);

        local_frame* frame = &ijvm_state->frames[get_current_frame()-1];

        set_program_counter(frame->args[0]);

        stack_ijvm->elements[frame->old_stack_top] = tos();
        stack_ijvm->current_size = frame->old_stack_top;

        ijvm_state->current_frame--;

        if (ijvm_state->current_frame == 0) {
                set_in_method(false);
        }

        return true;
}
bool op_istore(bool wide){
        printf("\nOP_ISTORE - 0x%x\n", OP_ISTORE);
        increment_pc();

        word_t index, value;

        printf("- wide %d , get_in_method %d\n", wide, get_in_method() );

        index = read_short();

        if (wide == true) {
                index = read_short();
                increment_pc_short();
        } else {
                index = get_instruction();
                increment_pc();
        }

        value = pop();

        if(get_in_method()) {
                push_frame_local_var(index, value);
        } else {
                push_local_variable(index, value);
                assert(get_local_variable(index) == value);
        }
        return true;
}
bool op_isub(){
        printf("\nISUB - 0x%x\n", OP_ISUB);
        increment_pc();

        word_t operator1, operator2, result;

        operator1 = pop();
        operator2 = pop();
        result = operator2 - operator1;
        push(result);
        assert(tos() == result);
        return true;
}
bool op_ldc_w(){
        printf("\nOP_LDC_W - 0x%x\n", OP_LDC_W);
        increment_pc();

        word_t constant;
        short index;

        index = read_short();
        constant = get_constant(index);

        push(constant);
        increment_pc_short();

        assert(tos() == constant);
        return true;
}
bool op_nop() {
        printf("\nOP_NOP - 0x%x\n", OP_NOP);
        increment_pc();

        return true;
}
bool op_out(){
        printf("\nOUT - 0x%x\n", OP_OUT);
        increment_pc();

        char ascii_out;

        ascii_out = pop();

        fprintf(ijvm_state->out_file, "%c", ascii_out);

        return true;
}
bool op_pop() {
        printf("\nPOP - 0x%x\n", OP_POP);
        increment_pc();

        pop();

        return true;
}
bool op_swap()
{
        printf("\nSWAP - 0x%x\n", OP_SWAP);
        increment_pc();

        word_t operator1,operator2;

        operator1 = pop();
        operator2 = pop();

        push(operator1);
        push(operator2);

        return true;
}

bool op_wide()
{
        printf("\nOP_WIDE - 0x%x\n", OP_WIDE);
        increment_pc();

        byte_t instruction;

        instruction = get_instruction();

        switch (instruction) {
        case OP_ILOAD:
                op_iload(true);
                break;
        case OP_ISTORE:
                op_istore(true);
                break;
        case OP_IINC:
                op_iinc(true);
                break;
        }

        return true;
}
