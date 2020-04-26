//
// Created by james on 18/04/2020.
//

#include "header/utils.h"
#include "header/tavrn.h"
#include "header/parser.h"

void initList(List * list){
    list->startNode = NULL;
    list->lastNode = list->startNode;
    list->size = 0;
}

void addList(List * list, void * element){
    ListNode * node = (ListNode*)malloc(sizeof(ListNode));
    node->data = element;
    node->next = NULL;
    if(list->size != 0)
        list->lastNode->next = node;
    else
        list->startNode = node;
    list->lastNode = node;
    list->size++;
}

/**
Get the data from a list index
*/
void * getList(List * list, uint32_t index){
    ListNode * current = list->startNode;
    for(uint32_t i = 0; i<index; i++)
        current=current->next;
    return current->data;
}

void insertList(List * list, void * element, uint32_t index){
    ListNode * current = list->startNode;
    uint32_t i = 0;
    while (current != NULL && i < index) {
        current = current->next;
        i ++;
    }
    // Create the new node
    ListNode * node = (ListNode*)malloc(sizeof(ListNode));
    node->data = element;
    // Insert the new node between the current and the one after
    node->next = current->next;
    current->next = node;
    list->size++;
}


void removeList(List * list, uint32_t index){
    ListNode * current = list->startNode;
    if(index==0){
        list->startNode = list->startNode->next;
        free(current->data);
        free(current);
    }else{
        uint32_t i = 0;
        // Go to the node that is just before the one we are removing
        while (current != NULL && i < index-1) {
            current = current->next;
            i ++;
        }
        ListNode * temp = current->next;
        current->next = current->next->next;
        free(temp->data);
        free(temp);
    }
    list->size--;
}

void removeTokenList(List * list, uint32_t index){
    ListNode * current = list->startNode;
    if(index==0){
        list->startNode = list->startNode->next;
        free(current->data);
        free(current);
    }else{
        uint32_t i = 0;
        // Go to the node that is just before the one we are removing
        while (current != NULL && i < index-1) {
            current = current->next;
            i ++;
        }
        ListNode * temp = current->next;
        current->next = current->next->next;
        Token * token = (Token*)temp->data;
        if(token->type==STRING_LITERAL || token->type==NUMBER_LITERAL) {
            free((char*)token->value.data);
        }
        free(temp->data);
        free(temp);
    }
    list->size--;
}

void freeList(List *list) {
    ListNode * current = list->startNode;
    ListNode * temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
}

void freeTokenList(List *list) {
    ListNode * current = list->startNode;
    ListNode * temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        Token * token = (Token*)temp->data;
        if(token->type==STRING_LITERAL || token->type==NUMBER_LITERAL) {
            free((char*)token->value.data);
        }
        free(temp);
    }
}



void initMap(Map * map){
    initList(&map->keys);
    initList(&map->values);

}
void addMap(Map * map, void * key, void * value){
    addList(&map->keys, key);
    addList(&map->values, value);
}

void * getStringMap(Map * map, char * key){
    for (uint32_t i = 0; i<map->keys.size; i++){
        if(strcmp(getList(&map->keys, i), key)==0)
            return getList(&map->values, i);
    }
    return 0;
}

void * getMap(Map * map, void * key, size_t keyDatasize){
    for (uint32_t i = 0; i<map->keys.size; i++){
        if(getList(&map->keys, i) == key)
            return getList(&map->values, i);
    }
    return 0;
}

void freeMap(Map * map){
    freeList(&map->keys);
    freeList(&map->values);
}

int baseToDecimal(char *str, int base){
    int len = strlen(str);
    int power = 1; // Initialize power of base
    int num = 0;  // Initialize result
    int i;

    // Decimal equivalent is str[len-1]*1 +
    // str[len-1]*base + str[len-1]*(base^2) + ...
    for (i = len - 1; i >= 0; i--)
    {
        // A digit in input number must be
        // less than number's base
        if (str[i]-'0' >= base)
        {
            printf("Invalid Number");
            return -1;
        }

        num += (str[i]-'0') * power;
        power = power * base;
    }
    return num;
}
