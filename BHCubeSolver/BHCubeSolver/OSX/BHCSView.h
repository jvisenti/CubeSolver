//
//  BHCSView.h
//  BHCubeSolver
//
//  Created by John Visentin on 2/28/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class BHGLScene;

@interface BHCSView : NSOpenGLView

@property (nonatomic, strong) BHGLScene *scene;

@end
