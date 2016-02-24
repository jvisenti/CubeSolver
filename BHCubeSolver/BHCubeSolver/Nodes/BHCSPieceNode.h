//
//  BHCSPieceNode.h
//  BHCubeSolver
//
//  Created by John Visentin on 3/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHGLNode.h"
#import "BHCSPiece.h"

@interface BHCSPieceNode : BHGLNode

@property (nonatomic, strong) BHCSPiece *piece;

/** Computed from the piece's axisRotation property.
    Setting this property has no effect. */
@property (nonatomic, assign) GLKQuaternion rotation;

- (id)initWithPiece:(BHCSPiece *)piece;

@end
