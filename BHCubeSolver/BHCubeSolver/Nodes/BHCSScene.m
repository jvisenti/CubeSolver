//
//  BHCSScene.m
//  BHCubeSolver
//
//  Created by John Visentin on 2/28/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHCSScene.h"
#import "BHCSWorldNode.h"
#import "BHGLBasicAnimation.h"
#import <QuartzCore/QuartzCore.h>

@interface BHCSScene ()

- (void)setupProgram;
- (void)setupCamera;
- (void)setupLights;

@end

@implementation BHCSScene

- (id)init
{
    if ((self = [super init]))
    {
        [self setupProgram];
        [self setupCamera];
        [self setupLights];
    }
    return self;
}

- (void)setupProgram
{
    BHGLProgram *program = [[BHGLProgram alloc] initWithVertexShaderNamed:@"lighting.vsh" fragmentShaderNamed:@"lighting.fsh"];
    program.mvpUniformName = kBHGLMVPUniformName;
    
    [program setVertexAttribute:BHGLVertexAttribPosition forName:kBHGLPositionAttributeName];
    [program setVertexAttribute:BHGLVertexAttribNormal forName:kBHGLNormalAttributeName];
    
    if ([program link])
    {
        self.program = program;
    }
}

- (void)setupCamera
{
    [self addCamera:[[BHGLCamera alloc] initWithFieldOfView:GLKMathDegreesToRadians(40) aspectRatio:1.0 nearClippingPlane:0.01 farClippingPlane:20]];
}

- (void)setupLights
{
    BHGLLight *light = [[BHGLLight alloc] init];
    light.type = BHGLLightTypePoint;
    light.ambientColor = BHGLColorMake(0.6f, 0.6f, 0.6f, 1.0f);
    light.diffuseColor = BHGLColorWhite;
    light.position = GLKVector3Make(0.0f, 3.0f, -10.0f);
    light.constantAttenuation = 0.6f;
    light.linearAttenuation = 0.02f;
    light.quadraticAttenuation = 0.08f;
    
    [self addLight:light];
    
    self.lightUniform = @"u_Lights";
}

#pragma mark - public interface

- (void)setWorld:(BHCSWorld *)world
{
    _world = world;
    
    GLKQuaternion currentRotation = self.worldNode ? self.worldNode.rotation : GLKQuaternionIdentity;
    
    [self stopRotation];
    [self.worldNode removeFromParent];
    
    if (world)
    {
        _worldNode = [[BHCSWorldNode alloc] initWithWorld:world];
        
        int size = MAX(MAX(world.size.w, world.size.h), world.size.d);
        self.worldNode.position = GLKVector3Make(0.0f, 0.0f, -15.0f);
        self.worldNode.scale = GLKVector3Make(4.0f/size, 4.0f/size, 4.0f/size);
        self.worldNode.rotation = currentRotation;
        
        [self addChild:self.worldNode];
        
        [self startRotation];
    }
}

- (void)startRotation
{
    [self stopRotation];
    
    __weak BHCSWorldNode *wnode = self.worldNode;
    BHGLBasicAnimation *rotate = [BHGLBasicAnimation runBlock:^{
        wnode.rotation = GLKQuaternionMultiply(GLKQuaternionMakeWithAngleAndAxis(0.005*M_PI, 0.0f, 1.0f, 0.0f), self.worldNode.rotation);
    }];
    rotate.repeats = YES;
    
    [self.worldNode runAnimation:rotate];
}

- (void)stopRotation
{
    [self.worldNode removeAllAnimations];
}

- (void)configureProgram:(BHGLProgram *)program
{
    [super configureProgram:program];
    
    // does this need to be transformed??
    GLKVector3 eye = GLKVector3Make(0.0f, 0.0f, 1.0f);
    
    glUniform3fv([self.program uniformPosition:@"u_EyeDirection"], 1, eye.v);
}

@end
