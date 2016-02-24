//
//  BHCSWorld.c
//  BHCubeSolver
//
//  Created by John Visentin on 3/29/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#include <stdio.h>
#include "BHCSWorld.h"

#include "BHDancingLinks.h"

typedef struct _BHCSRowInfo
{
    int pieceID;
    BHCSVector3 position;
    BHCSVector3 axisRotation;
} BHCSRowInfo;

BHDancingLinks* BHDancingLinksCreateWithWorld(BHCSWorld *world);
void BHCSWorldSolveDancingLinks(BHDancingLinks *matrix, RVStack *solution, int numPieces, int *numSolutions);

int intArrayContains(int *array, int n, int val);

BHCSWorld* BHCSWorldCreate()
{
    BHCSWorld *world = malloc(sizeof(BHCSWorld));
    
    // TODO: allow reading in any file
    
    world->size = BHCSVector3Make(4, 4, 4);
    world->numPieces = 12;
    
    world->pieces = malloc(world->numPieces * sizeof(BHCSPiece *));

    BHCSVector3 *cubes = malloc(6 * sizeof(BHCSVector3));
    
    cubes[0] = BHCSVector3Make(0, 0, 0);
    cubes[1] = BHCSVector3Make(0, 0, 1);
    cubes[2] = BHCSVector3Make(1, 0, 0);
    cubes[3] = BHCSVector3Make(1, 0, 1);
    cubes[4] = BHCSVector3Make(0, 1, 0);
    BHCSPiece *p1 = BHCSPieceCreateWithCubes(cubes, 5);
    world->pieces[0] = p1;
    
    cubes[0] = BHCSVector3Make(0, 0, 0);
    cubes[1] = BHCSVector3Make(0, 1, 0);
    cubes[2] = BHCSVector3Make(0, 2, 0);
    cubes[3] = BHCSVector3Make(1, 0, 0);
    cubes[4] = BHCSVector3Make(1, 0, 1);
    cubes[5] = BHCSVector3Make(2, 0, 0);
    BHCSPiece *p2 = BHCSPieceCreateWithCubes(cubes, 6);
    world->pieces[1] = p2;
    
    cubes[0] = BHCSVector3Make(0, 0, 1);
    cubes[1] = BHCSVector3Make(1, 0, 0);
    cubes[2] = BHCSVector3Make(1, 1, 0);
    cubes[3] = BHCSVector3Make(1, 0, 1);
    cubes[4] = BHCSVector3Make(2, 0, 1);
    BHCSPiece *p3 = BHCSPieceCreateWithCubes(cubes, 5);
    world->pieces[2] = p3;
    
    cubes[0] = BHCSVector3Make(0, 0, 1);
    cubes[1] = BHCSVector3Make(1, 0, 1);
    cubes[2] = BHCSVector3Make(2, 0, 0);
    cubes[3] = BHCSVector3Make(2, 0, 1);
    cubes[4] = BHCSVector3Make(3, 0, 0);
    BHCSPiece *p4 = BHCSPieceCreateWithCubes(cubes, 5);
    world->pieces[3] = p4;
    
    cubes[0] = BHCSVector3Make(0, 0, 0);
    cubes[1] = BHCSVector3Make(0, 0, 1);
    cubes[2] = BHCSVector3Make(0, 0, 2);
    cubes[3] = BHCSVector3Make(1, 0, 0);
    cubes[4] = BHCSVector3Make(2, 0, 0);
    BHCSPiece *p5 = BHCSPieceCreateWithCubes(cubes, 5);
    world->pieces[4] = p5;
    
    cubes[0] = BHCSVector3Make(0, 0, 0);
    cubes[1] = BHCSVector3Make(0, 1, 0);
    cubes[2] = BHCSVector3Make(0, 0, 1);
    cubes[3] = BHCSVector3Make(1, 0, 1);
    cubes[4] = BHCSVector3Make(1, 0, 2);
    cubes[5] = BHCSVector3Make(2, 0, 1);
    BHCSPiece *p6 = BHCSPieceCreateWithCubes(cubes, 6);
    world->pieces[5] = p6;
    
    cubes[0] = BHCSVector3Make(0, 0, 0);
    cubes[1] = BHCSVector3Make(0, 0, 1);
    cubes[2] = BHCSVector3Make(0, 0, 2);
    cubes[3] = BHCSVector3Make(1, 0, 2);
    cubes[4] = BHCSVector3Make(2, 0, 2);
    cubes[5] = BHCSVector3Make(2, 1, 2);
    BHCSPiece *p7 = BHCSPieceCreateWithCubes(cubes, 6);
    world->pieces[6] = p7;
    
    cubes[0] = BHCSVector3Make(0, 0, 0);
    cubes[1] = BHCSVector3Make(0, 0, 1);
    cubes[2] = BHCSVector3Make(0, 1, 1);
    cubes[3] = BHCSVector3Make(1, 0, 0);
    cubes[4] = BHCSVector3Make(2, 0, 0);
    BHCSPiece *p8 = BHCSPieceCreateWithCubes(cubes, 5);
    world->pieces[7] = p8;
    
    /* yellow zig zag */
    cubes[0] = BHCSVector3Make(0, 1, 1);
    cubes[1] = BHCSVector3Make(1, 0, 0);
    cubes[2] = BHCSVector3Make(1, 1, 0);
    cubes[3] = BHCSVector3Make(1, 1, 1);
    cubes[4] = BHCSVector3Make(2, 0, 0);
    BHCSPiece *p9 = BHCSPieceCreateWithCubes(cubes, 5);
    world->pieces[8] = p9;
    
    cubes[0] = BHCSVector3Make(0, 0, 0);
    cubes[1] = BHCSVector3Make(1, 0, 0);
    cubes[2] = BHCSVector3Make(2, 0, 0);
    cubes[3] = BHCSVector3Make(2, 0, 1);
    cubes[4] = BHCSVector3Make(3, 0, 0);
    BHCSPiece *p10 = BHCSPieceCreateWithCubes(cubes, 5);
    world->pieces[9] = p10;
    
    cubes[0] = BHCSVector3Make(0, 0, 0);
    cubes[1] = BHCSVector3Make(1, 0, 0);
    cubes[2] = BHCSVector3Make(1, 1, 0);
    cubes[3] = BHCSVector3Make(1, 1, 1);
    cubes[4] = BHCSVector3Make(1, 2, 1);
    BHCSPiece *p11 = BHCSPieceCreateWithCubes(cubes, 5);
    world->pieces[10] = p11;
    
    cubes[0] = BHCSVector3Make(0, 1, 1);
    cubes[1] = BHCSVector3Make(1, 0, 0);
    cubes[2] = BHCSVector3Make(1, 1, 0);
    cubes[3] = BHCSVector3Make(1, 2, 0);
    cubes[4] = BHCSVector3Make(1, 1, 1);
    cubes[5] = BHCSVector3Make(2, 1, 0);
    BHCSPiece *p12 = BHCSPieceCreateWithCubes(cubes, 6);
    world->pieces[11] = p12;
    
    free(cubes);
    
    return world;
}

void BHCSWorldFree(BHCSWorld *world)
{
    if (world)
    {
        int i;
        for (i = 0; i < world->numPieces; i++)
        {
            BHCSPieceFree(world->pieces[i]);
        }
        
        free(world->pieces);
        free(world);
    }
}

int BHCSWorldPointInside(BHCSWorld *world, BHCSVector3 p)
{
    return (p.x >= 0 && p.x < world->size.w
            && p.y >= 0 && p.y < world->size.h
            && p.z >= 0 && p.z < world->size.d);
}

int BHCSWorldPieceInside(BHCSWorld *world, BHCSPiece *piece)
{
    int allInside = 1;
    
    int i;
    for (i = 0; i < piece->numCubes && allInside; i++)
    {
        BHCSVector3 cubePos = BHCSVector3Add(piece->position, BHCSPieceRotateVector3(piece, piece->cubes[i]));
        allInside = BHCSWorldPointInside(world, cubePos);
    }
    
    return allInside;
}

int BHCSWorldCubeIndexFromPoint(BHCSWorld *world, BHCSVector3 p)
{
    return (world->size.w * world->size.d * p.y) + (world->size.w * p.z) + p.x;
}

int BHCSWorldFindAllSolutions(BHCSWorld *world)
{
    BHDancingLinks *matrix = BHDancingLinksCreateWithWorld(world);
    RVStack *solStack = RVStackCreate(12, sizeof(BHCSRowInfo));
    
    int numSol = 0;
    BHCSWorldSolveDancingLinks(matrix, solStack, world->numPieces, &numSol);
    
    BHDancingLinksRow *rows = BHDancingLinksGetRows(matrix);
    for (BHDancingLinksRow *row = rows; row != NULL; row = BHDancingLinksRowGetBelow(row))
    {
        free((void *)BHDancingLinksRowGetHeaderData(row));
        BHDancingLinksRowSetHeaderData(row, NULL);
    }
    
    BHDancingLinksFree(matrix);
    RVStackFree(solStack);
    
    return numSol;
}

#pragma mark - private methods

BHDancingLinks* BHDancingLinksCreateWithWorld(BHCSWorld *world)
{
    size_t cols = world->size.w * world->size.h * world->size.d + world->numPieces;
    
    BHDancingLinks *links = BHDancingLinksCreate(cols);
    
    int p, x, y, z, r, i;
    for (p = 0; p < world->numPieces; p++)
    {
        BHCSPiece *piece = world->pieces[p];
        
        int numRotations;
        int *rotationIndices = BHCSPieceGenerateUniqueRotationIndices(piece, &numRotations);
                
        for (x = 0; x < world->size.w; x++)
        {
            for (y = 0; y < world->size.h; y++)
            {
                for (z = 0; z < world->size.d; z++)
                {
                    BHCSVector3 point = BHCSVector3Make(x, y, z);
                    
                    for (r = 0; r < numRotations; r++)
                    {
                        BHCSPieceSetRotationIndex(piece, rotationIndices[r]);
                        piece->position = point;
                        
                        if (BHCSWorldPieceInside(world, piece))
                        {
                            BHCSRowInfo *rowInfo = malloc(sizeof(BHCSRowInfo));
                            rowInfo->pieceID = p+1;
                            rowInfo->position = point;
                            rowInfo->axisRotation = piece->axisRotation;
                            
                            BHDancingLinksRow *row = BHDancingLinksAddRow(links);
                            BHDancingLinksRowSetHeaderData(row, rowInfo);
                            
                            BHDancingLinksColumn *col = BHDancingLinksGetColumns(links);
                            
                            int *indices = malloc(piece->numCubes * sizeof(int));
                            
                            for (i = 0; i < piece->numCubes; i++)
                            {
                                BHCSVector3 cubePos = BHCSVector3Add(point, BHCSPieceRotateVector3(piece, piece->cubes[i]));
                                indices[i] = BHCSWorldCubeIndexFromPoint(world, cubePos);
                            }
                            
                            for (i = 0; i < cols; i++)
                            {
                                if (i == p || intArrayContains(indices, piece->numCubes, i-world->numPieces))
                                {
                                    BHDancingLinksAddNode(col, row);
                                }
                                
                                col = BHDancingLinksColumnGetNext(col);
                            }
                            
                            free(indices);
                        }
                    }
                }
            }
        }
        
        free(rotationIndices);
    }
    
    return links;
}

void BHCSWorldSolveDancingLinks(BHDancingLinks *matrix, RVStack *solution, int numPieces, int *numSolutions)
{
    if (matrix->rowCount == 0)
    {
        if (RVStackCount(solution) == numPieces)
        {
            *numSolutions = *numSolutions + 1;
            
            int i;
            for (i = 0 ; i < numPieces; i++)
            {
                BHCSRowInfo *rowInfo = (BHCSRowInfo *)RVStackPeekI(solution, i);
                printf("piece %i \tat (%i, %i, %i) rotation (%i, %i, %i)\n", rowInfo->pieceID, rowInfo->position.x, rowInfo->position.y, rowInfo->position.z, rowInfo->axisRotation.x, rowInfo->axisRotation.y, rowInfo->axisRotation.z);
            }
            printf("\n");
        }
        
        return;
    }
    
    BHDancingLinksColumn *leastCol = BHDancingLinksGetLeastColumn(matrix);
    size_t numRows = leastCol->count;
    
    if (numRows == 0)
    {
        return;
    }
    
    BHDancingLinksNode **colNodes = BHDancingLinksColumnGetNodes(leastCol);
    
    int i;
    for (i = 0; i < numRows; i++)
    {
        BHDancingLinksNode *node = colNodes[i];
        BHDancingLinksRow *removedRows = BHDancingLinksRemoveNode(matrix, node);
        
        BHCSRowInfo *rowInfo = (BHCSRowInfo *)(node->row->data);
        RVStackPush(solution, rowInfo);
        
        BHCSWorldSolveDancingLinks(matrix, solution, numPieces, numSolutions);
        
        RVStackRemove(solution);
        
        BHDancingLinksInsertRows(matrix, removedRows);
    }
    
    free(colNodes);
}

int intArrayContains(int *array, int n, int val)
{
    int found = 0;
    
    int i;
    for (i = 0; i < n && !found; i++)
    {
        found = (array[i] == val);
    }
    
    return found;
}
