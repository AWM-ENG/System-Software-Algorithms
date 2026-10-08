//Git Hub: AWM-ENG
#include<stdio.h>
#include<stdbool.h>
#include<stdint.h>

//Memory
uint8_t memory[1024];


//header
typedef struct Header
{
    int size;
    bool is_free;
    struct Header *prevheader;
    struct Header *postheader;
}Header;

//init function for memory
void init_memory()
{
    Header *header = (Header*)memory;
    header->is_free = true;
    header->size = 1024 - sizeof(Header);
    header->prevheader = NULL;
    header->postheader = NULL;

}

//Print memory
void print_memory(void) {
    printf("===== Memory State =====\n");
    Header *curr = (Header *)memory;
    int block = 0;
    while (curr != NULL) {
        printf("Block %d: Address=%p | Size=%zu | Status=%s\n",
               block++, (void *)curr, curr->size, curr->is_free ? "FREE" : "ALLOCATED");
        curr = curr->postheader;
    }
    printf("========================\n");
}

//Add to memory
void *add_memory(int size)
{
    if(size <= 0 || size > 1024 - sizeof(Header))
    {
        return NULL;
    }
    int aligned_size = (size + 7) & ~7;
    uint8_t *start = memory;
    uint8_t *end = memory + 1024;
    while (start < end)
    {
        Header *header = (Header*)start;
        if(header->is_free && header->size >= aligned_size)
        {
            int old_size = header->size;
            header->is_free = false;
            int remaining_size = old_size - aligned_size - sizeof(Header);
            if(remaining_size >= sizeof(Header) + 4)
            {
                header->size = aligned_size;
                Header *newheader= (Header*)(start + sizeof(Header) + aligned_size);
                newheader->is_free = true;
                newheader->size = remaining_size;
                Header *old_post = header->postheader;
                header->postheader = newheader;
                newheader->prevheader = header;
                newheader->postheader = old_post;
                if(old_post != NULL)
                {
                    old_post->prevheader = newheader;
                }
            } 
            return (void*)(start + sizeof(Header));
        }
        if(header->postheader != NULL)
        {
            start = (uint8_t*)header->postheader;
        }
        else
        {
            break;
        }
    }
    return NULL;
}

//Remove memory
void remove_memory(void *start)
{
    if(!start || (uint8_t*)start < memory + sizeof(Header) || (uint8_t*)start >= memory + 1024)
    {
        return;
    }
    Header *header = (Header*)((uint8_t*)start - sizeof(Header));
    if(header->is_free)
    {
        return;
    }
    header->is_free = true;
    //Grubing
    //post address
    if(header->postheader != NULL && header->postheader->is_free)
    {
        header->size += header->postheader->size + sizeof(Header);
        header->postheader = header->postheader->postheader;
        if(header->postheader != NULL)
        {
            header->postheader->prevheader = header;
        }
    }
    //pre address
    if(header->prevheader != NULL && header->prevheader->is_free)
    {
        header->prevheader->size += header->size + sizeof(Header);
        header->prevheader->postheader = header->postheader;
        if(header->prevheader->postheader != NULL)
        {
            header->prevheader->postheader->prevheader = header->prevheader;
        }
    }
}



int main(void) {
    init_memory();
//test code
    /* printf("--- Initial State ---\n");
    print_memory();

    void *p1 = add_memory(10);
    void *p2 = add_memory(20);
    void *p3 = add_memory(30);

    printf("\n--- After Allocation ---\n");
    print_memory();

    printf("\n--- Freeing Middle Block (p2) ---\n");
    remove_memory(p2);
    print_memory();

    printf("\n--- Freeing Remaining Blocks (Coalescing) ---\n");
    remove_memory(p1);
    remove_memory(p3);
    print_memory();
 */
    //Your code here to test the memory management functions

    return 0;
}
