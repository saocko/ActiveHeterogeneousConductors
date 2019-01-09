//
//  MyAPMAppDelegate.h
//  MyAPM
//
//  Created by Samuel Ocko on 6/9/11.
//  Copyright 2011 MIT. All rights reserved.
//

#include <Cocoa/Cocoa.h>
#import "MyView.h"
#import "MyAPMSystem.h"
#import "MyAutomator.h"
#import "MyOtherEnums.h"

@interface MyAPMAppDelegate : NSObject <NSApplicationDelegate> {
@private
    
    
    NSLock* _mySysLock;
    MyAutomator* _automator;
    Boolean _isRunning;
    NSWindow* _window;
    NSTextField* _field;    
    NSButton* _toggleButton;
    MyView* _view;
    bool updateViewYourSelf;
    MyAPMSystem * _system;
    NSTimer* _frameTimer;
    NSSlider* _diffusSlider, *_addCoeffSlider, *_playSpeedSlider, *_nParticlesSlider, *_sysSizeSlider, *_grainSizeSlider, *_removCoeffSlider, *_diffusBiasSlider;
    
    NSTextField * _maxMinField, *_timeField, *_diffusField, *_addCoeffField, *_removCoeffField, *_playSpeedField, *_flowField, *_diffusBiasField, *_nParticlesField, *_widthField, *_heightField;
    
    NSButton *_evapFeedbackButton, *_condFeedbackButton, 
    *_randDrivingButton;
    double _frameRate;
    double _playSpeed;
    
    NSMatrix *_stepModeMatrix, *_substrateModeMatrix;
    bool automated_mode;
}





@property (assign) IBOutlet NSWindow *_window;
@property (assign) IBOutlet MyView *_view;
@property (assign) IBOutlet NSTextField *_field;
@property (assign) IBOutlet NSTextField *_maxMinField;
@property (assign) IBOutlet NSTextField *_timeField;
@property (assign) IBOutlet NSTextField *_diffusField;
@property (assign) IBOutlet NSTextField *_diffusBiasField;

@property (assign) IBOutlet NSTextField *_addCoeffField;
@property (assign) IBOutlet NSTextField *_removCoeffField;

@property (assign) IBOutlet NSTextField *_playSpeedField;
@property (assign) IBOutlet NSTextField *_flowField;
@property (assign) IBOutlet NSTextField *_nParticlesField;
@property (assign) IBOutlet NSTextField *_widthField;
@property (assign) IBOutlet NSTextField *_heightField;



@property (assign) IBOutlet NSButton *_toggleButton;
@property (assign) IBOutlet NSButton *_evapFeedbackButton;
@property (assign) IBOutlet NSButton *_condFeedbackButton;
@property (assign) IBOutlet NSButton *_randDrivingButton;

@property (assign) IBOutlet NSSlider *_diffusSlider;
@property (assign) IBOutlet NSSlider *_diffusBiasSlider;

@property (assign) IBOutlet NSSlider *_addCoeffSlider;
@property (assign) IBOutlet NSSlider *_removCoeffSlider;
@property (assign) IBOutlet NSSlider *_playSpeedSlider;
@property (assign) IBOutlet NSSlider *_nParticlesSlider;
@property (assign) IBOutlet NSSlider *_sysSizeSlider;
@property (assign) IBOutlet NSSlider *_grainSizeSlider;


@property (assign) IBOutlet NSMatrix *_stepModeMatrix;
@property (assign) IBOutlet NSMatrix *_substrateModeMatrix;




-(IBAction) diffusSliderChanged: (id) sender;
-(IBAction) diffusBiasSliderChanged: (id) sender;

-(IBAction) addCoeffSliderChanged: (id) sender;
-(IBAction) removCoeffSliderChanged: (id) sender;

-(IBAction) playSpeedSliderChanged: (id) sender;



-(IBAction) printScreenToFile: (id) sender;
-(IBAction) toggleButtonPressed: (id) sender;
-(IBAction) optionButtonPressed: (id) sender;


-(IBAction) singleStep: (id) sender;

-(IBAction) simulButtonPressed: (id) sender;
-(IBAction) otherSimulButtonPressed: (id) sender;

-(IBAction) makeNewSystem: (id) sender;

-(void) updateSystemParameters;
-(void) setIsRunning: (Boolean) run;
- (void)frameTimerPing:(NSTimer *)theTimer;
-(StepMode) curStepMode;
-(SubstrateMode) curSubstrateMode;

@end
