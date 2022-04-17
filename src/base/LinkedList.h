//-------------------------------------------------------------------------------
// Copyright (c) 2020-2022 Fábio Pichler
/*-------------------------------------------------------------------------------

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-------------------------------------------------------------------------------*/

#pragma once

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// Doubly linked list

typedef struct LinkedList LinkedList;
typedef struct LinkedListNode LinkedListNode;

LinkedList *LinkedList_New();
void LinkedList_Delete(LinkedList * const self);

size_t LinkedList_GetSize(LinkedList * const self);
LinkedListNode *LinkedList_GetFirst(LinkedList * const self);
LinkedListNode *LinkedList_GetLast(LinkedList * const self);

void *LinkedList_GetValuePtr(LinkedList * const self, LinkedListNode *node);
const char *LinkedList_GetValueString(LinkedList * const self, LinkedListNode *node);
double LinkedList_GetValueNumber(LinkedList * const self, LinkedListNode *node);
LinkedListNode *LinkedList_GetPrev(LinkedList * const self, LinkedListNode *node);
LinkedListNode *LinkedList_GetNext(LinkedList * const self, LinkedListNode *node);

void LinkedList_Clear(LinkedList * const self);
void LinkedList_PushPtr(LinkedList *self, void *value);
void LinkedList_PushString(LinkedList *self, const char *value);
void LinkedList_PushNumber(LinkedList *self, double value);
void LinkedList_Prev(LinkedList * const self, LinkedListNode **node);
void LinkedList_Next(LinkedList * const self, LinkedListNode **node);

void LinkedList_Remove(LinkedList * const self, LinkedListNode **node);
void LinkedList_RemoveFromValuePtr(LinkedList * const self, void *value);
void LinkedList_RemoveFromValueString(LinkedList * const self, const char *value);
void LinkedList_RemoveFromValueNumber(LinkedList * const self, double value);

#ifdef __cplusplus
}
#endif
