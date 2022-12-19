#include "event_manager.h"
#include <stdio.h>
#include <string.h>

typedef enum
{
    EM_EVENT_TEST0 = 0,
    EM_EVENT_TEST1,
    EM_EVENT_TEST2,
    EM_EVENT_TEST3,
} em_test_event_t;

void func_int_arg_1(const em_arg_t *in)
{
    int arg;
    memcpy(&arg, in->data, sizeof(in->size));
    printf("FUNC_1 %d\n", arg);
}

void func_int_arg_2(const em_arg_t *in)
{
    int arg;
    memcpy(&arg, in->data, sizeof(in->size));
    printf("FUNC_2 %d\n", arg);
}

void func_void_arg(const em_arg_t *a)
{
    //ignore argument
    printf("FUNC VOID\n");
}

#define STR_SIZE 10
void func_string_arg(const em_arg_t *in)
{
    char arg[STR_SIZE];
    memcpy(&arg, in->data, in->size);
    printf("FUNC_3 %s\n", arg);
}

void test_one_func_one_event()
{
    int a = 10;
    em_arg_t test_arg;
    test_arg.size = sizeof(a);
    memcpy(test_arg.data, &a, test_arg.size);

    printf("Test one fucntion and one event\n");
    em_clear();
    em_init();
    em_subscribe(EM_EVENT_TEST1, func_int_arg_1);
    em_emit(EM_EVENT_TEST1,test_arg);

    int i = 0;
    while (i < 10)
    {
        em_handler();
        i++;
    }
}

void test_two_funcs_two_events()
{
    int a = 10;
    em_arg_t test_arg;
    test_arg.size = sizeof(a);
    memcpy(test_arg.data, &a, test_arg.size);

    printf("Test two functions and two events\n");
    em_clear();
    em_init();
    em_subscribe(EM_EVENT_TEST1, func_int_arg_1);
    em_subscribe(EM_EVENT_TEST2, func_int_arg_2);
    em_emit(EM_EVENT_TEST1,test_arg);
    em_emit(EM_EVENT_TEST2,test_arg);

    int i = 0;
    while (i < 10)
    {
        em_handler();
        i++;
    }
}

void test_two_funcs_one_event()
{
    int a = 10;
    em_arg_t test_arg;
    test_arg.size = sizeof(a);
    memcpy(test_arg.data, &a, test_arg.size);

    printf("Test two functions and one event\n");
    em_clear();
    em_init();
    em_subscribe(EM_EVENT_TEST1, func_int_arg_1);
    em_subscribe(EM_EVENT_TEST1, func_int_arg_2);
    em_emit(EM_EVENT_TEST1,test_arg);

    int i = 0;
    while (i < 10)
    {
        em_handler();
        i++;
    }
}

void test_string_arg()
{
    char str[STR_SIZE] = "hello!";
    em_arg_t test_str_arg;
    test_str_arg.size = sizeof(str)/sizeof(str[0]);
    memcpy(test_str_arg.data, &str, test_str_arg.size);

    printf("Test string argument\n");
    em_clear();
    em_init();
    em_subscribe(EM_EVENT_TEST1, func_string_arg);
    em_emit(EM_EVENT_TEST1,test_str_arg);

    int i = 0;
    while (i < 10)
    {
        em_handler();
        i++;
    }
}

void test_argument_change()
{
    int a = 10;
    em_arg_t test_arg;
    test_arg.size = sizeof(a);
    memcpy(test_arg.data, &a, test_arg.size);

    printf("Test argument change\n");
    em_clear();
    em_init();
    em_subscribe(EM_EVENT_TEST1, func_int_arg_1);
    em_emit(EM_EVENT_TEST1,test_arg);
    a = 77;
    memcpy(test_arg.data, &a, test_arg.size);
    em_emit(EM_EVENT_TEST1,test_arg);

    int i = 0;
    while (i < 10)
    {
        em_handler();
        i++;
    }
}

void test_ring_overflow()
{
    int a = 10;
    em_arg_t test_arg;
    test_arg.size = sizeof(a);
    memcpy(test_arg.data, &a, test_arg.size);

    printf("Test emit more than EVENT_RING_SIZE\n"); //EVENT_RING_SIZE == 10
    em_clear();
    em_init();
    em_subscribe(EM_EVENT_TEST1, func_int_arg_1);

    for (int i = 0; i < 20; i++)
    {
        a = i;
        memcpy(test_arg.data, &a, test_arg.size);
        em_emit(EM_EVENT_TEST1,test_arg);
    }

    int i = 0;
    while (i < 20)
    {
        em_handler();
        i++;
    }
}

void test_double_subscribe()
{
    int a = 10;
    em_arg_t test_arg;
    test_arg.size = sizeof(a);
    memcpy(test_arg.data, &a, test_arg.size);

    printf("Test double subscribe\n");
    em_clear();
    em_init();
    em_subscribe(EM_EVENT_TEST1, func_int_arg_1);
    em_subscribe(EM_EVENT_TEST1, func_int_arg_1); //should not be written in a dictionary
    em_emit(EM_EVENT_TEST1,test_arg); //waiting for single call

    int i = 0;
    while (i < 10)
    {
        em_handler();
        i++;
    }
}

void test_no_subscribe()
{
    int a = 10;
    em_arg_t test_arg;
    test_arg.size = sizeof(a);
    memcpy(test_arg.data, &a, test_arg.size);

    printf("Test emit without subscribe\n");
    em_clear();
    em_init();
    em_emit(EM_EVENT_TEST1,test_arg); //waiting no call

    int i = 0;
    while (i < 10)
    {
        em_handler();
        i++;
    }
}

int main()
{
    test_one_func_one_event();
    test_two_funcs_one_event();
    test_two_funcs_two_events();
    test_string_arg();
    test_argument_change();
    test_double_subscribe();
    test_no_subscribe();
    test_ring_overflow();

    return 0;
}
