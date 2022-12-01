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

void test_func1(em_arg_t in)
{
    int arg;
    memcpy(&arg, in.data, sizeof(in.size));
    printf("FUNC_1 %d\n", arg);
}

void test_func2(em_arg_t in)
{
    int arg;
    memcpy(&arg, in.data, sizeof(in.size));
    printf("FUNC_2 %d\n", arg);
}

void test_func_void(em_arg_t a)
{
    //ignore argument
    printf("FUNC VOID\n");
}

#define STR_SIZE 10
void test_func3(em_arg_t in)
{
    char arg[STR_SIZE];
    memcpy(&arg, in.data, in.size);
    printf("FUNC_3 %s\n", arg);
}

void test_1()
{
    int a = 10;
    em_arg_t test_arg;
    test_arg.size = sizeof(a);
    memcpy(test_arg.data, &a, test_arg.size);

    printf("Test one fucntion and one event\n");
    em_init();
    em_subscribe(EM_EVENT_TEST1, test_func1);
    em_emit(EM_EVENT_TEST1,test_arg);

    int i = 0;
    while (i < 10)
    {
        em_handler();
        i++;
    }
}

void test_1_2()
{
    int a = 10;
    em_arg_t test_arg;
    test_arg.size = sizeof(a);
    memcpy(test_arg.data, &a, test_arg.size);

    printf("Test two functions and two events\n");
    em_init();
    em_subscribe(EM_EVENT_TEST1, test_func1);
    em_subscribe(EM_EVENT_TEST2, test_func2);
    em_emit(EM_EVENT_TEST1,test_arg);
    em_emit(EM_EVENT_TEST2,test_arg);

    int i = 0;
    while (i < 10)
    {
        em_handler();
        i++;
    }
}

void test_1_1()
{
    int a = 10;
    em_arg_t test_arg;
    test_arg.size = sizeof(a);
    memcpy(test_arg.data, &a, test_arg.size);

    printf("Test two functions and one event\n");
    em_init();
    em_subscribe(EM_EVENT_TEST1, test_func1);
    em_subscribe(EM_EVENT_TEST1, test_func2);
    em_emit(EM_EVENT_TEST1,test_arg);

    int i = 0;
    while (i < 10)
    {
        em_handler();
        i++;
    }
}

void test_3()
{
    char str[STR_SIZE] = "hello!";
    em_arg_t test_str_arg;
    test_str_arg.size = sizeof(str)/sizeof(str[0]);
    memcpy(test_str_arg.data, &str, test_str_arg.size);

    printf("Test string argument\n");
    em_init();
    em_subscribe(EM_EVENT_TEST1, test_func3);
    em_emit(EM_EVENT_TEST1,test_str_arg);

    int i = 0;
    while (i < 10)
    {
        em_handler();
        i++;
    }
}

void test_1_1argument_change()
{
    int a = 10;
    em_arg_t test_arg;
    test_arg.size = sizeof(a);
    memcpy(test_arg.data, &a, test_arg.size);

    printf("Test argument change\n");
    em_init();
    em_subscribe(EM_EVENT_TEST1, test_func1);
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

void test_1_ring_overflow()
{
    int a = 10;
    em_arg_t test_arg;
    test_arg.size = sizeof(a);
    memcpy(test_arg.data, &a, test_arg.size);

    printf("Test emit more than EVENT_RING_SIZE\n"); //EVENT_RING_SIZE == 10
    em_init();
    em_subscribe(EM_EVENT_TEST1, test_func1);

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

void test_1_1double_sub()
{
    int a = 10;
    em_arg_t test_arg;
    test_arg.size = sizeof(a);
    memcpy(test_arg.data, &a, test_arg.size);

    printf("Test double subscribe\n");
    em_init();
    em_subscribe(EM_EVENT_TEST1, test_func1);
    em_subscribe(EM_EVENT_TEST1, test_func1); //should not be written in a dictionary
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
    test_1();

    return 0;
}
