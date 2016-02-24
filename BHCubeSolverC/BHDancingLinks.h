//
//  BHDancingLinks.h
//  BHCubeSolver
//
//  Created by John Visentin on 3/10/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#ifndef BHCubeSolver_BHDancingLinks_h
#define BHCubeSolver_BHDancingLinks_h

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _BHDancingLinksNode BHDancingLinksNode;
typedef struct _BHDancingLinksColumn BHDancingLinksColumn;
typedef struct _BHDancingLinksRow BHDancingLinksRow;
typedef struct _BHDancingLinks BHDancingLinks;
    
struct _BHDancingLinksColumn
{
    size_t count;
    
    BHDancingLinksNode *head;
    BHDancingLinksNode *tail;
    
    BHDancingLinksColumn *prev;
    BHDancingLinksColumn *next;
};

struct _BHDancingLinksRow
{
    size_t count;
    
    const void *data;
    
    BHDancingLinksNode *head;
    BHDancingLinksNode *tail;
    
    BHDancingLinksRow *above;
    BHDancingLinksRow *below;
};

struct _BHDancingLinksNode
{
    BHDancingLinksColumn *column;
    BHDancingLinksRow *row;
    
    BHDancingLinksNode *above;
    BHDancingLinksNode *below;
    
    BHDancingLinksNode *prev;
    BHDancingLinksNode *next;
};

struct _BHDancingLinks
{
    BHDancingLinksColumn *cols;
    BHDancingLinksRow *rows;
    
    size_t colCount;
    size_t rowCount;
};

BHDancingLinks* BHDancingLinksCreate(size_t numCols);
void BHDancingLinksFree(BHDancingLinks *links);
    
BHDancingLinksColumn* BHDancingLinksAddColumn(BHDancingLinks *links);
BHDancingLinksRow* BHDancingLinksAddRow(BHDancingLinks *links);
void BHDancingLinksAddNode(BHDancingLinksColumn *col, BHDancingLinksRow *row);
    
void BHDancingLinksInsertRows(BHDancingLinks *links, BHDancingLinksRow *rows);
BHDancingLinksRow* BHDancingLinksRemoveNode(BHDancingLinks *links, BHDancingLinksNode *node);
    
BHDancingLinksColumn* BHDancingLinksGetColumns(BHDancingLinks *links);
BHDancingLinksColumn* BHDancingLinksGetLeastColumn(BHDancingLinks *links);
BHDancingLinksRow* BHDancingLinksGetRows(BHDancingLinks *links);
size_t BHDancingLinksGetColumnCount(BHDancingLinks *links);
size_t BHDancingLinksGetRowCount(BHDancingLinks *links);
    
BHDancingLinksColumn* BHDancingLinksColumnGetNext(BHDancingLinksColumn *col);
BHDancingLinksColumn* BHDancingLinksColumnGetPrev(BHDancingLinksColumn *col);
BHDancingLinksNode** BHDancingLinksColumnGetNodes(BHDancingLinksColumn *col);
size_t BHDancingLinksColumnGetRowCount(BHDancingLinksColumn *col);
    
void BHDancingLinksRowSetHeaderData(BHDancingLinksRow *row, const void *data);
const void* BHDancingLinksRowGetHeaderData(BHDancingLinksRow *row);
BHDancingLinksRow* BHDancingLinksRowGetAbove(BHDancingLinksRow *row);
BHDancingLinksRow* BHDancingLinksRowGetBelow(BHDancingLinksRow *row);
BHDancingLinksNode** BHDancingLinksRowGetNodes(BHDancingLinksRow *row);
size_t BHDancingLinksRowGetColCount(BHDancingLinksRow *row);
    
BHDancingLinksColumn* BHDancingLinksNodeGetColumn(BHDancingLinksNode *node);
BHDancingLinksRow* BHDancingLinksNodeGetRow(BHDancingLinksNode *node);
BHDancingLinksNode* BHDancingLinksNodeGetNext(BHDancingLinksNode *node);
BHDancingLinksNode* BHDancingLinksNodeGetPrev(BHDancingLinksNode *node);
BHDancingLinksNode* BHDancingLinksNodeGetAbove(BHDancingLinksNode *node);
BHDancingLinksNode* BHDancingLinksNodeGetBelow(BHDancingLinksNode *node);
    
#ifdef __cplusplus
}
#endif

#endif
