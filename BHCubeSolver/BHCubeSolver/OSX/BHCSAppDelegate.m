//
//  BHCSAppDelegate.m
//  BHCubeSolver
//
//  Created by John Visentin on 2/28/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHCSAppDelegate.h"

@implementation BHCSAppDelegate

- (void)awakeFromNib
{
    [super awakeFromNib];
    [NSApp setDelegate:self];
}

- (BOOL)applicationShouldOpenUntitledFile:(NSApplication *)sender
{
    return NO;
}

@end
