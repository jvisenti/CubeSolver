//
//  BHCSPiece.c
//  XCodeLab2
//
//  Created by John Visentin on 3/29/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#include <stdio.h>
#include "BHCSPiece.h"

const int kBHCSPieceNumRotations = 24;

const BHCSVector3 Rotations[kBHCSPieceNumRotations] =
{
    {{0, 0, 0}},
    {{0, 1, 0}},
    {{0, 2, 0}},
    {{0, 3, 0}},
    {{0, 0, 2}},
    {{0, 1, 2}},
    {{0, 2, 2}},
    {{0, 3, 2}},
    
    {{0, 0, 1}},
    {{0, 1, 1}},
    {{0, 2, 1}},
    {{0, 3, 1}},
    {{0, 0, 3}},
    {{0, 1, 3}},
    {{0, 2, 3}},
    {{0, 3, 3}},
    
    {{1, 0, 0}},
    {{1, 0, 1}},
    {{1, 0, 2}},
    {{1, 0, 3}},
    {{3, 0, 0}},
    {{3, 0, 1}},
    {{3, 0, 2}},
    {{3, 0, 3}}
};

BHCSQuaternion BHCSQuaternionMakeWithAxisRotation(BHCSVector3 axisRotation);
int cubeArrayContains(BHCSVector3 **array, int n, BHCSVector3 *cubes, int m);
int vector3ArrayContains(BHCSVector3 *array, int n , BHCSVector3 v);

BHCSPiece* BHCSPieceCreateWithCubes(BHCSVector3 *cubes, size_t n)
{
    BHCSPiece *piece = malloc(sizeof(BHCSPiece));
    
    piece->cubes = malloc(n * sizeof(BHCSVector3));
    
    int i;
    for (i = 0; i < n; i++)
    {
        piece->cubes[i] = cubes[i];
        
        piece->size.w = fmax(piece->size.w, cubes[i].x+1);
        piece->size.h = fmax(piece->size.h, cubes[i].y+1);
        piece->size.d = fmax(piece->size.d, cubes[i].z+1);
    }
    
    piece->numCubes = n;
    
    return piece;
}

void BHCSPieceFree(BHCSPiece *piece)
{
    if (piece)
    {
        free(piece->cubes);
        free(piece);
    }
}

void BHCSPieceSetRotationIndex(BHCSPiece *piece, int index)
{
    piece->axisRotation = Rotations[index];
}

void BHCSPieceResetRotation(BHCSPiece *piece)
{
    piece->axisRotation = BHCSVector3Make(0, 0, 0);
}

int* BHCSPieceGenerateUniqueRotationIndices(BHCSPiece *piece, int *n)
{
    BHCSVector3 **generatedRotations = malloc(kBHCSPieceNumRotations * sizeof(BHCSVector3 *));
    
    *n = 0;
    
    int r, i;
    for (r = 0; r < kBHCSPieceNumRotations; r++)
    {
        BHCSVector3 *rotatedCubes = malloc(piece->numCubes * sizeof(BHCSVector3));
        BHCSQuaternion rotation = BHCSQuaternionMakeWithAxisRotation(Rotations[r]);
        
        BHCSVector3 maxPoint = BHCSVector3Make(piece->size.w-1, piece->size.h-1, piece->size.d-1);
        maxPoint = BHCSQuaternionRotateVector3(rotation, maxPoint);
        
        for (i = 0; i < piece->numCubes; i++)
        {
            rotatedCubes[i] = BHCSQuaternionRotateVector3(rotation, piece->cubes[i]);
            
            rotatedCubes[i].x -= fmin(0, maxPoint.x);
            rotatedCubes[i].y -= fmin(0, maxPoint.y);
            rotatedCubes[i].z -= fmin(0, maxPoint.z);
        }
        
        if (!cubeArrayContains(generatedRotations, r, rotatedCubes, piece->numCubes))
        {
            generatedRotations[r] = rotatedCubes;
            (*n)++;
        }
        else
        {
            generatedRotations[r] = NULL;
        }
    }
    
    int *uniqueRotationIndices = malloc((*n) * sizeof(int));
    
    i = 0;
    for (r = 0; r < kBHCSPieceNumRotations; r++)
    {
        if (generatedRotations[r] != NULL)
        {
            uniqueRotationIndices[i] = r;
            i++;
            
            free(generatedRotations[r]);
        }
    }
    
    free(generatedRotations);
    
    return uniqueRotationIndices;
}

BHCSQuaternion BHCSPieceGetQuaternionRotation(BHCSPiece *piece)
{
    return BHCSQuaternionMakeWithAxisRotation(piece->axisRotation);
}

BHCSVector3 BHCSPieceRotateVector3(BHCSPiece *piece, BHCSVector3 v)
{
    return BHCSQuaternionRotateVector3(BHCSPieceGetQuaternionRotation(piece), v);    
}

#pragma mark - private functions

BHCSQuaternion BHCSQuaternionMakeWithAxisRotation(BHCSVector3 axisRotation)
{
    BHCSQuaternion rx = BHCSQuaternionMakeWithAngleAndAxis(axisRotation.x * M_PI_2, 1.0f, 0.0f, 0.0f);
    BHCSQuaternion ry = BHCSQuaternionMakeWithAngleAndAxis(axisRotation.y * M_PI_2, 0.0f, 1.0f, 0.0f);
    BHCSQuaternion rz = BHCSQuaternionMakeWithAngleAndAxis(axisRotation.z * M_PI_2, 0.0f, 0.0f, 1.0f);
    
    BHCSQuaternion r = BHCSQuaternionMultiply(rz, ry);
    r = BHCSQuaternionMultiply(r, rx);
    
    return r;
}

int cubeArrayContains(BHCSVector3 **array, int n, BHCSVector3 *cubes, int m)
{
    int contains = 0;
    
    int i, j;
    for (i = 0; i < n && !contains; i++)
    {
        if (array[i] != NULL)
        {
            int allContained = 1;
            
            for (j = 0; j < m && allContained; j++)
            {
                allContained = vector3ArrayContains(array[i], m, cubes[j]);
            }
            
            contains = allContained;
        }
    }
    
    return contains;
}

int vector3ArrayContains(BHCSVector3 *array, int n , BHCSVector3 v)
{
    int found = 0;
    
    int i;
    for (i = 0; i < n && !found; i++)
    {
        found = BHCSVector3EqualToVector3(array[i], v);
    }
    
    return found;
}

void printVectorArray(BHCSVector3 *array, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("(%i, %i, %i) ", array[i].x, array[i].y, array[i].z);
    }
    printf("\n");
}