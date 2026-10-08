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
void print_memory(int length)
{
    printf("-----Memory-----\n");
    for(int i =0;i< length;i++)
    {
        printf("%d\n",memory[i]);
    }
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





int main()
{
init_memory();

//Your code here to test the memory management functions

    return 0;
}
