#include <stdio.h>
#include "my_heap.c"

int main(){


    heap_t *h=(heap_t *)malloc(sizeof(heap_t));
    h=heapCreate(8192,first_fit);
    printf("--------------------------------------\n");
    printf("Heap start at:\t\t%p\n",h->memoryAddress);
    printf("Freelist Head at:\t%p\n",h->flist->head);
    printf("Freelist->Head->Next:\t%p\n",h->flist->head->next);
    printf("Freelist->Head->Prev:\t%p\n",h->flist->head->prev);
    printf("Freelist->Head->Size:\t%zu\n",h->flist->head->size);
    printf("Ptable Start at:\t%p\n",h->ptable->table);
    printf("Ptable N.Entries:\t%d\n",h->ptable->n_entries);
    printf("Ptable Max Entries:\t%d\n",h->ptable->max_entries);
    
    void *p1 = heapAlloc(h,1024);
    void *p2 = heapAlloc(h,1024);
    void *p3 = heapAlloc(h,1024);
    void *p4 = heapAlloc(h,1024);
    printf("--------------------------------------\n");
    printf("\t p1 = %p\n",p1);
    printf("\t p2 = %p\n",p2);
    printf("\t p3 = %p\n",p3);
    printf("\t p4 = %p\n",p4);
    printf("--------------------------------------\n");
    
    
    heapFree(h,p1);
    heapFree(h,p2);
    heapFree(h,p3);
    heapFree(h,p4);
    


    printf("--------------------------------------\n");
    printf("\t Partition Table Status\n");

    struct nodotable *vett = h->ptable->table;
    for(int i=0;i<h->ptable->n_entries;i++){

        printf("[%d] - Base: %p | Limit: %zu\n",i,vett[i].base,vett[i].limit);

    }
    
    printf("--------------------------------------\n");
    printf("\t Freelist Status\n");
    for(struct nodolist* fl=h->flist->head;fl!=NULL;fl=fl->next)
        printf("fl: %p - fl->size: %zu - fl->prev: %p - fl->next: %p\n",fl,fl->size,fl->prev,fl->next);
    
    return 0;


}
