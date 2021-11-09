
#define ARRAR_ERASED -1
#define SUCCESS 0
#define INVALID_POSITION 1
#define POSTION_INIT 2
#define POSTION_NOT_INIT 3
#define POSTION_EMPTY 4
#define ARRAY_FULL 5

typedef struct CArray
{
    int *array;
    int size;
}CArray;

// returns array
CArray *get_carray(int size);
CArray *get_copy_carray(CArray *array);

// inp[ut / output
int insert_value_carray(CArray *array, int postion, int value);
int remove_value_carray(CArray *array, int postion);
int push_value_carray(CArray *array, int value);
int update_value_carray(CArray *array, int postion, int value);

// erase
int erase_carray(CArray *array);

// switching
int switch_value_carray(CArray *array, int postion1, int postion2);
int reverse_carray(CArray *array);

// sortint
int bubble_sort_carray(CArray *array);
int selection_sort_carray(CArray *array);
int insertion_sort_carray(CArray *array);
int blender_carray(CArray *array);

// searhing
int valueOcurrance_carray(CArray *array, int value);
CArray *value_position_carray(CArray *array, int value);
int find_max_carray(CArray *array);
int find_min_carray(CArray *array);

// display
int display_carray(CArray *array);
