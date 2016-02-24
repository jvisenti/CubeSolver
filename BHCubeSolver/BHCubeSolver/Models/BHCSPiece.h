//
//  BHCSPiece.h
//  BHCubeSolver
//
//  Created by John Visentin on 2/28/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKMathTypes.h>
#import "BHCSCube.h"

#define kBHCSPieceNumInPlaceRotations 24

@interface BHCSPiece : NSObject

@property (nonatomic, assign) BHGLColor color;
@property (atomic, assign, getter = isHidden) BOOL hidden;

/** Location of negative-most x, y, z (before rotation) in world coordinates. */
@property (atomic, assign) BHCSVector3 worldPosition;

/** Each element in the vector represents the number of 90 degree 
    rotations CCW around the corresponding axis. */
@property (atomic, readonly) BHCSVector3 axisRotation;

/** 3D bounding box for the piece. */
@property (nonatomic, readonly) BHCSVector3 size;

/** A 1D array of all cubes in the piece with no rotation applied. */
@property (nonatomic, readonly) NSArray *cubes;

/** pos is relative to the worldPosition property. */
- (BHCSCube *)cubeAtRelativePoint:(BHCSVector3)pos;

/** Rotate 90 degrees CCW around x-axis. */
- (void)rotateX;

/** Rotate 90 degrees CCW around x-axis. */
- (void)rotateY;

/** Rotate 90 degrees CCW around x-axis. */
- (void)rotateZ;

/** Set a pre-computed unique axis rotation.
    @param index A value from 0 up to (but not including) 
    kBHCSPieceNumInPlaceRotations */
- (void)setRotationIndex:(int)index;

- (void)resetRotation;

/** axisRotation property represented as a GLKQuaternion. */
- (GLKQuaternion)quaternionRotation;

/** Apply axisRotation to given vector. */
- (BHCSVector3)rotateVector3:(BHCSVector3)vec;


/** File format must be @code
    c: r, g, b; <-- optional
    x1, y1, z1;
    x2, y2, z2;
    ...
    xn, yn, zn;
    @endcode
 */
- (id)initWithFile:(NSString *)filePath;

/** String format must be @code
    c: r, g, b; <-- optional
    x1, y1, z1;
    x2, y2, z2;
    ...
    xn, yn, zn;
    @endcode
 */
- (id)initWithString:(NSString *)string;

@end
