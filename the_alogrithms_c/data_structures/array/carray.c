/*
return codes

-1 - array erased
0 - success
1 - invalud postion
2 - postion already initialized(use update function)
3 - postion not initialized (use insert function)
4 - postion already empty
5 - array is full

*/

#include "carray.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(CArray *array, int postion1, int postion2);

CArray *get_carray(int size)
{
    CArray *array = (CArray *)malloc(sizeof(CArray));
    array->array = (int *)malloc(sizeof(int) * size);
    array->size = size;
    int i;
    for (i = 0; i < size; i++)
        array->array[i] = 0;

    return array;
}

int insert_value_carray(CArray *array, int postion, int value)
{
    if (postion >= 0 && postion < array->size)
    {
        if (array->array[postion] == 0)
        {
            array->array[postion] = value;
            return SUCCESS;
        }
        else
        {
            return POSTION_INIT;
        }
    }
    return INVALID_POSITION;
}

int remove_value_carray(CArray *array, int postion)
{
    if (postion >= 0 && postion < array->size)
    {
        if (array->array[postion] != 0)
        {
            array->array[postion] = 0;
        }
        else
            return POSTION_EMPTY;
    }
    return INVALID_POSITION;
}

int push_value_carray(CArray *array, int value)
{
    int i;
    int ok = 0;
    for (i = 0; i < array->size; i++)
    {
        if (array->array[i] == 0)
        {
            array->array[i] = value;
            ok = 1;
            break;
        }
    }
    if (ok == 1)
        return SUCCESS;
    else
        return ARRAY_FULL;
}

int update_value_carray(CArray *array, int position, int value)
{
    if (position >= 0 && position < array->size)
    {
        if (array->array[position] != 0)
        {
        }
        else
            return POSTION_NOT_INIT;
    }
    return INVALID_POSITION;
}

int erase_carray(CArray *array)
{
    int i;
    for (i = 0; i < array->size; i++)
        array->array[i] = 0;

    return 0;
}

int switch_value_carray(CArray *array, int position1, int position2)
{
    if (position1 >= 0 && position1 < array->size && position2 >= 0 && position2 < array->size)
    {
        int temp = array->array[position1];
        array->array[position1] = array->array[position2];
        array->array[position2] = temp;
    }
    return INVALID_POSITION;
}

int reverse_carray(CArray *array)
{
    int i;
    for (i = 0; i < array->size / 2; i++)
        swap(array, i, array->size - i - 1);

    return SUCCESS;
}

int display_carray(CArray *array)
{
    int i;
    printf("\nC ARRAY\n");
    for (i = 0; i < array->size; i++)
    {
        printf("%d ", array->array[i]);
    }
    printf("\n");
    return 0;
}

int blender_carray(CArray *array)
{
    srand(time(NULL) * array->size);
    int i;
    int total = array->size * 100;
    for (i = 0; i < total; i++)
    {
        swap(array, rand() % array->size, rand() % array->size);
    }
    return 0;
}

CArray *get_copy_carray(CArray *arr)
{
    CArray *array = (CArray *)malloc(sizeof(CArray));
    array->array = (int *)malloc(sizeof(int) * arr->size);
    array->size = arr->size;
    int i;
    for (i = 0; i < arr->size; i++)
    {
        array->array[i] = arr->array[i];
    }
    return array;
}

void swap(CArray *array, int position1, int position2)
{
    int temp = array->array[position1];
    array->array[position1] = array->array[position2];
    array->array[position2] = temp;
}

int bubble_sort_carray(CArray *array)
{
    int i, j;
    for (i = 0; i < array->size - 1; i++)
    {
        for (j = 0; j < array->size - i - 1; j++)
        {
            if (array->array[j] > array->array[j + 1])
            {
                swap(array, j, j + 1);
            }
        }
    }
    return 0;
}

int selection_sort_carray(CArray *array)
{
    int i, j, min;
    for (i = 0; i < array->size - 1; i++)
    {
        min = i;
        for (j = i + 1; j < array->size; j++)
            if (array->array[j] < array->array[min])
                min = j;
        swap(array, min, i);
    }
    return 0;
}

int insertion_sort_carray(CArray *array)
{
    int i, j, num;
    for (i = 1; i < array->size; i++)
    {
        num = array->array[i];
        j = i - 1;
        while (j >= 0 && array->array[j] > num)
        {
            array->array[j + 1] = array->array[j];
            j--;
        }
        array->array[j + 1] = num;
    }
    return 0;
}

int valueOcurrance_carray(CArray *array, int value)
{
    int i, total = 0;
    for (i = 0; i < array->size; i++)
    {
        if (array->array[i] == value)
            total++;
    }
    return total;
}

CArray *value_position_carray(CArray *array, int value)
{
    int i, j = 0;
    int total = valueOcurrance_carray(array, value);
    CArray *result_array = get_carray(total);
    for (i = 0; i < array->size; i++)
    {
        if (array->array[i] == value)
        {
            result_array->array[j] = i;
            j++;
        }
    }
    return result_array;
}

int find_min_carray(CArray *array)
{
    int i;
    int min = array->array[0];
    for (i = 1; i < array->size; i++)
    {
        if (array->array[i] < min)
        {
            min = array->array[i];
        }
    }
    return min;
}

int find_max_carray(CArray *array)
{
    int i;
    int max = array->array[0];
    for (i = 1; i < array->size; i++)
    {
        if (array->array[i] > max)
        {
            max = array->array[i];
        }
    }
    return max;
}
