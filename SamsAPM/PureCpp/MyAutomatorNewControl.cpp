//
//  MyAutomatorNewControl.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 9/24/13.
//  Copyright 2013 MIT. All rights reserved.
//


#import "MyAutomator.h"
#include <stdio.h>
#include <math.h>
#include <iostream>
#import "CppHelper.h"
#import "DataCollector.h"
#include <fstream>
#import "UpdatePrinter.h"

#import "MyTxtPrinter.h"
#import "MyRegionHelper.h"
#import "MyProgressPrinter.h"
#import "FilePathHelpers.h"



void MyAutomator::iterate(){
    
    
    _system->should_plot = true;        
    
    cout<<"At new iterate"<<endl;
    if(is_finished){
        _system->should_plot = false;        
        return;
    }
    
    
    
    if(!(mode.video_mode)){
        cout<<"cur_run_params_ind is "<<cur_run_params_ind<<" Out of "<< single_run_params_list.size()<< endl;
        MySingleRunParams cur_run_params = single_run_params_list[cur_run_params_ind];
        //cout<<"Doing path name "<<cur_run_params.path_name<<endl;
        makeFilePath(cur_run_params.path_name);
        if(mode.simulate_mode){
            newSimulationStep(cur_run_params);    
        }
        else{
            importAndDisplayStep(cur_run_params);            
        }
        cur_run_params_ind++;            
        iter_number +=1;
        if(cur_run_params_ind >= single_run_params_list.size()){
            is_finished = true;
            cout<<"Just finished!"<<endl;
            return;
        }            
        
    }
    else{
        
        cout<<"Trying video stuff framestodo: "<< video_frames_to_do <<", Cur frame is " <<current_frame_number<< endl;
        MySingleRunParams cur_run_params;
        
        if(iter_number == 0 or current_frame_number>=video_frames_to_do){
            current_frame_number = 0;
            if(iter_number !=0){
                cur_run_params_ind++;
                //Need to update the run params
                if(cur_run_params_ind >= single_run_params_list.size()){
                    is_finished = true;
                    cout<<"Just finished!"<<endl;
                    return;
                }
                
            }
            cur_run_params = single_run_params_list[cur_run_params_ind];                
            
            loadAndPrimeVideoFile(cur_run_params);
            makeFilePath(cur_run_params.path_name);            
            iter_number +=1;            
        }
        cur_run_params = single_run_params_list[cur_run_params_ind];                
        
        newVideoImportAndDisplayStep(cur_run_params);
        current_frame_number++;        
        
    }


    
    
}



void MyAutomator::importAndDisplayStep(MySingleRunParams params){
    cout<<"At new importAndDisplayStep"<<endl;
    char snapshot_state_txt_path[500];
    sprintf(snapshot_state_txt_path, "%sSingleStatePlotEvapWindTax%.2fcondWindTax%.2f.txt", params.path_name, params.remov_coeff, params.add_coeff);
    
    
    ifstream infile(snapshot_state_txt_path);
    char error_message[500];
    sprintf(error_message,  "Couldn't open file %s !", snapshot_state_txt_path);
    MyAssert(infile, error_message);
    
    int dummy;
    for (int x = 0; x < _system->width(); x++) {
        infile>>dummy;
        cout<<dummy<<"   ";
    }
    cout<<endl;
    
    int int_cell_value;
    for (int y = _system->height()-1; y>=0; y--) {
        
        for (int x = 0; x < _system->width(); x++) {
            infile>>int_cell_value;
            cout<<int_cell_value<<"   ";
            _system->setCellType(x, y, (MySysCellType) int_cell_value);
        }    
        cout<<endl;
    }
    
    
    _system->solveForCurrent();
    sprintf(_system->plot_title, "%sSnapshotEvapWindTax%.2fcondWindTax%.2f", params.path_name, params.remov_coeff, params.add_coeff);
    
    
}



void MyAutomator::newSimulationStep(MySingleRunParams params){
    
    _system->setRemovCoeff(params.remov_coeff);
    _system->setAddCoeff(params.add_coeff);
    _system->setNParticles(params.density*_system->area());
    _system->sysParams().substrate_mode = EmptySubstrateMode;
    _system->resetState();
    
    
    cout<<"At new Simulation step, (evap, cond) are ("<<params.remov_coeff<<", "<<params.add_coeff<<")"<<endl;
    //printf("Path name is %s \n", params.path_name);
    //cout<<"Path is "<<params.path_name<<endl;
    char evolution_txt_printer_path[500];
    sprintf(evolution_txt_printer_path, "%sEvolutionPlotEvapWindTax%.2fcondWindTax%.2f.txt", params.path_name, params.remov_coeff, params.add_coeff);    
    
    char ensemble_txt_printer_path[500];
    sprintf(ensemble_txt_printer_path, "%sEnsemblePlotEvapWindTax%.2fcondWindTax%.2f.txt", params.path_name, params.remov_coeff, params.add_coeff);
    
    char snapshot_state_txt_printer_path[500];
    sprintf(snapshot_state_txt_printer_path, "%sSingleStatePlotEvapWindTax%.2fcondWindTax%.2f.txt", params.path_name, params.remov_coeff, params.add_coeff);
    
    
    MyTxtPrinter evolution_state_printer(evolution_txt_printer_path);
    MyTxtPrinter ensemble_state_printer(ensemble_txt_printer_path);
    MyTxtPrinter snapshot_state_printer(snapshot_state_txt_printer_path);
    
    ensemble_state_printer.printSystemHeader(_system);    
    snapshot_state_printer.printSystemHeader(_system);
    evolution_state_printer.printSystemHeader(_system);
        
    
    int n_steps = fmin( _system->nParticles(), _system->area()-_system->nParticles()) * params.steps_per_particle_or_hole;
    
    int n_not_recording_steps = (n_steps* params.frac_not_recording_steps);
    int n_update = max(n_steps/updates_per_run, 1);
    
    
    DataCollector mean_power_density;
    UpdatePrinter my_print;
    
//    cout<<"N steps "<<n_steps<<endl;
    
    for (int i = 0; i<n_steps; i++) {
        if(i%n_update == 0){my_print.updateProg(i * 1./n_steps);}
        _system->step();
        //cout<<"Steps per print "<<params.steps_per_txt_print<<endl;
        
        
        if(i > n_not_recording_steps){            
            double append_value = _system->totalPower()/_system->area();
            mean_power_density.append(append_value);            
        }
        
        if((i%(params.steps_per_txt_print)) == 0){
        //    cout<<"Printing system"<<endl;
            evolution_state_printer.printSystem(_system);
            if(i>n_not_recording_steps){
                ensemble_state_printer.printSystem(_system);
            }
        }
    }
    
    snapshot_state_printer.printSystem(_system);
    
    sprintf(_system->plot_title, "%sSnapshotEvapWindTax%.2fcondWindTax%.2f", params.path_name, params.remov_coeff, params.add_coeff);
    
    
    
    char permeability_path[500];    
    sprintf(permeability_path, "%sConductivityEvapWindTax%.2fcondWindTax%.2f.txt", params.path_name, params.remov_coeff, params.add_coeff);
    MyTxtPrinter permability_printer(permeability_path);
    permability_printer.printDouble(mean_power_density.mean());    
    cout<<"Finished new simulation step"<<endl;
}




void MyAutomator::loadAndPrimeVideoFile(MySingleRunParams params){
    char evolution_txt_load_path[500];
    
    sprintf(evolution_txt_load_path, "%sEvolutionPlotEvapWindTax%.2fcondWindTax%.2f.txt", params.path_name, params.remov_coeff, params.add_coeff);    
    //    video_txt_stream.init(evolution_txt_load_path, 'r');
    if(iter_number>0){
        video_txt_stream.close();
        //Not quite sure what this does
    }
    
    
    double cur_dens = params.density;
    int total_steps = round(_system->area() * fmin(cur_dens, 1-cur_dens) * params.steps_per_particle_or_hole);
    
    
    cout<<"Total Steps "<<total_steps<<endl;
    cout<<"params.steps_per_txt_print "<<params.steps_per_txt_print<<endl;
    video_frames_to_do = floor(total_steps/params.steps_per_txt_print);
    
    
    video_txt_stream.open(evolution_txt_load_path);
    
    
    char error_message[500];
    sprintf(error_message,  "Couldn't open file %s !", evolution_txt_load_path);
    MyAssert(video_txt_stream, error_message);
    
    
    current_frame_number = 0;
    
    
    
    char mk_dir_command[500];    
    sprintf(mk_dir_command, "mkdir -p %s/FramesEvapWindTax%.2fcondWindTax%.2f/", params.path_name, params.remov_coeff, params.add_coeff);    
    system(mk_dir_command);
    
}


void MyAutomator::newVideoImportAndDisplayStep(MySingleRunParams params){
    cout<<"At newVideoImportAndDisplayStep"<<endl;
    
    
    
    int dummy;
    for (int x = 0; x < _system->width(); x++) {
        //        video_txt_stream.
        video_txt_stream>>dummy;
       //       cout<<dummy<<"   ";
    }
    //cout<<endl;
    //  cout<<"End of buff"<<endl;
    
    
    
    int int_cell_value;
    for (int y = _system->height()-1; y>=0; y--) {
        
        for (int x = 0; x < _system->width(); x++) {
            video_txt_stream>>int_cell_value;
            //            cout<<int_cell_value<<"   ";
            _system->setCellType(x, y, (MySysCellType) int_cell_value);
        }    
           //cout<<endl;
    }
    
    sprintf(_system->plot_title, "%s/FramesEvapWindTax%.2fcondWindTax%.2f/Frame%.06d", params.path_name, params.remov_coeff, params.add_coeff, current_frame_number);
    _system->solveForCurrent();
    
//    cout<<_system->plot_title<<endl;
}



