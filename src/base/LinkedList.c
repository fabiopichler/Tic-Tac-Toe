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

#include "LinkedList.h"

#include <stdlib.h>
#include <string.h>

struct LinkedListNode
{
    union
    {
        void *ptr;
        const char *string;
        double number;
    } value;

    struct LinkedListNode *prev;
    struct LinkedListNode *next;
};

struct LinkedList
{
    LinkedListNode *firstNode;
    LinkedListNode *lastNode;
    size_t size;
};

LinkedList *LinkedList_New()
{
    LinkedList * const self = malloc(sizeof (LinkedList));

    self->firstNode = NULL;
    self->lastNode = NULL;
    self->size = 0;

    return self;
}

void LinkedList_Delete(LinkedList * const self)
{
    if (!self)
        return;

    LinkedList_Clear(self);

    free(self);
}

size_t LinkedList_GetSize(LinkedList * const self)
{
    return self->size;
}

LinkedListNode *LinkedList_GetFirst(LinkedList * const self)
{
    return self->firstNode;
}

LinkedListNode *LinkedList_GetLast(LinkedList * const self)
{
    return self->lastNode;
}

void *LinkedList_GetValuePtr(LinkedList * const self, LinkedListNode *node)
{
    return node->value.ptr;
}

const char *LinkedList_GetValueString(LinkedList * const self, LinkedListNode *node)
{
    return node->value.string;
}

double LinkedList_GetValueNumber(LinkedList * const self, LinkedListNode *node)
{
    return node->value.number;
}

LinkedListNode *LinkedList_GetPrev(LinkedList * const self, LinkedListNode *node)
{
    return node->prev;
}

LinkedListNode *LinkedList_GetNext(LinkedList * const self, LinkedListNode *node)
{
    return node->next;
}

void LinkedList_Clear(LinkedList * const self)
{
    LinkedListNode *current = self->firstNode;

    while (current)
    {
        LinkedListNode *next = current->next;
        free(current);
        current = next;
    }

    self->firstNode = NULL;
    self->lastNode = NULL;
    self->size = 0;
}

static LinkedListNode *push_next(LinkedListNode *prev)
{
    LinkedListNode *node = malloc(sizeof (LinkedListNode));

    node->prev = prev;
    node->next = NULL;

    return node;
}

static LinkedListNode *push(LinkedList *self)
{
    self->size++;

    if (self->firstNode == NULL)
        return self->firstNode = self->lastNode = push_next(NULL);

    LinkedListNode *last = LinkedList_GetLast(self);

    return self->lastNode = last->next = push_next(last);
}

void LinkedList_PushPtr(LinkedList *self, void *value)
{
    push(self)->value.ptr = value;
}

void LinkedList_PushString(LinkedList *self, const char *value)
{
    push(self)->value.string = value;
}

void LinkedList_PushNumber(LinkedList *self, double value)
{
    push(self)->value.number = value;
}

void LinkedList_Prev(LinkedList * const self, LinkedListNode **node)
{
    *node = (*node)->prev;
}

void LinkedList_Next(LinkedList * const self, LinkedListNode **node)
{
    *node = (*node)->next;
}

void LinkedList_Remove(LinkedList * const self, LinkedListNode **node)
{
    if (*node == NULL)
        return;

    LinkedListNode *current = *node;
    LinkedListNode *prev = current->prev;
    LinkedListNode *next = current->next;

    if (prev)
        prev->next = next;

    if (next)
        next->prev = prev;

    if (self->firstNode == current)
        self->firstNode = next;

    if (self->lastNode == current)
        self->lastNode = prev;

    free(current);

    *node = next;

    self->size--;
}

void LinkedList_RemoveFromValuePtr(LinkedList * const self, void *value)
{
    for (LinkedListNode *iterator = LinkedList_GetFirst(self); iterator != NULL;)
    {
        if (LinkedList_GetValuePtr(self, iterator) == value)
            LinkedList_Remove(self, &iterator);
        else
            LinkedList_Next(self, &iterator);
    }
}

void LinkedList_RemoveFromValueString(LinkedList * const self, const char *value)
{
    for (LinkedListNode *iterator = LinkedList_GetFirst(self); iterator != NULL;)
    {
        if (strcmp(LinkedList_GetValueString(self, iterator), value) == 0)
            LinkedList_Remove(self, &iterator);
        else
            LinkedList_Next(self, &iterator);
    }
}

void LinkedList_RemoveFromValueNumber(LinkedList * const self, double value)
{
    for (LinkedListNode *iterator = LinkedList_GetFirst(self); iterator != NULL;)
    {
        if (LinkedList_GetValueNumber(self, iterator) == value)
            LinkedList_Remove(self, &iterator);
        else
            LinkedList_Next(self, &iterator);
    }
}
