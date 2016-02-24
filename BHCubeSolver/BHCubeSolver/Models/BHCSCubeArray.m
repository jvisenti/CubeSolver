//
//  BHCSCubeArray.m
//  BHCubeSolver
//
//  Created by John Visentin on 3/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHCSCubeArray.h"

@interface BHCSCubeArray ()

@property (nonatomic, strong) NSMutableArray *cubes;

@end

@implementation BHCSCubeArray

- (id)initWithSize:(BHCSVector3)size
{
    if ((self = [super init]))
    {
        self.size = size;
        
        int volume =  size.w * size.h * size.d;
        self.cubes = [NSMutableArray arrayWithCapacity:volume];
        
        for (int x = 0; x < volume; x++)
        {
            [self.cubes addObject:[NSNull null]];
        }
    }
    return self;
}

#pragma mark - public interface

- (BHCSCube *)cubeAtPoint:(BHCSVector3)point
{
    if ([self pointInside:point])
    {
        int index = [self indexFromPoint:point];
        
        return [self cubeAtIndex:index];
    }
    
    return nil;
}

- (void)setCube:(BHCSCube *)cube atPoint:(BHCSVector3)point
{
    if ([self pointInside:point])
    {
        int index = [self indexFromPoint:point];
        [self setCube:cube atIndex:index];
    }
}

- (BOOL)pointInside:(BHCSVector3)point
{
    return (point.x >= 0 && point.x < self.size.w
            && point.y >= 0 && point.y < self.size.h
            && point.z >= 0 && point.z < self.size.d);
}

- (BHCSCube *)cubeAtIndex:(int)index
{
    BHCSCube *cube = nil;
    
    if (index >= 0 && index < [self.cubes count])
    {
        id obj = self.cubes[index];
        if ([obj isKindOfClass:[BHCSCube class]])
        {
            cube = obj;
        }
    }
    
    return cube;
}

- (void)setCube:(BHCSCube *)cube atIndex:(int)index
{
    if (index >= 0 && index < [self.cubes count])
    {
        self.cubes[index] = cube ? cube : [NSNull null];
    }
}

- (int)indexFromPoint:(BHCSVector3)point
{
    return (self.size.w * self.size.d * point.y) + (self.size.w * point.z) + point.x;
}

- (void)removeAllCubes
{
    [self.cubes removeAllObjects];
}

@end
