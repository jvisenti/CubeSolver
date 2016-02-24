//
//  BHCSWorld.h
//  BHCubeSolver
//
//  Created by John Visentin on 3/29/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#ifndef BHCubeSolver_BHCSWorld_h
#define BHCubeSolver_BHCSWorld_h

#include "BHCSPiece.h"
#include "RVStack.h"

typedef struct _BHCSWorld
{
    BHCSVector3 size;
    
    BHCSPiece **pieces;
    size_t numPieces;
} BHCSWorld;

BHCSWorld* BHCSWorldCreate();
void BHCSWorldFree(BHCSWorld *world);

int BHCSWorldPointInside(BHCSWorld *world, BHCSVector3 p);
int BHCSWorldPieceInside(BHCSWorld *world, BHCSPiece *piece);

int BHCSWorldCubeIndexFromPoint(BHCSWorld *world, BHCSVector3 p);

int BHCSWorldFindAllSolutions(BHCSWorld *world);

#endif
