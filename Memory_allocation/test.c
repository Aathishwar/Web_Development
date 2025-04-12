#include <stdio.h>
#include "malloc.c"

int main() {
    init_memory_pool();

    printf("Allocating 100 bytes...\n");
    void *a = my_malloc(100);
    memory_status();

    printf("Allocating 200 bytes...\n");
    void *b = my_malloc(200);
    memory_status();

    printf("Freeing first block...\n");
    my_free(a);
    memory_status();

    printf("Allocating 50 bytes...\n");
    void *c = my_malloc(50);
    memory_status();

    printf("Freeing second and third block...\n");
    my_free(b);
    memory_status();
    my_free(c);
    memory_status();

    int * arr = my_malloc(sizeof(int)*5);
    printf("Enter values\n");
    for(int i=0;i<5;i++) scanf("%d",&arr[i]);
    printf("the values are\n");
    for(int i=0;i<5;i++) printf("%d ",arr[i]);
    memory_status();
    my_free(arr);
    memory_status();

    printf("C allocation:\n");
    int *arr1= my_calloc(5,sizeof(int));
    printf("The array values:\n");
    for(int i=0;i<5;i++) printf("%d ",arr[i]);
    printf("\n");
    
    
    arr1 = my_realloc(arr1,sizeof(int)*10);
    printf("Enter values :\n");
    for(int i=0;i<10;i++) scanf("%d",&arr[i]);
    printf("The values are:\n");
    for(int i=0;i<10;i++) printf("%d ",arr[i]);
    my_free(arr1);

    return 0;
}
