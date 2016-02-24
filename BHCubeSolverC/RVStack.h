//
//  stack.h
//  
//
//  Created by Rob Visentin on 2/11/14.
//
// Simple stack implementation for holding arbitrary data types
//

#ifndef _stack_h
#define _stack_h

typedef struct _RVStack RVStack;

/* returns a new stack allocated with the given start size. 
    @param elementSize the size of each element that the stack will store.
    @note stack size will double automatically as needed. */
RVStack* RVStackCreate(size_t startSize, size_t elementSize);

/* free all memory allocated for the stack, including the stack itself. */
void RVStackFree(RVStack *stack);

/* pushes an element onto the top of the stack. */
void RVStackPush(RVStack *stack, const void *data);

/* pushes the first n elements in the given array onto the top of the stack. */
void RVStackPushN(RVStack *stack, const void *array, size_t n);

/* returns the element at the top of the stack, but leaves it on the stack. */
void* const RVStackPeek(const RVStack *stack);

/* returns the ith element from the top of the stack, but leaves it in the stack. */
void* const RVStackPeekI(const RVStack *stack, unsigned long i);

/* returns the first n elements from the top of the stack, but leaves them on the stack. 
    @note returned value must be freed. */
void* const* RVStackPeekN(const RVStack *stack, size_t n);

/* returns the element at the top of the stack and removes it from the stack. 
    @note returned value must be freed. */
void* RVStackPop(RVStack *stack);

/* returns the first n elements at the top of the stack and removes them from the stack. 
    @note returned value must be freed. */
void* RVStackPopN(RVStack *stack, size_t n);

/* removes the element at the top of the stack. */
void RVStackRemove(RVStack *stack);

/* removes the first n elements at the top of the stack. */
void RVStackRemoveN(RVStack *stack, size_t n);

size_t RVStackCount(const RVStack *stack);
int RVStackIsEmpty(const RVStack *stack);

#endif
