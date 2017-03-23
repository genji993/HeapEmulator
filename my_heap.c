#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SIZE_ALIGN 16
#define START_ENTRIES 10

typedef enum{

    first_fit,
    worst_fit,
    best_fit

}free_policy_t;

struct nodolist{

    size_t size;
    struct nodolist *next;
    struct nodolist *prev;

};

typedef struct freelist_s{

    struct nodolist* head;
    free_policy_t policy;


}freelist_t;



struct nodotable{

    void *base;
    size_t limit;

};

typedef struct ptable_s{

    struct nodotable* table;
    int n_entries;
    int max_entries;




}ptable_t;


typedef struct heap_s{

    freelist_t *flist;
    ptable_t *ptable;
    free_policy_t policy;
    void *memoryAddress;

}heap_t;

size_t alignSize(size_t currSize){

    return currSize+(SIZE_ALIGN-1) & ~ (SIZE_ALIGN-1);


}


heap_t *heapCreate(size_t heapSize,free_policy_t policy){

    size_t alignedSize = alignSize(heapSize);
    

    
    void *memory = malloc(alignedSize); 
   


    freelist_t *flist=(freelist_t*)malloc(sizeof(freelist_t));
    assert(flist!=NULL);
    flist->head=(struct nodolist*)memory;
    flist->head->next=NULL;
    flist->head->prev=NULL;
    flist->head->size=alignedSize;
    flist->policy=policy;

    ptable_t *ptable =(ptable_t *)malloc(sizeof(ptable_t));
    assert(ptable!=NULL);
    ptable->n_entries=0;
    ptable->max_entries=START_ENTRIES;
    struct nodotable *table = (struct nodotable *)malloc(START_ENTRIES*(sizeof(struct nodotable)));
    ptable->table=table;

    heap_t *new_heap =(heap_t *)malloc(sizeof(heap_t));
    assert(new_heap!=NULL);
    new_heap->policy=policy;
    new_heap->flist=flist;
    new_heap->ptable=ptable;
    new_heap->policy=policy;
    new_heap->memoryAddress=memory; 


    return new_heap;
}


/*
 * heapAlloc()
 * 1.Discrimina la free_policy
 * 2.Cerca il frame a seconda della policy
 * 3.Elimina il frame dalla freelist
 * 4.Inserisce entry nella partition table
 */
void *heapAlloc(heap_t*h,size_t size)
{

    size = alignSize(size);
    free_policy_t policy=h->policy;
    struct nodolist*fl=h->flist->head,*retval;



    switch(policy){
    
        case first_fit:

            while(fl!=NULL){

                if(fl->size>=size)
                    break;


                fl=fl->next;
            }

            break;
        case best_fit:
            break;
        case worst_fit:
            break;




    }

    if(fl->size==size){

        // Elimino Nodo
        fl->prev->next=fl->next;
        fl->next->prev=fl->prev;
        fl->size=0;
        fl->next=NULL;
        fl->prev=NULL;



    }else{

        // Resize Nodo di testa
        size_t aux = fl->size - size;
       
        void *sum =(void*) ((unsigned long)fl)+size;
        struct nodolist *node =(struct nodolist*)sum;
        

        if(fl->prev==NULL){
            
            // Nodo di Testa
            
          
            node->size=aux; 
            node->next=NULL;
            node->prev=NULL;
            
            h->flist->head=node;




        }else{

            // Caso Generale
            
            struct nodolist* tmp=fl->next;
            fl->prev->next=node;
            node->size=aux;
            node->next=tmp;
            node->prev=fl->prev;


        }




    }


    /* Inserisco in ptable */

    if(h->ptable->n_entries>=h->ptable->max_entries){

        /* table realloc */
        h->ptable->table=realloc(h->ptable->table,2*h->ptable->max_entries*sizeof(struct nodotable));
        h->ptable->max_entries=2*h->ptable->max_entries;
   
    }

    int n_entries = h->ptable->n_entries;
    h->ptable->table[n_entries].base=fl;
    h->ptable->table[n_entries].limit=size;
    h->ptable->n_entries++;
   




    return fl;


}


/*
 * 1.Partition Table Lookup
 * 2.FreeList Frame Add
 * 3. Recompact
 */

void heapFree(heap_t *h,void *p){

    struct nodotable *table = h->ptable->table;
    struct nodolist *fl = h->flist->head,*aux;
    int i;
    size_t limit;
   
    
    for(i=0;i<h->ptable->n_entries;i++)
        if(table[i].base==p) break;
    limit = table[i].limit;
    table[i].base=table[h->ptable->n_entries-1].base;
    table[i].limit=table[h->ptable->n_entries-1].limit;
    h->ptable->n_entries--;


    while(fl!=NULL){

        if((void*)fl>p)
            break;
        else fl=fl->next;


    }

    aux=(struct nodolist*)p;
    aux->next=fl;
    aux->prev=fl->prev;
    aux->size=limit;

   

    if(fl->prev==NULL){
    
        fl->prev=aux;

        h->flist->head=aux;

    }else{

        
        fl->prev->next=aux;
        fl->prev=aux;


    }


}







