//
//  BHCSAlgorithmXMatrix.m
//  BHCubeSolver
//
//  Created by John Visentin on 3/2/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHCSAlgorithmXMatrix.h"
#import "BHCSPiece.h"
#import "BHCSWorld.h"
#import "BHDancingLinks.h"

@interface BHCSRowInfo : NSObject

@property (nonatomic, strong) BHCSPiece *piece;
@property (nonatomic, assign) BHCSVector3 worldPos;
@property (nonatomic, assign) int rotationIndex;

@end

@implementation BHCSRowInfo
@end

@interface BHCSAlgorithmXMatrix ()

- (BHDancingLinks *)matrixWithWorld:(BHCSWorld *)world;

- (BOOL)solveRecursive:(BHDancingLinks *)matrix numPieces:(NSUInteger)pieceCount;

@end

@implementation BHCSAlgorithmXMatrix

#pragma mark - public interface

- (BOOL)solveWorld:(BHCSWorld *)world
{
    BHDancingLinks *matrix = [self matrixWithWorld:world];
    
    BOOL solved = [self solveRecursive:matrix numPieces:[world.pieces count]];
    
    BHDancingLinksRow *rows = BHDancingLinksGetRows(matrix);
    for (BHDancingLinksRow *row = rows; row != NULL; row = BHDancingLinksRowGetBelow(row))
    {
        CFBridgingRelease(BHDancingLinksRowGetHeaderData(row));
        BHDancingLinksRowSetHeaderData(row, NULL);
    }
    
    BHDancingLinksFree(matrix);
    
#ifdef DEBUG
    if (!solved)
    {
        NSLog(@"[%@] matrix has no solution!", NSStringFromClass([self class]));
    }
#endif
    
    return solved;
}

#pragma mark - private interface

- (BHDancingLinks *)matrixWithWorld:(BHCSWorld *)world
{
    NSArray *pieces = [world.pieces allObjects];
    
    int numPieces = (int)[pieces count];
    int cols = world.size.w * world.size.h * world.size.d + numPieces;
    
    BHDancingLinks *matrix = BHDancingLinksCreate(cols);
    
    for (int p = 0; p < numPieces; p++)
    {
        BHCSPiece *piece = pieces[p];
        BHCSVector3 initialPos = piece.worldPosition;
        
        for (int x = 0; x < world.size.w; x++)
        {
            for (int y = 0; y < world.size.h; y++)
            {
                for (int z = 0; z < world.size.d; z++)
                {
                    BHCSVector3 point = BHCSVector3Make(x, y, z);
                    
                    for (int i = 0; i < kBHCSPieceNumInPlaceRotations; i++)
                    {
                        [piece setRotationIndex:i];
                        piece.worldPosition = point;
                        
                        if ([world pieceInside:piece])
                        {
                            BHCSRowInfo *rowInfo = [[BHCSRowInfo alloc] init];
                            rowInfo.piece = piece;
                            rowInfo.worldPos = point;
                            rowInfo.rotationIndex = i;
                            
                            BHDancingLinksRow *row = BHDancingLinksAddRow(matrix);
                            BHDancingLinksRowSetHeaderData(row, (void *)CFBridgingRetain(rowInfo));

                            BHDancingLinksColumn *col = BHDancingLinksGetColumns(matrix);
                            
                            NSMutableIndexSet *cubeIndices = [NSMutableIndexSet indexSet];
                            
                            [piece.cubes enumerateObjectsUsingBlock:^(BHCSCube *cube, NSUInteger idx, BOOL *stop) {
                                BHCSVector3 cubePos = BHCSVector3Make(round(cube.position.x), round(cube.position.y), round(cube.position.z));
                                BHCSVector3 p = BHCSVector3Add(point, [piece rotateVector3:cubePos]);
                                
                                int index = [world cubeIndexFromPoint:p];
                                
                                [cubeIndices addIndex:index];
                            }];
                            
                            for (int i = 0; i < cols; i++)
                            {
                                if (i == p || [cubeIndices containsIndex:i - numPieces])
                                {
                                    BHDancingLinksAddNode(col, row);
                                }
                                
                                col = BHDancingLinksColumnGetNext(col);
                            }
                        }
                    }
                }
            }
        }
        
        piece.worldPosition = initialPos;
    }

    return matrix;
}

- (BOOL)solveRecursive:(BHDancingLinks *)matrix numPieces:(NSUInteger)pieceCount
{
    if (BHDancingLinksGetRowCount(matrix) == 0)
    {
        /*  NOTE: this requires we know the solution size ahead of time.
         Using straight AlgorithmX terminated too early, as soon as matrix was empty.
         But there are situations where the matrix can be empty but not all columns covered in the solution e.g.
         
         10111
         01101
         10100
         01100
         
         (not sure if flaw in algorithm or flaw in understanding)
         */
        return pieceCount == 0 ? YES : NO;
    }
    
#ifdef DEBUG
    NSLog(@"total cols: %i, rows: %i", (int)BHDancingLinksGetColumnCount(matrix), (int)BHDancingLinksGetRowCount(matrix));
#endif
    
    BHDancingLinksColumn *leastCol = BHDancingLinksGetLeastColumn(matrix);
    
    if (BHDancingLinksColumnGetRowCount(leastCol) == 0)
    {
#ifdef DEBUG
        NSLog(@"NOPE: col empty");
#endif
        return NO;
    }
    
#ifdef DEBUG
    NSLog(@"Choosing col with %i rows", (int)BHDancingLinksColumnGetRowCount(leastCol));
#endif
    
    BOOL solved = NO;
    
    BHDancingLinksNode **colNodes = BHDancingLinksColumnGetNodes(leastCol);
    
    for (int i = 0; i < BHDancingLinksColumnGetRowCount(leastCol) && !solved; i++)
    {
        BHDancingLinksNode *node = colNodes[i];
        BHDancingLinksRow *removedRows = BHDancingLinksRemoveNode(matrix, node);
        
        BHCSRowInfo *rowInfo = (__bridge BHCSRowInfo *)(BHDancingLinksRowGetHeaderData(BHDancingLinksNodeGetRow(node)));
        BHCSVector3 initialPos = rowInfo.piece.worldPosition;
        
        rowInfo.piece.worldPosition = rowInfo.worldPos;
        [rowInfo.piece setRotationIndex:rowInfo.rotationIndex];
        rowInfo.piece.hidden = NO;
        
        solved = [self solveRecursive:matrix numPieces:pieceCount-1];
        
        if (!solved)
        {
            rowInfo.piece.hidden = YES;
            rowInfo.piece.worldPosition = initialPos;
            
            BHDancingLinksInsertRows(matrix, removedRows);
        }
    }
    
    free(colNodes);
    
    return solved;
}

@end
