//
//  BHCSCubeArray.h
//  BHCubeSolver
//
//  Created by John Visentin on 3/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "BHCSCube.h"

@interface BHCSCubeArray : NSObject

@property (nonatomic, assign) BHCSVector3 size;

- (id)initWithSize:(BHCSVector3)size;

- (BHCSCube *)cubeAtPoint:(BHCSVector3)point;
- (void)setCube:(BHCSCube *)cube atPoint:(BHCSVector3)point;

- (BOOL)pointInside:(BHCSVector3)point;

- (BHCSCube *)cubeAtIndex:(int)index;
- (void)setCube:(BHCSCube *)cube atIndex:(int)index;

- (int)indexFromPoint:(BHCSVector3)point;

- (void)removeAllCubes;

@end
