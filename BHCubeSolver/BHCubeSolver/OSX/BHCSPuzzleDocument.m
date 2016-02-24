//
//  BHCSPuzzleDocument.m
//  BHCubeSolver
//
//  Created by John Visentin on 3/3/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#import "BHCSPuzzleDocument.h"

@implementation BHCSPuzzleDocument

- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError *__autoreleasing *)outError
{
    BOOL success = NO;
    
    self.worldString = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    if (!self.worldString)
    {
        *outError = [NSError errorWithDomain:NSCocoaErrorDomain code:NSFileReadUnknownError userInfo:nil];
    }
    else
    {
        success = YES;
    }

    return success;
}

- (NSString *)windowNibName
{
    return NSStringFromClass([self class]);
}

@end
