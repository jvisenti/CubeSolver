//
//  BHDancingLinks.c
//  BHCubeSolver
//
//  Created by John Visentin on 3/10/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "BHDancingLinks.h"

#pragma mark - definitions

void BHDancingLinksUnlinkColumn(BHDancingLinks *links, BHDancingLinksColumn *col);
void BHDancingLinksRelinkColumn(BHDancingLinks *links, BHDancingLinksColumn *col);

void BHDancingLinksUnlinkRow(BHDancingLinks *links, BHDancingLinksRow *row);
void BHDancingLinksRelinkRow(BHDancingLinks *links, BHDancingLinksRow *row);

#pragma mark - public

BHDancingLinks* BHDancingLinksCreate(size_t numCols)
{
    BHDancingLinks *links = malloc(sizeof(BHDancingLinks));
    
    links->cols = NULL;
    links->rows = NULL;
    
    links->colCount = 0;
    links->rowCount = 0;
    
    for (size_t i = 0; i < numCols; i++)
    {
        BHDancingLinksAddColumn(links);
    }
    
    return links;
}

void BHDancingLinksFree(BHDancingLinks *links)
{
    for (BHDancingLinksColumn *col = links->cols; col != NULL; col = links->cols)
    {
        links->cols = col->next;
        free(col);
    }
    
    for (BHDancingLinksRow *row = links->rows; row != NULL; row = links->rows)
    {
        for (BHDancingLinksNode *node = row->head; node != NULL; node = row->head)
        {
            row->head = node->next;
            free(node);
        }
        
        links->rows = row->below;
        free(row);
    }
    
    free(links);
}

BHDancingLinksColumn* BHDancingLinksAddColumn(BHDancingLinks *links)
{
    BHDancingLinksColumn *col = malloc(sizeof(BHDancingLinksColumn));
    
    col->count = 0;
    col->head = NULL;
    col->tail = NULL;
    
    col->prev = NULL;
    col->next = links->cols;
    
    if (links->cols != NULL)
    {
        links->cols->prev = col;
    }
    
    links->cols = col;
    links->colCount++;
    
    return col;
}

BHDancingLinksRow* BHDancingLinksAddRow(BHDancingLinks *links)
{
    BHDancingLinksRow *row = malloc(sizeof(BHDancingLinksRow));
    
    row->count = 0;
    row->data = NULL;
    row->head = NULL;
    row->tail = NULL;
    
    row->above = NULL;
    row->below = NULL;
    
    row->above = NULL;
    row->below = links->rows;
    
    if (links->rows != NULL)
    {
        links->rows->above = row;
    }
    
    links->rows = row;
    links->rowCount++;
    
    return row;
}

void BHDancingLinksAddNode(BHDancingLinksColumn *col, BHDancingLinksRow *row)
{
    BHDancingLinksNode *node = malloc(sizeof(BHDancingLinksNode));
    
    node->column = col;
    node->row = row;
    
    node->prev = row->tail;
    node->next = NULL;
    
    if (row->head == NULL)
    {
        row->head = node;
    }
    
    if (row->tail != NULL)
    {
        row->tail->next = node;
    }
    
    row->tail = node;
    row->count++;
    
    node->above = NULL;
    node->below = col->head;
    
    if (col->tail == NULL)
    {
        col->tail = node;
    }
    
    if (col->head != NULL)
    {
        col->head->above = node;
    }
    
    col->head = node;
    col->count++;
}

void BHDancingLinksInsertRows(BHDancingLinks *links, BHDancingLinksRow *rows)
{
    BHDancingLinksRow *row = rows;
    
    for (BHDancingLinksNode *temp = row->head; temp != NULL; temp = temp->next)
    {
        BHDancingLinksRelinkColumn(links, temp->column);
    }
    
    while (row != NULL)
    {
        BHDancingLinksRow *tempRow = row->below;
        BHDancingLinksRelinkRow(links, row);
        row = tempRow;
    }
}

BHDancingLinksRow* BHDancingLinksRemoveNode(BHDancingLinks *links, BHDancingLinksNode *node)
{
    BHDancingLinksRow *row = node->row;
    BHDancingLinksUnlinkRow(links, row);
    row->above = NULL;
    row->below = NULL;
    
    BHDancingLinksRow *rowTail = row;
    
    // remove all conflicting cols
    for (BHDancingLinksNode *temp = row->head; temp != NULL; temp = temp->next)
    {
        // remove all conflicting rows
        for (BHDancingLinksNode *temp2 = temp->column->head; temp2 != NULL; temp2 = temp2->below)
        {            
            BHDancingLinksUnlinkRow(links, temp2->row);
            
            temp2->row->above = rowTail;
            temp2->row->below = NULL;
            
            rowTail->below = temp2->row;
            rowTail = temp2->row;
        }
        
        BHDancingLinksUnlinkColumn(links, temp->column);
    }
    
    return row;
}

BHDancingLinksColumn* BHDancingLinksGetColumns(BHDancingLinks *links)
{
    return links->cols;
}

BHDancingLinksColumn* BHDancingLinksGetLeastColumn(BHDancingLinks *links)
{
    BHDancingLinksColumn *minCol = links->cols;
    
    for (BHDancingLinksColumn *temp = minCol->next; temp != NULL; temp = temp->next)
    {
        if (temp->count < minCol->count)
        {
            minCol = temp;
        }
    }
    
    return minCol;
}

BHDancingLinksRow* BHDancingLinksGetRows(BHDancingLinks *links)
{
    return links->rows;
}

size_t BHDancingLinksGetColumnCount(BHDancingLinks *links)
{
    return links->colCount;
}

size_t BHDancingLinksGetRowCount(BHDancingLinks *links)
{
    return links->rowCount;
}

BHDancingLinksColumn* BHDancingLinksColumnGetNext(BHDancingLinksColumn *col)
{
    return col->next;
}

BHDancingLinksColumn* BHDancingLinksColumnGetPrev(BHDancingLinksColumn *col)
{
    return col->prev;
}

BHDancingLinksNode** BHDancingLinksColumnGetNodes(BHDancingLinksColumn *col)
{
    BHDancingLinksNode **nodes = malloc(col->count * sizeof(BHDancingLinksNode *));
    
    int i = 0;
    for (BHDancingLinksNode *node = col->head; node != NULL; node = node->below)
    {
        nodes[i] = node;
        i++;
    }
    
    return nodes;
}

size_t BHDancingLinksColumnGetRowCount(BHDancingLinksColumn *col)
{
    return col->count;
}

void BHDancingLinksRowSetHeaderData(BHDancingLinksRow *row, const void *data)
{
    row->data = data;
}

const void* BHDancingLinksRowGetHeaderData(BHDancingLinksRow *row)
{
    return row->data;
}

BHDancingLinksRow* BHDancingLinksRowGetAbove(BHDancingLinksRow *row)
{
    return row->above;
}

BHDancingLinksRow* BHDancingLinksRowGetBelow(BHDancingLinksRow *row)
{
    return row->below;
}

BHDancingLinksNode** BHDancingLinksRowGetNodes(BHDancingLinksRow *row)
{
    BHDancingLinksNode **nodes = malloc(row->count * sizeof(BHDancingLinksNode *));
    
    int i = 0;
    for (BHDancingLinksNode *node = row->head; node != NULL; node = node->next)
    {
        nodes[i] = node;
        i++;
    }
    
    return nodes;
}

size_t BHDancingLinksRowGetColCount(BHDancingLinksRow *row)
{
    return row->count;
}

BHDancingLinksColumn* BHDancingLinksNodeGetColumn(BHDancingLinksNode *node)
{
    return node->column;
}

BHDancingLinksRow* BHDancingLinksNodeGetRow(BHDancingLinksNode *node)
{
    return node->row;
}

BHDancingLinksNode* BHDancingLinksNodeGetNext(BHDancingLinksNode *node)
{
    return node->next;
}

BHDancingLinksNode* BHDancingLinksNodeGetPrev(BHDancingLinksNode *node)
{
    return node->prev;
}

BHDancingLinksNode* BHDancingLinksNodeGetAbove(BHDancingLinksNode *node)
{
    return node->above;
}

BHDancingLinksNode* BHDancingLinksNodeGetBelow(BHDancingLinksNode *node)
{
    return node->below;
}

#pragma mark - private

void BHDancingLinksUnlinkColumn(BHDancingLinks *links, BHDancingLinksColumn *col)
{
    if (col == links->cols)
    {
        links->cols = col->next;
    }
    
    if (col->next != NULL)
    {
        col->next->prev = col->prev;
    }
    
    if (col->prev != NULL)
    {
        col->prev->next = col->next;
    }
    
    for (BHDancingLinksNode *temp = col->head; temp != NULL; temp = temp->next)
    {
        if (temp == temp->row->head)
        {
            temp->row->head = temp->next;
        }
        
        if (temp == temp->row->tail)
        {
            temp->row->tail = temp->prev;
        }
        
        if (temp->prev != NULL)
        {
            temp->prev->next = temp->next;
        }
        
        if (temp->next != NULL)
        {
            temp->next->prev = temp->prev;
        }
        
        temp->row->count--;
    }
    
    links->colCount--;
}

void BHDancingLinksRelinkColumn(BHDancingLinks *links, BHDancingLinksColumn *col)
{
    if (links->cols == NULL || col->next == links->cols)
    {
        links->cols = col;
    }
    
    if (col->prev != NULL)
    {
        col->prev->next = col;
    }
    
    if (col->next != NULL)
    {
        col->next->prev = col;
    }
    
    links->colCount++;
}

void BHDancingLinksUnlinkRow(BHDancingLinks *links, BHDancingLinksRow *row)
{
    if (row == links->rows)
    {
        links->rows = row->below;
    }
    
    if (row->above != NULL)
    {
        row->above->below = row->below;
    }
    
    if (row->below != NULL)
    {
        row->below->above = row->above;
    }
    
    for (BHDancingLinksNode *temp = row->head; temp != NULL; temp = temp->next)
    {
        if (temp == temp->column->head)
        {
            temp->column->head = temp->below;
        }
        
        if (temp == temp->column->tail)
        {
            temp->column->tail = temp->above;
        }
        
        if (temp->above != NULL)
        {
            temp->above->below = temp->below;
        }
        
        if (temp->below != NULL)
        {
            temp->below->above = temp->above;
        }
        
        temp->column->count--;
    }
    
    links->rowCount--;
}

void BHDancingLinksRelinkRow(BHDancingLinks *links, BHDancingLinksRow *row)
{
    row->above = NULL;
    row->below = links->rows;
    
    if (row->below != NULL)
    {
        row->below->above = row;
    }
    
    for (BHDancingLinksNode *temp = row->head; temp != NULL; temp = temp->next)
    {
        temp->above = NULL;
        temp->below = temp->column->head;
        
        if (temp->column->head != NULL)
        {
            temp->column->head->above = temp;
        }
        
        temp->column->head = temp;
        
        if (temp->column->tail == NULL)
        {
            temp->column->tail = temp;
        }
        
        temp->column->count++;
    }
    
    links->rows = row;
    
    links->rowCount++;
}
