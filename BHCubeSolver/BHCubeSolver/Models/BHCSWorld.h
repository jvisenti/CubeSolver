//
//  BHCSWorld.h
//  BHCubeSolver
//
//  Created by John Visentin on 3/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BHCSPiece.h"

@interface BHCSWorld : NSObject

@property (nonatomic, readonly) BHCSVector3 size;
@property (nonatomic, copy) NSSet *pieces;

- (id)initWithSize:(BHCSVector3)size;

/** File format must be valid for loading pieces but should contain @code
    w x h x d;
    @endcode
    At the start of the file to specify world size. */
- (id)initWithFile:(NSString *)filePath;

/** String format must be valid for loading pieces but should contain @code
 w x h x d;
 @endcode
 At the start of the file to specify world size. */
- (id)initWithString:(NSString *)string;

/** File format must be valid piece formats (see BHCSPiece.h) separated by "//" */
- (void)loadPiecesFromFile:(NSString *)filePath;

/** String format must be valid piece formats (see BHCSPiece.h) separated by "//" */
- (void)loadPiecesFromString:(NSString *)string;

- (BHCSCube *)cubeAtPoint:(BHCSVector3)point;

- (int)cubeIndexFromPoint:(BHCSVector3)point;

- (BOOL)pieceInside:(BHCSPiece *)piece;
- (BOOL)pointInside:(BHCSVector3)point;

/** A brute force recursive solver. */
- (void)solveBrute;

/** An implementation of Don Knuth's AlgorithmX, an algorithm for solving exact cover problems. */
- (void)solveAlgorithmX;

@end
