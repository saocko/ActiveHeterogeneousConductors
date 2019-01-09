//
//  MyAutomator.h
//  MyAPM
//
//  Created by Samuel Ocko on 9/13/12.
//  Copyright 2012 MIT. All rights reserved.
//

#include <fstream>
#import "MyAPMSystem.h"
#import "MyAutomatorHelpers.h"
#include <stdlib.h>
using namespace std;





class MyAutomator {
    ;

    double evapWindTax;
    double condWindTax;
    int iter_number;
    double min_remov_coeff, max_remov_coeff, delta_remov_coeff;
    double min_add_coeff, max_add_coeff, delta_add_coeff;
    
    char current_path[500];
    double steps_per_particle_or_hole;
    int steps_per_txt_print, updates_per_run;
    
    
    int current_frame_number;
    ifstream video_txt_stream;
    int video_frames_to_do;
    int n_sweeps, current_param_run_num;
    
    MyAPMSystem* _system;
    bool is_finished;
    
    
    
    vector<pair<int, double> > densities_and_inds_to_do;
    
    
    vector<MySingleRunParams> single_run_params_list;
    MyAutomatorMode mode;
    int cur_run_params_ind;
    
    
    
    int cur_density_ind_ind;
    
    vector<double> diffus_to_do;
    int cur_diffus_index;
    
    double diffus, bias_diffus;
    char base_path[500];
    int n_MFT_systems;

    SystemParams sys_params;
    
public:
    MyAutomator(bool linuxMode_);
    MyAPMSystem* getSystem();
    bool isFinished(){return is_finished;}
    char* getCurrentPath();
    
    
    SystemParams meanFieldsysParams();
    
    void updateCurrentPath();
    void fillSingleRunParamsList();
    void iterate();
    
    
    
    void annealingSimulationStep();
    void SimulationStep();
    void newSimulationStep(MySingleRunParams params);
    
    void importAndDisplayStep(MySingleRunParams params);
    void loadAndPrimeVideoFile(MySingleRunParams params);
    void newVideoImportAndDisplayStep(MySingleRunParams params);
    
    void videoImportAndDisplayStep();
    void generateUniformDensVoltSamples();
    void generateDummyUniformDensVoltSamples();
    
    void generateCrystalDensVoltSamples(double mean_dens, MyRegionHelper reg1, const char* run_name);
    void generateDummyCrystalDensVoltSamples(double mean_dens, MyRegionHelper  reg1, const char* run_name);

    
    void randomCircuitTest();
    
    void generateMFTFiles();
    
    
    vector<double> generateMFTDensityList();    
    vector<double> generateSkewList(double meandens, double reg1frac);
};





