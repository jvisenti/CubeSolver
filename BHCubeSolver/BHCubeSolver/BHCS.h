//
//  BHCS.h
//  BHCubeSolver
//
//  Created by John Visentin on 3/1/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#ifndef BHCubeSolver_BHCS_h
#define BHCubeSolver_BHCS_h

typedef union _BHCSVector3
{
    struct {int x, y, z;};
    struct {int w, h, d;};
} BHCSVector3;

static inline BHCSVector3 BHCSVector3Make(int x, int y, int z)
{
    return (BHCSVector3){x, y, z};
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

#endif
