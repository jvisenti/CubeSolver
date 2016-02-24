//
//  BHCSWorldNode.m
//  BHCubeSolver
//
//  Created by John Visentin on 3/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHCSWorldNode.h"
#import "BHCSPieceNode.h"
#import "BHGLBasicAnimation.h"

#define kSSWorldNodeUnpackDuration 2.0

@interface BHCSWorldNode ()

@property (nonatomic, weak) BHCSCube *boundingBox;

- (void)updateBoundingBox;

@end

@implementation BHCSWorldNode

- (id)initWithWorld:(BHCSWorld *)world
{
    if ((self = [super init]))
    {
        self.world = world;
        _packed = YES;
    }
    return self;
}

#pragma mark - public interface

- (void)setWorld:(BHCSWorld *)world
{
    [self.children enumerateObjectsUsingBlock:^(BHGLNode *child, NSUInteger idx, BOOL *stop) {
        [child removeFromParent];
    }];
    
    [world.pieces enumerateObjectsUsingBlock:^(BHCSPiece *piece, BOOL *stop) {
        BHCSPieceNode *pieceNode = [[BHCSPieceNode alloc] initWithPiece:piece];
        
        [self addChild:pieceNode];
    }];
    
    _world = world;
    
    [self updateBoundingBox];
}

- (void)update:(NSTimeInterval)dt
{
    [super update:dt];
    
    float xOff = self.world.size.w % 2 == 0 ? (self.world.size.w-1)/2.0 : self.world.size.w/2;
    float yOff = self.world.size.h % 2 == 0 ? (self.world.size.h-1)/2.0 : self.world.size.h/2;
    float zOff = self.world.size.d % 2 == 0 ? (self.world.size.d-1)/2.0 : self.world.size.d/2;
    
    GLKVector3 offset = GLKVector3Make(xOff, yOff, zOff);
    
    if (self.isPacked)
    {
        [self.children enumerateObjectsUsingBlock:^(BHGLNode *child, NSUInteger idx, BOOL *stop) {
            if ([child isKindOfClass:[BHCSPieceNode class]])
            {
                BHCSPieceNode *pieceNode = (BHCSPieceNode *)child;
                pieceNode.position = GLKVector3Make(pieceNode.piece.worldPosition.x, pieceNode.piece.worldPosition.y, pieceNode.piece.worldPosition.z);
                pieceNode.position = GLKVector3Subtract(pieceNode.position, offset);
            }
        }];
    }
}

- (void)unpack
{
    if (self.isPacked)
    {
        [self.children enumerateObjectsUsingBlock:^(BHGLNode *child, NSUInteger idx, BOOL *stop) {
            if ([child isKindOfClass:[BHCSPieceNode class]])
            {
                BHCSPieceNode *pieceNode = (BHCSPieceNode *)child;
                BHGLBasicAnimation *trans = [BHGLBasicAnimation translateTo:GLKVector3MultiplyScalar(pieceNode.position, 2.0f) withDuration:kSSWorldNodeUnpackDuration];
                [pieceNode runAnimation:trans];
            }
        }];
        
        BHGLBasicAnimation *setPacked = [BHGLBasicAnimation runBlock:^{
            _packed = NO;
        }];
        
        BHGLAnimation *seq = [BHGLAnimation sequence:@[[BHGLBasicAnimation wait:kSSWorldNodeUnpackDuration], setPacked]];
        [self runAnimation:seq];
    }
}

- (void)pack
{
    if (!self.isPacked)
    {
        [self.children enumerateObjectsUsingBlock:^(BHGLNode *child, NSUInteger idx, BOOL *stop) {
            if ([child isKindOfClass:[BHCSPieceNode class]])
            {
                BHCSPieceNode *pieceNode = (BHCSPieceNode *)child;
                BHGLBasicAnimation *trans = [BHGLBasicAnimation translateTo:GLKVector3MultiplyScalar(pieceNode.position, 0.5f) withDuration:kSSWorldNodeUnpackDuration];
                [pieceNode runAnimation:trans];
            }
        }];
        
        BHGLBasicAnimation *setPacked = [BHGLBasicAnimation runBlock:^{
            _packed = YES;
        }];
        
        BHGLAnimation *seq = [BHGLAnimation sequence:@[[BHGLBasicAnimation wait:kSSWorldNodeUnpackDuration], setPacked]];
        [self runAnimation:seq];
    }
}

#pragma mark - private interface

- (void)updateBoundingBox;
{
    [self.boundingBox removeFromParent];
    
    BHCSCube *boundingBox = [[BHCSCube alloc] init];
    boundingBox.scale = GLKVector3Make(self.world.size.w+0.01, self.world.size.h+0.01, self.world.size.d+0.01);
    
    boundingBox.material.surfaceColor = BHGLColorMake(1.0f, 1.0f, 1.0f, 0.1f);
    boundingBox.material.blendEnabled = GL_TRUE;
    boundingBox.material.blendSrcRGB = GL_SRC_ALPHA;
    boundingBox.material.blendDestRGB = GL_ONE_MINUS_SRC_ALPHA;
    
    [self addChild:boundingBox];
    
    self.boundingBox = boundingBox;
}

@end
