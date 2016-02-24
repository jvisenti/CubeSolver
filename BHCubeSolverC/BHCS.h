//
//  BHCS.h
//  BHCubeSolver
//
//  Created by John Visentin on 3/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#ifndef BHCubeSolver_BHCS_h
#define BHCubeSolver_BHCS_h

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

typedef union _BHCSVector3
{
    struct {int x, y, z;};
    struct {int w, h, d;};
} BHCSVector3;

typedef struct _BHCSQuaternion
{
    float x, y, z, w;
} BHCSQuaternion;

static inline BHCSVector3 BHCSVector3Make(int x, int y, int z)
{
    return (BHCSVector3){{x, y, z}};
}

static inline BHCSVector3 BHCSVector3Add(BHCSVector3 v1, BHCSVector3 v2)
{
    return BHCSVector3Make(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

static inline BHCSVector3 BHCSVector3Subtract(BHCSVector3 v1, BHCSVector3 v2)
{
    return BHCSVector3Make(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

static inline BHCSVector3 BHCSVector3MultiplyScalar(BHCSVector3 v, float scalar)
{
    return BHCSVector3Make(v.x * scalar, v.y * scalar, v.z * scalar);
}

static inline int BHCSVector3EqualToVector3(BHCSVector3 v1, BHCSVector3 v2)
{
    return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}

static inline BHCSQuaternion BHCSQuaternionMakeWithAngleAndAxis(float angle, float x, float y, float z)
{
    float half = 0.5f * angle;
    float scale = sinf(half);
    
    return (BHCSQuaternion){scale * x, scale * y, scale * z, cosf(half)};
}

static inline BHCSQuaternion BHCSQuaternionInvert(BHCSQuaternion q)
{
    float scale = 1.0f / (q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
    
    return (BHCSQuaternion){-q.x * scale, -q.y * scale, -q.z * scale, q.w * scale};
}

static inline BHCSQuaternion BHCSQuaternionMultiply(BHCSQuaternion q1, BHCSQuaternion q2)
{
    return (BHCSQuaternion){
        q1.w * q2.x +
        q1.x * q2.w +
        q1.y * q2.z -
        q1.z * q2.y,
        
        q1.w * q2.y +
        q1.y * q2.w +
        q1.z * q2.x -
        q1.x * q2.z,
        
        q1.w * q2.z +
        q1.z * q2.w +
        q1.x * q2.y -
        q1.y * q2.x,
        
        q1.w * q2.w -
        q1.x * q2.x -
        q1.y * q2.y -
        q1.z * q2.z};
}

static inline BHCSVector3 BHCSQuaternionRotateVector3(BHCSQuaternion q, BHCSVector3 v)
{
    BHCSQuaternion rq = {v.x, v.y, v.z, 0.0f};
    rq = BHCSQuaternionMultiply(BHCSQuaternionMultiply(q, rq), BHCSQuaternionInvert(q));
    
    return BHCSVector3Make(round(rq.x), round(rq.y), round(rq.z));
}

#endif
