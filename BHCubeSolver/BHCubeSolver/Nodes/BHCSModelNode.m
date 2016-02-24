//
//  BHCSModelNode.m
//  BHCubeSolver
//
//  Created by John Visentin on 2/28/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHCSModelNode.h"

@implementation BHCSModelNode

- (void)configureProgram:(BHGLProgram *)program
{
    [super configureProgram:program];
    
    glUniform1i([program uniformPosition:@"u_MatIndex"], self.material.identifier);
    
    // can use modelView instead if only uniform scaling occurs
    GLKMatrix3 normalMatrix = GLKMatrix3InvertAndTranspose(GLKMatrix4GetMatrix3(program.modelViewMatrix), NULL);
    glUniformMatrix3fv([program uniformPosition:@"u_NormalMatrix"], 1, GL_FALSE, normalMatrix.m);
    
    glUniformMatrix4fv([program uniformPosition:@"u_MVMatrix"], 1, GL_FALSE, program.modelViewMatrix.m);
}

@end
