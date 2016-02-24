//
//  BHCSWorldNode.h
//  BHCubeSolver
//
//  Created by John Visentin on 3/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHGLNode.h"
#import "BHCSWorld.h"

@interface BHCSWorldNode : BHGLNode

@property (nonatomic, strong) BHCSWorld *world;
@property (nonatomic, readonly, getter = isPacked) BOOL packed;

- (id)initWithWorld:(BHCSWorld *)world;

- (void)unpack;
- (void)pack;

@end
