#include <tools.h>
#include "ijvm.h"
#include <stdio.h>
#include "stack.h"
#include <assert.h>

word_t read_32(FILE *fp)
{
        uint32_t num;
        fread(&num, sizeof(uint32_t), 1, fp);
        return ((num>>24)&0xff) | ((num<<8)&0xff0000) |((num>>8)&0xff00) | ((num<<24)&0xff000000);
}
byte_t read_8(FILE *fp)
{
        uint8_t num;
        fread(&num, sizeof(char), 1, fp);
        return num;
}
void set_program_counter(int value)
{
        ijvm_state->program_counter = value;
        assert(get_program_counter() == value);
}
struct data *read_data(FILE *fp)
{
        fp = fopen(fp, "rb");
        ijvm_state = (struct data*)malloc(sizeof(struct data));
        set_program_counter(0);
        ijvm_state->file_header = read_32(fp);
        ijvm_state->cp_origin = read_32(fp);
        ijvm_state->cp_size = read_32(fp);
        if(ijvm_state->cp_size > 0)
        {
                ijvm_state->c_pool = (word_t *)malloc(ijvm_state->cp_size * sizeof(word_t));
                for (size_t i = 0; i < ijvm_state->cp_size/4; i++) {
                        ijvm_state->c_pool[i] = read_32(fp);
                }
        }
        ijvm_state->text_origin = read_32(fp);
        ijvm_state->txt_size = read_32(fp);
        ijvm_state->buffer = (byte_t *)malloc(ijvm_state->txt_size * sizeof(byte_t));
        for (size_t i = 0; i < ijvm_state->txt_size; i++) {
                ijvm_state->buffer[i] = read_8(fp);
        }
        fclose(fp);
        return ijvm_state;
}
void initialize_local_variables(int size)
{
        ijvm_state->local_var = (word_t *)malloc(size * sizeof(word_t));
        ijvm_state->lv_size = size;
}
void print_local_variables()
{
        printf("\n--> print_local_variables() <--\n\n");
        for (size_t i = 0; i < ijvm_state->lv_size; i++) {
                printf("[%d] - %d\n", i, ijvm_state->local_var[i]);
        }
        printf("\n---\n");
}
word_t push_local_variable(word_t index, word_t variable)
{
        assert(index < ijvm_state->lv_size);
        ijvm_state->local_var[index] = variable;
        printf("hi tools.c\n");
}
void print_hex(/*byte_t* byte_pt, int size*/)
{
        printf("\n--> print_hex() <--\n\n");
        for (size_t i = 0; i < text_size(); i++) {
                printf("0x%02x ", ijvm_state->buffer[i]);
                if ((i+1)%4 == 0) {
                        printf("\n");
                }
        }
        printf("\n---\n");
}
word_t merge_short(byte_t byte1, byte_t byte2)
{
        return (byte1 << 8) | (byte2 & 0xff);
}
void increment_pc()
{
        ijvm_state->program_counter++;
}
word_t read_short()
{
        byte_t byte1 = ijvm_state->buffer[get_program_counter()];
        byte_t byte2 = ijvm_state->buffer[get_program_counter()+1];
        return merge_short(byte1, byte2);
}
void increment_pc_short()
{
        increment_pc();
        increment_pc();
}
void read_and_branch()
{
        word_t increment;
        int value;

        increment= read_short();
        value = (get_program_counter()-1) + increment;

        set_program_counter(value);

        assert(get_program_counter() == value);
}
void print_cp()
{
        for (size_t i = 0; i < ijvm_state->cp_size; i++) {
                printf("[%d] - %d\n", i, ijvm_state->c_pool[i]);
        }
}
int  get_current_frame()
{
        return ijvm_state->current_frame;
}
bool init_local_frame(word_t old_pc)
{
        word_t num_args, local_var_size, total_size;
        local_frame* frame;

        frame = (local_frame *)malloc(sizeof(local_frame));
        num_args = read_short();
        increment_pc_short();
        printf("- number of args: %d\n", num_args);
        local_var_size = read_short();
        increment_pc_short();
        printf("- local vars: %d\n", frame->local_var_size);
        total_size = local_var_size + num_args;
        printf("- total size: %d\n", total_size);

        if(total_size>0) {
                frame->args = (word_t *)malloc(total_size * sizeof(word_t));
        }
        if (num_args > 0) {
                for (size_t i = num_args-1; i > 0; i--) {
                        frame->args[i] = pop();
                }
        }

        frame->args[0] = old_pc;
        frame->old_stack_top = size();

        printf("- frame number %d\n", get_current_frame());

        ijvm_state->frames[get_current_frame()] = *frame;
        ijvm_state->current_frame++;

        return true;
}
bool get_in_method()
{
        return ijvm_state->method;
}
void set_in_method(bool value)
{
        ijvm_state->method = value;
        assert(ijvm_state->method == value);
}
word_t get_frame_local_variable(int index)
{
        local_frame* frame = &ijvm_state->frames[get_current_frame()-1];
        printf("- frame->args[%d] = %d\n", index, frame->args[index] );
        return frame->args[index];
}
word_t push_frame_local_var(int index, word_t var)
{
        local_frame* frame = &ijvm_state->frames[get_current_frame()-1];
        frame->args[index] = var;
        printf("- frame->args[%d] = %d\n",index,frame->args[index] );
}
void init_frames(int number_of_frames)
{
        ijvm_state->frames = (local_frame *)malloc(number_of_frames * sizeof(local_frame));
        ijvm_state->current_frame = 0;
        ijvm_state->method = false;
}
void set_state(bool state)
{
        ijvm_state->state = state;
        assert(ijvm_state->state == state);
}
