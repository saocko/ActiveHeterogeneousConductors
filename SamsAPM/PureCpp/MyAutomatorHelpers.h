//
//  MyAutomatorHelpers.h
//  MyAPM
//
//  Created by Samuel Ocko on 9/23/13.
//  Copyright 2013 MIT. All rights reserved.
//



struct MySingleRunParams {
    double remov_coeff, add_coeff;
    double density;
    int dens_ind;
    int sweep_num;
    char path_name[500];    
    
    double steps_per_particle_or_hole;  
    int steps_per_txt_print;
    double frac_not_recording_steps;
};


struct MyAutomatorMode {
    double min_remov_coeff, max_remov_coeff, delt_remov_coeff;
    double min_add_coeff, max_add_coeff, delt_add_coeff;
    double min_dens, max_dens, delt_dens;
    
        
    bool video_mode;
    bool annealing_mode;
    bool simulate_mode;
    bool isLinuxMode;
    
    char base_path[500];
    char run_name[500];
    
    int n_sweeps;    
    int steps_per_txt_print;
    double steps_per_particle_or_hole;
    double frac_not_recording_steps;
    
    int width, height;
    bool local_diffus;
};

void readInfoFromFile(const char* filename, MyAutomatorMode& mode_to_modify);

MyAutomatorMode basicParamSweep();
void linuxizeMode(MyAutomatorMode& mode);
void SnapshotizeMode(MyAutomatorMode &mode);
void videoizeMode(MyAutomatorMode& mode);
void SimulateAndSnapshotizeMode(MyAutomatorMode& mode);