//
//  BHCSView.m
//  BHCubeSolver
//
//  Created by John Visentin on 2/28/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHCSView.h"
#import "BHCSScene.h"
#import "BHGL.h"
#import "BHCSPuzzleDocument.h"

@interface BHCSView ()

@property (nonatomic, weak) IBOutlet NSButton *bruteButton;
@property (nonatomic, weak) IBOutlet NSButton *improvedButton;
@property (nonatomic, weak) IBOutlet NSButton *unpackButton;
@property (nonatomic, strong) NSTimer *timer;

- (void)updateGL:(NSTimer *)timer;

- (NSString *)worldString;

- (IBAction)solveBrute:(NSButton *)sender;
- (IBAction)solveImproved:(NSButton *)sender;
- (IBAction)togglePacked:(NSButton *)sender;

@end

@implementation BHCSView

- (void)mouseDown:(NSEvent *)theEvent
{
    [((BHCSScene *)self.scene) stopRotation];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    CGFloat dx = [theEvent deltaX] / self.bounds.size.width;
    CGFloat dy = [theEvent deltaY] / self.bounds.size.height;
    
    BHGLBasicAnimation *rotateY = [BHGLBasicAnimation rotateBy:GLKQuaternionMakeWithAngleAndAxis(M_PI*dx, 0.0f, 1.0f, 0.0f)];
    
    BHGLBasicAnimation *rotateX = [BHGLBasicAnimation rotateBy:GLKQuaternionMakeWithAngleAndAxis(M_PI*dy, 1.0f, 0.0f, 0.0f)];
    
    [((BHCSScene *)self.scene).worldNode runAnimation:[BHGLAnimation group:@[rotateX, rotateY]]];
}

- (void)mouseUp:(NSEvent *)theEvent
{
    [((BHCSScene *)self.scene) startRotation];
}

- (NSString *)worldString
{
    BHCSPuzzleDocument *document = [[NSDocumentController sharedDocumentController] documentForWindow:[self window]];
    
    return document.worldString;
}

- (void)solveBrute:(NSButton *)sender
{
    NSString *title = [sender title];
    [sender setTitle:@"Solving..."];
    
    [self.bruteButton setEnabled:NO];
    [self.improvedButton setEnabled:NO];
    
    [self.unpackButton setHidden:YES];
    
    BHCSWorld *world = [[BHCSWorld alloc] initWithString:[self worldString]];
    
    __weak NSButton *wbrute = self.bruteButton;
    __weak NSButton *wimproved = self.improvedButton;
    __weak NSButton *wunpack = self.unpackButton;
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
        [world solveBrute];
        
        [sender setTitle:title];
        
        [wbrute setEnabled:YES];
        [wimproved setEnabled:YES];
        [wunpack setHidden:NO];
    });
    
    ((BHCSScene *)self.scene).world = world;
}

- (void)solveImproved:(NSButton *)sender
{
    NSString *title = [sender title];
    [sender setTitle:@"Solving..."];
    
    [self.bruteButton setEnabled:NO];
    [self.improvedButton setEnabled:NO];
    
    [self.unpackButton setHidden:YES];
    
    BHCSWorld *world = [[BHCSWorld alloc] initWithString:[self worldString]];
    
    __weak NSButton *wbrute = self.bruteButton;
    __weak NSButton *wimproved = self.improvedButton;
    __weak NSButton *wunpack = self.unpackButton;
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
        [world solveAlgorithmX];
        
        [sender setTitle:title];
        
        [wbrute setEnabled:YES];
        [wimproved setEnabled:YES];
        [wunpack setHidden:NO];
    });
    
    ((BHCSScene *)self.scene).world = world;
}
- (void)togglePacked:(NSButton *)sender
{
    if ([[sender title] isEqualToString:@"Unpack"] && ((BHCSScene *)self.scene).worldNode.isPacked)
    {
        [((BHCSScene *)self.scene).worldNode unpack];
        
        [sender setTitle:@"Pack"];
    }
    else if ([[sender title] isEqualToString:@"Pack"] && !((BHCSScene *)self.scene).worldNode.isPacked)
    {
        [((BHCSScene *)self.scene).worldNode pack];
        
        [sender setTitle:@"Unpack"];
    }
}

- (void)awakeFromNib
{
    [super awakeFromNib];
    
    NSOpenGLPixelFormatAttribute attrs[] = {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 24,
        // Must specify the 3.2 Core Profile to use OpenGL 3.2
        NSOpenGLPFAOpenGLProfile,
        NSOpenGLProfileVersion3_2Core,
        0
    };
    
    NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    
    if (!pf)
    {
        NSLog(@"No OpenGL pixel format");
    }
    
    NSOpenGLContext* context = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
    
    [self setPixelFormat:pf];
    
    [self setOpenGLContext:context];
    
    [self.unpackButton setHidden:YES];
}

- (void)viewDidMoveToWindow
{
    if (![self window])
    {
        [self.timer invalidate];
        self.timer = nil;
        
        [self clearGLContext];
        
        ((BHCSScene *)self.scene).world = nil;
    }
}

- (void)prepareOpenGL
{
    self.scene = [[BHCSScene alloc] init];
    ((BHCSScene *)self.scene).world = [[BHCSWorld alloc] initWithSize:BHCSVector3Make(1, 1, 1)];
    
    self.timer = [NSTimer timerWithTimeInterval:1.0/40.0 target:self selector:@selector(updateGL:) userInfo:nil repeats:YES];
    [[NSRunLoop currentRunLoop] addTimer:self.timer forMode:NSDefaultRunLoopMode];
    
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.4f, 0.6f, 0.8f, 0.0f);    
}

- (void)updateGL:(NSTimer *)timer
{
    if(![[NSApplication sharedApplication] isHidden])
    {
        [self.scene updateRecursive:[timer timeInterval]];
            
        [self setNeedsDisplay:YES];
    }
}

/** Simply sets the new viewport */
- (void)reshape
{
    [super reshape];
    
    glViewport(0, 0, self.bounds.size.width, self.bounds.size.height);
}

- (void)drawRect:(NSRect)dirtyRect
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    [self.scene render];
    
    [[self openGLContext] flushBuffer];
}

@end
