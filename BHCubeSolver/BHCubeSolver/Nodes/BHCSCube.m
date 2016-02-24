//
//  BHCSCube.m
//  BHCubeSolver
//
//  Created by John Visentin on 2/28/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHCSCube.h"
#import "BHGLCUtils.h"

const GLfloat Vertices[24][6] = {
    // Front
    {0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    {-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f},
    // Back
    {0.5, -0.5, -0.5f, 0.0f, 0.0f, -1.0f},
    {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
    {-0.5, 0.5, -0.5f, 0.0f, 0.0f, -1.0f},
    {0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f},
    // Left
    {-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f},
    {-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f},
    // Right
    {0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
    {0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f},
    {0.5f, 0.5, 0.5, 1.0f, 0.0f, 0.0f},
    {0.5, -0.5, 0.5, 1.0f, 0.0f, 0.0f},
    // Top
    {0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f},
    {0.5, 0.5, -0.5, 0.0f, 1.0f, 0.0f},
    {-0.5, 0.5, -0.5, 0.0f, 1.0f, 0.0f},
    {-0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f},
    // Bottom
    {0.5, -0.5, -0.5, 0.0f, -1.0f, 0.0f},
    {0.5, -0.5, 0.5, 0.0f, -1.0f, 0.0f},
    {-0.5, -0.5, 0.5, 0.0f, -1.0f, 0.0f},
    {-0.5, -0.5f, -0.5, 0.0f, -1.0f, 0.0f}
};

const GLubyte Indices[] = {
    // Front
    0, 1, 2,
    2, 3, 0,
    // Back
    4, 5, 6,
    6, 7, 4,
    // Left
    8, 9, 10,
    10, 11, 8,
    // Right
    12, 13, 14,
    14, 15, 12,
    // Top
    16, 17, 18,
    18, 19, 16,
    // Bottom
    20, 21, 22,
    22, 23, 20
};

@interface BHCSCube ()

+ (BHGLMesh *)sharedMesh;

@end

@implementation BHCSCube

static BHGLMesh *SharedMesh;
+ (BHGLMesh *)sharedMesh
{
    static BHGLMesh *SharedMesh;
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        
        BHGLVertexType vType = BHGLVertexTypeCreate(2);
        vType.attribs[0] = BHGLVertexAttribPosition;
        vType.types[0] = GL_FLOAT;
        vType.lengths[0] = 3;
        vType.normalized[0] = GL_FALSE;
        vType.offsets[0] = (GLvoid *)0;
        
        vType.attribs[1] = BHGLVertexAttribNormal;
        vType.types[1] = GL_FLOAT;
        vType.lengths[1] = 3;
        vType.normalized[1] = GL_FALSE;
        vType.offsets[1] = (GLvoid *)(3*sizeof(GLfloat));
        
        vType.stride = sizeof(Vertices[0]);
        
        SharedMesh = [[BHGLMesh alloc] initWithVertexData:Vertices vertexDataSize:sizeof(Vertices) vertexType:&vType indexData:Indices indexDataSize:sizeof(Indices) indexType:GL_UNSIGNED_BYTE];
        
        BHGLVertexTypeFree(vType);
    });
    
    return SharedMesh;
}

- (id)init
{
    if ((self = [super init]))
    {
        self.mesh = [BHCSCube sharedMesh];
    
        self.material.surfaceColor = BHGLColorBlack;
        self.material.ambientColor = BHGLColorWhite;
        self.material.diffuseColor = BHGLColorWhite;
        self.material.specularColor = BHGLColorMake(0.6f, 0.6f, 0.6f, 1.0f);
        self.material.shininess = 10.0f;
    }
    
    return self;
}

- (void)configureProgram:(BHGLProgram *)program
{
    [super configureProgram:program];
    
    [program setUniform:@"u_Material" withMaterial:self.material];
}

- (void)render
{
    if (!self.isHidden)
    {
        [super render];
    }
}

- (void)renderWithProgram:(BHGLProgram *)program
{
    if (!self.isHidden)
    {
        [super renderWithProgram:program];
    }
}

@end
