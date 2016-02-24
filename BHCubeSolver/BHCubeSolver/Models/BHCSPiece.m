//
//  BHCSPiece.m
//  BHCubeSolver
//
//  Created by John Visentin on 2/28/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHCSPiece.h"
#import "BHCSCubeArray.h"

@interface BHCSPiece ()
{
    GLKQuaternion *_cachedRotation;
}

@property (nonatomic, strong) BHCSCubeArray *cubeArray;

- (void)invalidateRotationCache;

@end

@implementation BHCSPiece

@synthesize cubes = _cubes;

#pragma mark - initialization

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
    if ((self = [super init]))
    {
        string = [string stringByReplacingOccurrencesOfString:@"\n" withString:@""];
        string = [string stringByReplacingOccurrencesOfString:@" " withString:@""];
        
        NSArray *lines = [string componentsSeparatedByString:@";"];
        NSUInteger numLines = [lines count];
        
        int lineStart = 0;
        
        BHGLColor color = BHGLColorBlack;
        
        if (numLines)
        {
            NSString *colorLine = [lines objectAtIndex:0];
            NSRange range = [colorLine rangeOfString:@"c:"];
            if (range.location != NSNotFound)
            {
                NSString *substr = [colorLine substringFromIndex:NSMaxRange(range)];
                NSArray *colorComponents = [substr componentsSeparatedByString:@","];
                
                if ([colorComponents count] == 3)
                {
                    float r = [colorComponents[0] floatValue];
                    float g = [colorComponents[1] floatValue];
                    float b = [colorComponents[2] floatValue];
                    
                    color = BHGLColorMake(r, g, b, 1.0f);
                }
                
                lineStart = 1;
            }
        }
        
        NSMutableArray *points = [NSMutableArray arrayWithCapacity:numLines];
        
        int width = 0;
        int height = 0;
        int depth = 0;
        
        for (int i = lineStart; i < numLines-1; i++)
        {
            NSArray *point = [[lines objectAtIndex:i] componentsSeparatedByString:@","];
            
            if ([point count] != 3)
            {
                NSLog(@"[%@] error: each point must have x, y, and z coordinates.", NSStringFromClass([self class]));
                return self;
            }
            
            int x = [[point objectAtIndex:0] intValue];
            int y = [[point objectAtIndex:1] intValue];
            int z = [[point objectAtIndex:2] intValue];
            
            width = MAX(width, x+1);
            height = MAX(height, y+1);
            depth = MAX(depth, z+1);
            
            [points addObject:point];
        }
        
        self.cubeArray = [[BHCSCubeArray alloc] initWithSize:BHCSVector3Make(width, height, depth)];
        
        for (NSArray *point in points)
        {
            int x = [[point objectAtIndex:0] intValue];
            int y = [[point objectAtIndex:1] intValue];
            int z = [[point objectAtIndex:2] intValue];
            
            BHCSCube *cube = [[BHCSCube alloc] init];
            [self.cubeArray setCube:cube atPoint:BHCSVector3Make(x, y, z)];
            
            cube.position = GLKVector3Make(x, y, z);
        }
        
        self.color = color;
    }
    return self;
}

#pragma mark - public interface

- (BHCSVector3)size
{
    return self.cubeArray.size;
}

- (NSArray *)cubes
{
    if (!_cubes)
    {
        NSMutableArray *mutableCubes = [NSMutableArray array];
        
        for (int i = 0; i < self.cubeArray.size.w; i++)
        {
            for (int j = 0; j < self.cubeArray.size.h; j++)
            {
                for (int k = 0; k < self.cubeArray.size.d; k++)
                {
                    BHCSCube *cube = [self.cubeArray cubeAtPoint:BHCSVector3Make(i, j, k)];
                    
                    if (cube)
                    {
                        [mutableCubes addObject:cube];
                    }
                }
            }
        }
        
        _cubes = [mutableCubes copy];
    }
    
    return _cubes;
}

- (void)setColor:(BHGLColor)color
{
    _color = color;
    
    [self.cubes enumerateObjectsUsingBlock:^(BHCSCube *cube, NSUInteger idx, BOOL *stop) {
        cube.material.surfaceColor = color;
    }];
}

- (BHCSCube *)cubeAtRelativePoint:(BHCSVector3)pos
{
    return [self.cubeArray cubeAtPoint:pos];
}

- (void)rotateX
{
    _axisRotation.x = (_axisRotation.x + 1) % 4;
    
    [self invalidateRotationCache];
}

- (void)rotateY
{
    _axisRotation.y = (_axisRotation.y + 1) % 4;
    
    [self invalidateRotationCache];
}

- (void)rotateZ
{
    _axisRotation.z = (_axisRotation.z + 1) % 4;
    
    [self invalidateRotationCache];

}

- (void)setRotationIndex:(int)index
{
    static BHCSVector3 Rotations[kBHCSPieceNumInPlaceRotations];
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        Rotations[0] = BHCSVector3Make(0, 0, 0);
        Rotations[1] = BHCSVector3Make(0, 1, 0);
        Rotations[2] = BHCSVector3Make(0, 2, 0);
        Rotations[3] = BHCSVector3Make(0, 3, 0);
        Rotations[4] = BHCSVector3Make(0, 0, 2);
        Rotations[5] = BHCSVector3Make(0, 1, 2);
        Rotations[6] = BHCSVector3Make(0, 2, 2);
        Rotations[7] = BHCSVector3Make(0, 3, 2);
        
        Rotations[8] = BHCSVector3Make(0, 0, 1);
        Rotations[9] = BHCSVector3Make(0, 1, 1);
        Rotations[10] = BHCSVector3Make(0, 2, 1);
        Rotations[11] = BHCSVector3Make(0, 3, 1);
        Rotations[12] = BHCSVector3Make(0, 0, 3);
        Rotations[13] = BHCSVector3Make(0, 1, 3);
        Rotations[14] = BHCSVector3Make(0, 2, 3);
        Rotations[15] = BHCSVector3Make(0, 3, 3);
        
        Rotations[16] = BHCSVector3Make(1, 0, 0);
        Rotations[17] = BHCSVector3Make(1, 0, 1);
        Rotations[18] = BHCSVector3Make(1, 0, 2);
        Rotations[19] = BHCSVector3Make(1, 0, 3);
        Rotations[20] = BHCSVector3Make(3, 0, 0);
        Rotations[21] = BHCSVector3Make(3, 0, 1);
        Rotations[22] = BHCSVector3Make(3, 0, 2);
        Rotations[23] = BHCSVector3Make(3, 0, 3);
    });
    
    if (index >= 0 && index < kBHCSPieceNumInPlaceRotations)
    {
        _axisRotation = Rotations[index];
        [self invalidateRotationCache];
    }
}

- (void)resetRotation
{
    _axisRotation.x = 0;
    _axisRotation.y = 0;
    _axisRotation.z = 0;
    
    [self invalidateRotationCache];
}

- (GLKQuaternion)quaternionRotation
{
    if (!_cachedRotation)
    {
        GLKQuaternion rx = GLKQuaternionMakeWithAngleAndAxis(self.axisRotation.x * M_PI_2, 1.0f, 0.0f, 0.0f);
        GLKQuaternion ry = GLKQuaternionMakeWithAngleAndAxis(self.axisRotation.y * M_PI_2, 0.0f, 1.0f, 0.0f);
        GLKQuaternion rz = GLKQuaternionMakeWithAngleAndAxis(self.axisRotation.z * M_PI_2, 0.0f, 0.0f, 1.0f);
        
        GLKQuaternion r = GLKQuaternionMultiply(rz, ry);
        r = GLKQuaternionMultiply(r, rx);
        
        _cachedRotation = (GLKQuaternion *)malloc(sizeof(GLKQuaternion));
        memcpy(_cachedRotation->q, r.q, sizeof(r.q));
    }
    
    return *_cachedRotation;
}

- (BHCSVector3)rotateVector3:(BHCSVector3)vec
{
    GLKVector3 glkv3 = GLKVector3Make(vec.x, vec.y, vec.z);
    glkv3 = GLKQuaternionRotateVector3([self quaternionRotation], glkv3);
    
    return BHCSVector3Make(round(glkv3.x), round(glkv3.y), round(glkv3.z));
}

#pragma mark - private interface

- (void)invalidateRotationCache
{
    dispatch_sync(dispatch_get_main_queue(), ^{
        if (_cachedRotation)
        {
            free(_cachedRotation);
            _cachedRotation = NULL;
        }
    });
}

@end
