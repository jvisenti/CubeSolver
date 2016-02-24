//
//  BHCSScene.h
//  BHCubeSolver
//
//  Created by John Visentin on 2/28/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHGLScene.h"
#import "BHCSWorldNode.h"

@interface BHCSScene : BHGLScene

@property (nonatomic, strong) BHCSWorld *world;
@property (nonatomic, readonly) BHCSWorldNode *worldNode;

- (void)stopRotation;
- (void)startRotation;

@end
