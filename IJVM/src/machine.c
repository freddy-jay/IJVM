#include <ijvm.h>
#include "stack.h"
#include "tools.h"
#include "instructions.h"
#include <stdbool.h>
#include <assert.h>
int get_program_counter()
{
        return ijvm_state->program_counter;
}
byte_t* get_text()
{
        return ijvm_state->buffer;
}
word_t* get_stack()
{
        return stack_ijvm->elements;
}

int stack_size(void){
        return stack_ijvm->current_size;
}

int text_size()
{
        return ijvm_state->txt_size;
}
word_t tos()
{
        return top();
}
byte_t get_instruction()
{
        return ijvm_state->buffer[get_program_counter()];
}
word_t get_local_variable(int i)
{
        if (get_in_method()) {
                return get_frame_local_variable(i);
        } else {
                assert(i < ijvm_state->lv_size);
                return ijvm_state->local_var[i];
        }
}
word_t get_constant(int i)
{
        assert(i < ijvm_state->cp_size);
        return ijvm_state->c_pool[i];
}
void set_input(FILE *fp)
{
        ijvm_state->in_file = fp;
}
void set_output(FILE *fp)
{
        ijvm_state->out_file = fp;
        // call if OP_OUT && out file is not set
}
int init_ijvm(char *binary_file)
{
        FILE *fp = binary_file;
        ijvm_state = read_data(fp);

        if(ijvm_state != NULL)
        {
                FILE *input = fopen("tmp_input", "rb");
                FILE *output = fopen("tmp_output", "w");
                set_output(output);
                set_input(input);

                stack_ijvm = init_stack(128);

                initialize_local_variables(32);

                init_frames(16);

                set_state(true);
                return 0;
        }
        else
        {
                return -1;
        }
}
void destroy_ijvm()
{
        if(ijvm_state->cp_size > 0) {
                free(ijvm_state->c_pool);
        }
        free(ijvm_state->buffer);
        free(ijvm_state->local_var);
        free(ijvm_state);

        free(stack_ijvm->elements);
        free(stack_ijvm);
}
bool step()
{
        byte_t instruction;

        if(get_program_counter() >= text_size()) {
                set_state(false);
                return false;
        } else {
                instruction = get_instruction();
        }
        switch (instruction) {
        case OP_BIPUSH:
                return op_bipush();
        case OP_DUP:
                return op_dup();
        case OP_ERR:
                return op_err();
        case OP_GOTO:
                return op_goto();
        case OP_HALT:
                return op_halt();
        case OP_IADD:
                return op_iadd();
        case OP_IAND:
                return op_iand();
        case OP_IFEQ:
                return op_ifeq();
        case OP_IFLT:
                return op_iflt();
        case OP_ICMPEQ:
                return op_icmpeq();
        case OP_IINC:
                return op_iinc(false);
        case OP_ILOAD:
                return op_iload(false);
        case OP_IN:
                return op_in();
        case OP_INVOKEVIRTUAL:
                return op_invokevirtual();
        case OP_IOR:
                return op_ior();
        case OP_IRETURN:
                return op_ireturn();
        case OP_ISTORE:
                return op_istore(false);
        case OP_ISUB:
                return op_isub();
        case OP_LDC_W:
                return op_ldc_w();
        case OP_NOP:
                return op_nop();
        case OP_OUT:
                return op_out();
        case OP_POP:
                return op_pop();
        case OP_SWAP:
                return op_swap();
        case OP_WIDE:
                return op_wide();
        // case OP_NEWARRAY:
        //         return op_newarray();
        // case OP_IALOAD:
        //         return op_iaload();
        // case OP_IASTORE:
        //         return op_iastore();
        // case OP_NETBIND:
        //         return op_netbind();
        // case OP_NETCONNECT:
        //         return op_netconnect();
        // case OP_NETIN:
        //         return op_netin();
        // case OP_NETOUT:
        //         return op_netout();
        // case OP_NETCLOSE:
        //         return op_netclose();
        default:
                set_state(false);
                return false;
                break;
        }
}

bool finished()
{
        return ijvm_state->state;
}

void run()
{
        printf("\n--> RUN () <--\n\n");
        bool flag = true;
        while((get_program_counter() < text_size()) && (flag == true))  {
                flag = step();
        }
        printf("\n--> PROGRAM FINISHED <--\n");
}
