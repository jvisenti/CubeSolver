//
//  BHCSPieceNode.m
//  BHCubeSolver
//
//  Created by John Visentin on 3/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHCSPieceNode.h"

@implementation BHCSPieceNode

- (id)initWithPiece:(BHCSPiece *)piece
{
    if ((self = [super init]))
    {
        self.piece = piece;
    }
    return self;
}

- (GLKQuaternion)rotation
{
    return self.piece ? [self.piece quaternionRotation] : GLKQuaternionIdentity;
}

- (void)setRotation:(GLKQuaternion)rotation
{
    [super setRotation:rotation];
}

- (void)setPiece:(BHCSPiece *)piece
{
    [self.children enumerateObjectsUsingBlock:^(BHGLNode *child, NSUInteger idx, BOOL *stop) {
        [child removeFromParent];
    }];
    
    _piece = piece;
    
    [piece.cubes enumerateObjectsUsingBlock:^(BHCSCube *cube, NSUInteger idx, BOOL *stop) {
        [self addChild:cube];
    }];
}

- (void)render
{
    if (!self.piece.isHidden)
    {
        [super render];
    }
}

- (void)renderWithProgram:(BHGLProgram *)program
{
    if (!self.piece.isHidden)
    {
        [super renderWithProgram:program];
    }
}

@end
