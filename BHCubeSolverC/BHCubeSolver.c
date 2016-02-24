//
//  BHCubeSolver.c
//  XCodeLab2
//
//  Created by John Visentin on 3/29/14.
//  Copyright (c) 2014 Brockenhaus Studio. All rights reserved.
//

#include <stdio.h>
#include "BHCSWorld.h"

int main(int argc, char** argv)
{
    BHCSWorld *world = BHCSWorldCreate();
    
    int numSolutions = BHCSWorldFindAllSolutions(world);
    
    printf("total solutions: %i\n", numSolutions);
    
    BHCSWorldFree(world);
}