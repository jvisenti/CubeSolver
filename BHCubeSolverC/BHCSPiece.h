//
//  BHCSPiece.h
//  XCodeLab2
//
//  Created by John Visentin on 3/29/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#ifndef XCodeLab2_BHCSPiece_h
#define XCodeLab2_BHCSPiece_h

#include "BHCS.h"

extern const int kBHCSPieceNumRotations;

typedef struct _BHCSPiece
{
    BHCSVector3 position;
    BHCSVector3 axisRotation;
    BHCSVector3 size;
    
    size_t numCubes;
    BHCSVector3 *cubes;
} BHCSPiece;

BHCSPiece* BHCSPieceCreateWithCubes(BHCSVector3 *cubes, size_t n);
void BHCSPieceFree(BHCSPiece *piece);

void BHCSPieceSetRotationIndex(BHCSPiece *piece, int index);
void BHCSPieceResetRotation(BHCSPiece *piece);

int* BHCSPieceGenerateUniqueRotationIndices(BHCSPiece *piece, int *n);

BHCSQuaternion BHCSPieceGetQuaternionRotation(BHCSPiece *piece);
BHCSVector3 BHCSPieceRotateVector3(BHCSPiece *piece, BHCSVector3 v);

#endif
