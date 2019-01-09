//
//  MyViewController.m
//  SamsBeeSolving
//
//  Created by Samuel Ocko on 6/9/11.
//  Copyright 2011 MIT. All rights reserved.
//

#import "MyViewController.h"


@implementation MyViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Initialization code here.
        [[NSColor greenColor] set];
        [NSBezierPath fillRect:[[self view] bounds]];
        [[self view] setNeedsDisplay:true];
//        [self view] ->backgroundColor = [NSColor blackColor];

    }
    
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

@end
