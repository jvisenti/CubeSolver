//
//  stack.c
//  
//
//  Created by Rob Visentin on 2/11/14.
//
// Simple stack implementation for holding point2D

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "RVStack.h"

struct _RVStack
{
    void *stack;        // array to hold elements
    size_t count;       // number of elements currently on the stack

    size_t elementSize; // size of each element
    size_t stackLength; // length of stack array, which doubles as needed
};

#pragma mark - init/destroy

RVStack* RVStackCreate(size_t startSize, size_t elementSize)
{
    assert(startSize > 0);
    
    RVStack *stack = malloc(sizeof(RVStack));
    
    if (stack == NULL)
        return NULL;
    
    stack->stack = malloc(startSize * elementSize);
    
    if (stack->stack == NULL)
    {
        free(stack);
        return NULL;
    }
    
    stack->count = 0;
    
    stack->elementSize = elementSize;
    stack->stackLength = startSize;
    
    return stack;
}

void RVStackFree(RVStack *stack)
{
    assert(stack);
    
    free(stack->stack);
    free(stack);
}

#pragma mark - pushing

void RVStackPush(RVStack *stack, const void *data)
{
    assert(stack);
    
    if (stack->count == stack->stackLength) // double stack space if needed
    {
        stack->stack = realloc(stack->stack, 2 * stack->stackLength * stack->elementSize);
        assert(stack->stack);
        
        stack->stackLength = 2 * stack->stackLength;
    }
    
    memcpy((char *)stack->stack + stack->elementSize * stack->count, data, stack->elementSize);
    
    stack->count++;
}

void RVStackPushN(RVStack *stack, const void *array, size_t n)
{
    assert(stack);
    
    int i;
    for (i = 0; i < n; i++)
    {
        RVStackPush(stack, array + i);
    }
}

#pragma mark - peeking

void* const RVStackPeek(const RVStack *stack)
{
    return RVStackPeekI(stack, 0);
}

void* const RVStackPeekI(const RVStack *stack, unsigned long i)
{
    assert(stack && i >= 0 && i < stack->count);
    
    return (char *)stack->stack + stack->elementSize * (stack->count-1-i);
}

void* const* RVStackPeekN(const RVStack *stack, size_t n)
{
    assert(stack && n <= stack->count);
    
    void **ret = malloc(n * sizeof(void*));
    assert(ret);
    
    unsigned long i;
    for (i = 0; i < n; i++)
    {
        ret[i] = RVStackPeekI(stack, stack->count-1 - i);
    }
    
    return ret;
}

#pragma mark - poppping

void* RVStackPop(RVStack *stack)
{
    assert(stack && !RVStackIsEmpty(stack));
    
    return RVStackPopN(stack, 1);
}

void* RVStackPopN(RVStack *stack, size_t n)
{
    assert(stack && n <= stack->count);
    
    void *ret = malloc(n * stack->elementSize);
    assert(ret);
    
    unsigned long i;
    for (i = 0; i < n; i++)
    {
        RVStackRemove(stack);

        memcpy((char *)ret + stack->elementSize * (n-1 - i), (char *)stack->stack + stack->elementSize * stack->count, stack->elementSize);
    }
    
    return ret;
}

#pragma mark - remove

void RVStackRemove(RVStack *stack)
{
    assert(stack && stack->count > 0);
    
    RVStackRemoveN(stack, 1);
}

void RVStackRemoveN(RVStack *stack, size_t n)
{
    assert(stack && n <= stack->count);
    stack->count -= n;
}

#pragma mark - utils

size_t RVStackCount(const RVStack *stack)
{
    assert(stack);
    
    return stack->count;
}

int RVStackIsEmpty(const RVStack *stack)
{
    assert(stack);
    
    return stack->count == 0;
}