//
//  MyView.h
//  MyAPM
//
//  Created by Samuel Ocko on 6/9/11.
//  Copyright 2011 MIT. All rights reserved.
//

#include <Cocoa/Cocoa.h>
#import "MyAPMSystem.h"
#import "MySysCellType.h"
#include <stdlib.h>

#import "My2Vec.h"
using namespace std;

@interface MyView : NSView {
@private
    MyAPMSystem *_system;
    NSButton *_showButton;
    NSMutableArray *arrows, *links, *densities;
    vector<vector<NSBezierPath* >   > other_links, other_vert_arrows, other_cells, other_horiz_arrows;
    NSColor* color_array[100];//Bigger than I'll ever need
    NSColor *color0, *color1, *color2, *color3;
}



-(void) setToTriangle: (NSBezierPath *) path Start:(My2Vec) start End:(My2Vec) end Size:(double) size;
-(void) setSystem: (MyAPMSystem*) newsystem;
-(void) printToFile: (NSString*) output_file_path;
//-(NSBezierPath*) arrowAtX: (int) x Y:(int) y;
//-(NSBezierPath*) linkAtX: (int) x Y:(int) y;
//-(NSBezierPath*) getCellAtX: (int) x Y:(int) y;

-(NSColor*) colorForCell: (MySysCellType) c;

@property (assign) IBOutlet NSButton *_showButton;

@end


