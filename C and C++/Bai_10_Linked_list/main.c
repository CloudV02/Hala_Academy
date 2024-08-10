#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* node *createNode(int value);  
void push_back(node** array, int value); // them 1 node vao phia sau -> done
void push_front(node **array, int value); // them 1 node vao phia truoc -> done
void pop_back(node **array); // xoa node cuoi cung -> done 
void pop_front(node **array); // xoa node dau tien
int front(node *array); // lay gia tri cua node dau tien
int back(node *array); // lay gia tri cua node cuoi cung
void insert(node **array, int value, int pos); // them 1 node vao mot vi tri bat ky -> done
void delete_list(node **array, int pos); // xoa 1 node tai mot vi tri bat ky -> done
int size(node *array); // lay kich thuoc cua list -> do
int get(node *array, int pos); 
bool empty(node *array); // kiem tra list co rong hay khong
void clear -> xoa tat ca vung nho cap phat -> done
 */
int number_ele = 0;

/*  */
typedef struct Node
{
    int data;
    struct Node *next;

}Node;

/* create a node */
Node * create_Node(int value){
    Node *ptr = (Node *)malloc(sizeof(Node));
    ptr->data = value;
    ptr->next = NULL;
    number_ele++;
    return ptr;
}

/*them vao node phia sau cung*/
void push_back(Node **ptr,int value){

    Node *new_ptr = create_Node(value);

    if(*ptr == NULL){
        *ptr = new_ptr; 
        return;
    }
    /* lay cai node truoc gan gia tri vao node sau */
    else{ // 0 - 1(node -> next = &new_ptr)- 2(new_ptr)
        Node *p = *ptr;
        while (p->next != NULL)
        {
            p = p->next;
        }
        p->next = new_ptr;
        
    }
}

/*them vao node vao dang truoc*/
void push_front(Node **ptr,int value){
    Node *new_Node = (Node *)malloc(sizeof(Node));

    if (*ptr == NULL)
    {
        *ptr = new_Node;
        return;
    }
    else{
        new_Node->data = value;
        new_Node->next = *ptr;
    }
    *ptr = new_Node;
}

/* Xoa not cuoi cung */
void pop_back(Node **ptr){
    
    Node *p = *ptr;

    if(*ptr == NULL){
        return;
    }
    /* Check phan tu dau tien */
    if (p->next == NULL)
    {
        free(p->next);
        *ptr = NULL;
        number_ele--;
        return;
    }

    /* Duyet cac phan tu sau do */ /* 0 -1(p->next = NULL  - 2 */
    while (p->next != NULL)
    {
        if((p->next)->next == NULL){
            free((p->next)->next);
            p->next = NULL;
        }
        else{
            p = p->next;
        }
    }
    number_ele--;
    
}
void insert(Node **ptr, int value, int pos)
{
    Node *p = *ptr;
    /* Neu cai truoc do tro toi NULL thi minh la phan tu cuoi */
    if(pos > number_ele){
        return;
    }
    /* 4 phan tu - 0 - 1 - 2 - 3 -> (4) -> NULL*/
    if (pos == number_ele)
    {
        push_back(ptr,value);
    }
    if(pos < number_ele){
        for(int i = 0; i<pos; ){ /* 0 - 1 -  2() - (2)3 -> NULL*/
            if(pos == 0){
                push_front(ptr,value);
                return;
            }
            if (i == pos - 1) // 0 - 1 (p->next = &2) -  2 3 - 4
            {
                Node *new_Node = create_Node(value);
                /*pos hien tai*/        
                new_Node->data = value;
                new_Node->next = p->next; /*next cua pos dang trc*/

                /* cai p trc do se luu new_Node */
                p->next = new_Node;
            }
            p = p->next;
            i++;
        }
    }
    
    /* Neu truoc va sau deu co gia tri */
}

void delete_node(Node **ptr, int pos)
{
    if(pos>=(number_ele)||*ptr == NULL){
        return;
    }
    if(pos == (number_ele - 1)){ // 4---- 0 1 2 3
        pop_back(ptr);
    }
    if (pos < (number_ele - 1)) /* 0 - 1 - 2 - 3 - 4*/
    {
        Node *p = *ptr;
        Node *temp;
        if (pos == 0)
        {
            *ptr = p->next;
            free(p);
        }
            
        for(int i = 0; i<pos;i++){
            if(i == pos - 1){
                temp = p->next;
                p->next = (p->next)->next; 
                free(temp);
            }
            p = p->next;
        }

        number_ele--;
    }
        
}

void clear(Node **ptr){
    int number = number_ele;
    for (size_t i = 0; i < number; i++)
    {
        pop_back(ptr);
    }
}
int main()
{
    Node *arr = NULL;

    arr = create_Node(5); //0
    push_back(&arr, 10); //1
    push_back(&arr, 15); //2
    push_back(&arr, 20); //4
    //push_front(&arr, 25);
    insert(&arr,30,3); // 3
    delete_node(&arr,2);
    delete_node(&arr,0);
    clear(&arr);
}
