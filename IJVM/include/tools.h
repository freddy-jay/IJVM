#ifndef tools_ijvm
#define tools_ijvm
#include <ijvm.h>

typedef struct local_frame {
    word_t* args;
    word_t local_var_size;
    int old_stack_top;
} local_frame;

struct data {
        FILE *in_file;
        FILE *out_file;
        int program_counter;
        int file_header;
        int cp_origin;
        int cp_size;
        int* c_pool;
        int text_origin;
        word_t txt_size;
        byte_t* buffer;
        word_t* local_var;
        int lv_size;
        bool method;
        local_frame* frames;
        int current_frame;
        bool wide;
        bool state;
} *ijvm_state;

word_t read_32(FILE *fp);

byte_t read_8(FILE *fp);

void set_program_counter(int value);

struct data *read_data(FILE *fp);

void initialize_local_variables(int size);

void print_local_variables();

word_t push_local_variable(word_t index, word_t variable);

void print_hex(/*byte_t* byte_pt, int size*/);

word_t merge_short(byte_t byte1, byte_t byte2);

void increment_pc();

word_t read_short();

void increment_pc_short();

void read_and_branch();

void print_cp();

int  get_current_frame();

bool init_local_frame(word_t old_pc);

bool get_in_method();

word_t get_frame_local_variable(int index);

word_t push_frame_local_var(int index, word_t var);

void init_frames(int number_of_frames);

#endif
