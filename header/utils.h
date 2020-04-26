//
// Created by james on 18/04/2020.
//

#ifndef TAVRN_UTILS_H
#define TAVRN_UTILS_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

typedef struct ListNode{
    void * data;
    struct ListNode * next;
}ListNode;

typedef struct List{
    struct ListNode * startNode;
    struct ListNode * lastNode;
    uint32_t size;
}List;

//
//typedef struct List{
//    void ** array;
//    size_t used;
//    size_t size;
//} List;

typedef struct Map{
    List keys;
    List values;
}Map;

void initList(List * list);
void addList(List * list, void * element);
void * getList(List * list, uint32_t index);
void insertList(List * list, void * element, uint32_t index);
void removeList(List * list, uint32_t index);
void removeTokenList(List * list, uint32_t index);
void freeList(List * list);
void freeTokenList(List * list);

void initMap(Map * map);
void addMap(Map * map, void * key, void * value);
void * getMap(Map * map, void * key, size_t keyDatasize);
void * getStringMap(Map * map, char * key);
void freeMap(Map * map);

int baseToDecimal(char *str, int base);

#endif //TAVRN_UTILS_H
