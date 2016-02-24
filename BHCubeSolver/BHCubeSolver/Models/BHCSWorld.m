//
//  BHCSWorld.m
//  BHCubeSolver
//
//  Created by John Visentin on 3/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHCSWorld.h"
#import "BHCSCubeArray.h"
#import "BHCSAlgorithmXMatrix.h"

#ifdef DEBUG
#import <QuartzCore/QuartzCore.h>
#endif

@interface BHCSWorld ()

@property (nonatomic, strong) BHCSCubeArray *cubeArray;

- (BOOL)canPlacePiece:(BHCSPiece *)piece atPoint:(BHCSVector3)point;
- (void)placePiece:(BHCSPiece *)piece atPoint:(BHCSVector3)point;
- (void)removePiece:(BHCSPiece *)piece;

- (BOOL)solveRecursiveBrute:(NSSet *)remainingPieces;

@end

@implementation BHCSWorld

#pragma mark - initialization

- (id)initWithSize:(BHCSVector3)size
{
    if ((self = [super init]))
    {
        self.cubeArray = [[BHCSCubeArray alloc] initWithSize:size];
    }
    return self;
}

- (id)initWithFile:(NSString *)filePath
{
    NSError *error = nil;
    NSString *contents = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:&error];
    
    if (error)
    {
        NSLog(@"[%@] error reading file: %@. Reason: %@", NSStringFromClass([self class]), filePath, error);
        return nil;
    }
    
    return [self initWithString:contents];
}

- (id)initWithString:(NSString *)string
{
    string = [string stringByReplacingOccurrencesOfString:@"\n" withString:@""];
    string = [string stringByReplacingOccurrencesOfString:@" " withString:@""];
    
    NSArray *lines = [string componentsSeparatedByString:@";"];
    
    if (lines)
    {
        string = [string substringFromIndex:[string rangeOfString:@";"].location+1];
        
        NSArray *sizeArray = [[lines objectAtIndex:0] componentsSeparatedByString:@"x"];
        
        if ([sizeArray count] == 3)
        {
            BHCSVector3 size = BHCSVector3Make([[sizeArray objectAtIndex:0] intValue], [[sizeArray objectAtIndex:1] intValue], [[sizeArray objectAtIndex:2] intValue]);
            
            self = [self initWithSize:size];
            [self loadPiecesFromString:string];
        }
    }
    
    if (!self)
    {
        NSLog(@"[%@] malformed string.", NSStringFromClass([self class]));
    }
    
    return self;
}

- (void)loadPiecesFromFile:(NSString *)filePath
{
    NSError *error = nil;
    NSString *contents = [NSString stringWithContentsOfFile:filePath encoding:NSUTF8StringEncoding error:&error];
    
    if (error)
    {
        NSLog(@"[%@] error reading file: %@. Reason: %@", NSStringFromClass([self class]), filePath, error);
        self.pieces = nil;
    }
    
    [self loadPiecesFromString:contents];
}

- (void)loadPiecesFromString:(NSString *)string
{
    string = [string stringByReplacingOccurrencesOfString:@"\n" withString:@""];
    string = [string stringByReplacingOccurrencesOfString:@" " withString:@""];
    
    NSArray *pieces = [string componentsSeparatedByString:@"//"];
    
    NSMutableSet *mutablePieces = [NSMutableSet setWithCapacity:[pieces count]];
    
    for (NSString *pieceString in pieces)
    {
        BHCSPiece *piece = [[BHCSPiece alloc] initWithString:pieceString];
        
        if (piece)
        {
            [mutablePieces addObject:piece];
        }
    }
    
    self.pieces = mutablePieces;
}

#pragma mark - public interface

- (BHCSVector3)size
{
    return self.cubeArray.size;
}

- (BHCSCube *)cubeAtPoint:(BHCSVector3)point
{
    return [self.cubeArray cubeAtPoint:point];
}

- (int)cubeIndexFromPoint:(BHCSVector3)point
{
    return [self.cubeArray indexFromPoint:point];
}

- (BOOL)pieceInside:(BHCSPiece *)piece
{
    __block BOOL inside = YES;
    
    [piece.cubes enumerateObjectsUsingBlock:^(BHCSCube *cube, NSUInteger idx, BOOL *stop) {
        BHCSVector3 cubePos = BHCSVector3Make(round(cube.position.x), round(cube.position.y), round(cube.position.z));
        BHCSVector3 p = BHCSVector3Add(piece.worldPosition, [piece rotateVector3:cubePos]);
        
        if (![self.cubeArray pointInside:p])
        {
            inside = NO;
            *stop = YES;
        }
    }];
    
    return inside;
}

- (BOOL)pointInside:(BHCSVector3)point
{
    return [self.cubeArray pointInside:point];
}

- (void)solveBrute
{
    [self.pieces enumerateObjectsUsingBlock:^(BHCSPiece *piece, BOOL *stop) {
        piece.hidden = YES;
        piece.worldPosition = BHCSVector3Make(0, 0, -100); // hack to make sure not visible
    }];
    
#ifdef DEBUG
    NSTimeInterval start = CACurrentMediaTime();
#endif
    
    [self solveRecursiveBrute:self.pieces];
    
#ifdef DEBUG
    NSLog(@"[%@] solve time: %f", NSStringFromClass([self class]), CACurrentMediaTime() - start);
#endif
    
    [self.cubeArray removeAllCubes];
}

- (void)solveAlgorithmX
{
    [self.pieces enumerateObjectsUsingBlock:^(BHCSPiece *piece, BOOL *stop) {
        piece.hidden = YES;
        piece.worldPosition = BHCSVector3Make(0, 0, -100); // hack to make sure not visible
    }];
    
#ifdef DEBUG
    NSTimeInterval start = CACurrentMediaTime();
#endif
    
    BHCSAlgorithmXMatrix *matrix = [[BHCSAlgorithmXMatrix alloc] init];
    [matrix solveWorld:self];
    
#ifdef DEBUG
    NSLog(@"[%@] solve time: %f", NSStringFromClass([self class]), CACurrentMediaTime() - start);
#endif
    
    [self.pieces enumerateObjectsUsingBlock:^(BHCSPiece *piece, BOOL *stop) {
        piece.hidden = NO;
    }];
}

#pragma mark - private interface

/** Try every piece at every location in every rotation. BRUTE FORCE YEAH! */
- (BOOL)solveRecursiveBrute:(NSSet *)remainingPieces
{
    // base case: all pieces used, we're done
    if ([remainingPieces count] == 0)
    {
        return YES;
    }
    
    for (BHCSPiece *piece in remainingPieces)
    {
        for (int x = 0; x < self.size.w; x++)
        {
            for (int y = 0; y < self.size.h; y++)
            {
                for (int z = 0; z < self.size.d; z++)
                {
                    BHCSVector3 point = BHCSVector3Make(x, y, z);
                    
                    for (int i = 0; i < kBHCSPieceNumInPlaceRotations; i++)
                    {
                        [piece setRotationIndex:i];
                        
                        if ([self canPlacePiece:piece atPoint:point])
                        {
                            [self placePiece:piece atPoint:point];
                            
                            NSMutableSet *nextPieces = [remainingPieces mutableCopy];
                            [nextPieces removeObject:piece];
                            
                            BOOL solved = [self solveRecursiveBrute:nextPieces];
                            
                            if (solved)
                            {
                                return YES;
                            }
                            else
                            {
                                [self removePiece:piece];
                            }
                        }
                    }
                }
            }
        }
    }

    return NO;
}

- (BOOL)canPlacePiece:(BHCSPiece *)piece atPoint:(BHCSVector3)point
{
    __block BOOL conflict = NO;

    [piece.cubes enumerateObjectsUsingBlock:^(BHCSCube *cube, NSUInteger idx, BOOL *stop) {
        BHCSVector3 cubePos = BHCSVector3Make(round(cube.position.x), round(cube.position.y), round(cube.position.z));
        BHCSVector3 p = BHCSVector3Add(point, [piece rotateVector3:cubePos]);
        
//        TODO: maybe make this work
//        BHCSVector3 y = BHCSVector3Make(0, 1, 0);
//        y = [piece rotateVector3:y];
//        y = BHCSVector3Add(cubePos, y);
        
        BOOL below = YES; //p.y <= 0 || [piece cubeAtRelativePoint:y] || [self.cubeArray cubeAtPoint:BHCSVector3Make(p.x, p.y - 1, p.z)];
        
        if (![self.cubeArray pointInside:p] || [self.cubeArray cubeAtPoint:p] || !below)
        {
            conflict = YES;
            *stop = YES;
        }
    }];
    
    return !conflict;
}

- (void)placePiece:(BHCSPiece *)piece atPoint:(BHCSVector3)point
{
    [piece.cubes enumerateObjectsUsingBlock:^(BHCSCube *cube, NSUInteger idx, BOOL *stop) {
        BHCSVector3 cubePos = BHCSVector3Make(round(cube.position.x), round(cube.position.y), round(cube.position.z));
        BHCSVector3 p = BHCSVector3Add(point, [piece rotateVector3:cubePos]);
        
        [self.cubeArray setCube:cube atPoint:p];
    }];
    
    piece.worldPosition = point;
    piece.hidden = NO;
}

- (void)removePiece:(BHCSPiece *)piece
{
    [piece.cubes enumerateObjectsUsingBlock:^(BHCSCube *cube, NSUInteger idx, BOOL *stop) {
        BHCSVector3 cubePos = BHCSVector3Make(round(cube.position.x), round(cube.position.y), round(cube.position.z));
        BHCSVector3 p = BHCSVector3Add(piece.worldPosition, [piece rotateVector3:cubePos]);
        
        [self.cubeArray setCube:nil atPoint:p];
    }];
    
    piece.worldPosition = BHCSVector3Make(0, 0, -100); // hack to make sure not visible
    piece.hidden = YES;
}

@end
