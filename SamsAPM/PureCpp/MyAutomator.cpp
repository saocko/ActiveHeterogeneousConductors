//
//  MyAutomator.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 9/13/12.
//  Copyright 2012 MIT. All rights reserved.
//

#include "MyAutomator.h"
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

using namespace std;
MyAutomator::MyAutomator(bool linuxMode_){
    
    cout<<"In MyAutomator constructor !"<<endl;
    int my_rand_seed;
    time((time_t*)&my_rand_seed);
    cout<<"Rand seed is "<<my_rand_seed<<endl;
    srand((int)time(NULL));
    
    
    char input_params_path[500];
    n_MFT_systems =  100;
    

    if(1){        
        //Decides which path to load from
        sprintf(base_path, "/Volumes/SamOckosBackup/APMWorkspace/JulyFinalizing/GillespSuperRuns/Mar4WallPhaseSep");
        sprintf(input_params_path, "%s/APMProgramInput.txt", base_path);        
    }
    
    
    iter_number = 0;

    
        
    diffus = 1;
    bias_diffus = 0;
    
    cout<<"At MyAutomator, densities to do are..."<<endl;
    for (int i = 0; i<densities_and_inds_to_do.size(); i++) {
        cout<<"Density number "<< densities_and_inds_to_do[i].first <<" is "<<densities_and_inds_to_do[i].second<<endl;
    }
    cout<<endl;
    
    //New control system stuff    
    mode = basicParamSweep();
    sprintf(mode.base_path, "%s", base_path);
    
    printf("%s \n", input_params_path);
    readInfoFromFile(input_params_path, mode);  
//    videoizeMode(mode);  
    SnapshotizeMode(mode);
//    linuxizeMode(mode);
//    SimulateAndSnapshotizeMode(mode);
    if(linuxMode_){
        linuxizeMode(mode); 
    }

    char full_path_to_make[500];
    sprintf(full_path_to_make,"%s/%s/", mode.base_path, mode.run_name);
    makeFilePath(full_path_to_make);
    
    char copy_command[500];
    sprintf(copy_command, "cp -rf %s %s/InputParams.txt", input_params_path, full_path_to_make);
    system(copy_command);
    
    
    
    
    //Initializes system stuff. Some of this may be obselete, haven't gotten around to stripping it down yet
    
    
    steps_per_txt_print = 20;
    updates_per_run = 3;
    
    current_frame_number = 0;
    SystemParams sys_params;
    
    sys_params.removFeedback =  true;
    sys_params.addFeedback = true;
    sys_params.randDriv = false;
    sys_params.local_model = mode.local_diffus;
    
    if(sys_params.local_model){
        sys_params.step_mode = AddAndRemovLocalMode;
    }
    else{
        sys_params.step_mode = AddAndRemovNonLocalMode;
    }
    
    sys_params.filled_resistance = 20;
    
    sys_params.width = mode.width;
    sys_params.height = mode.height;    
    
    sys_params.N = (sys_params.width * sys_params.height) * 1./6.;
    //cout<<"N is "<<sys_params.N<<endl;
    
    sys_params.diffus = 1;
    sys_params.bias_diffus = 0;
    sys_params.addCoeff = mode.min_add_coeff;    
    sys_params.removCoeff = mode.min_remov_coeff;
    sys_params.substrate_mode = EmptySubstrateMode;
    

    cur_density_ind_ind = 0;
    for(int i = 0; (mode.min_dens + i * mode.delt_dens)<=(mode.max_dens + MY_EPS); i++){
        densities_and_inds_to_do.push_back(pair<int, double>(i, mode.min_dens + i * mode.delt_dens));                
    }
        
    
    printf("Mode base path is %s, run name is %s, \n", mode.base_path, mode.run_name);
    fillSingleRunParamsList();
    cur_run_params_ind = 0;        
    _system = new MyAPMSystem(sys_params);
    _system->should_plot = true;
    is_finished = false;
    
    
    cout<<"Exiting MyAutomator constructor !"<<endl;
    
}


void MyAutomator::updateCurrentPath(){
    
    char dir_info[500];
    if(sys_params.local_model){
        sprintf(dir_info, "LocalDiff%.2fBias%.2f", sys_params.diffus, sys_params.bias_diffus);
    }
    else{
        //sprintf(dir_info, "RandDrivDiff%.2fBias%.2f", diffus, bias_diffus);
        //        sprintf(dir_info, "FirstRun");
        //       sprintf(dir_info, "SecondRun");        
        //        sprintf(dir_info, "VertDrifDiff%.2fBias%.2f", diffus, bias_diffus);
        sprintf(dir_info, "NonLocal");
    }
    
    sprintf(current_path, "%s/%s/Sweep%d/DensInd%d/", base_path,   dir_info,  current_param_run_num,densities_and_inds_to_do[cur_density_ind_ind].first);
    makeFilePath(current_path);
    
}

char* MyAutomator::getCurrentPath(){
    return current_path;
}

MyAPMSystem* MyAutomator::getSystem(){
    return _system;
}




void MyAutomator::SimulationStep(){
    
    
    cout<<"At Simulation step, (evap, cond) are ("<<_system->removCoeff()<<", "<<_system->addCoeff()<<")"<<endl;
    
    char evolution_txt_printer_path[500];
    sprintf(evolution_txt_printer_path, "%sEvolutionPlotEvapWindTax%.2fcondWindTax%.2f.txt", current_path, _system->removCoeff(), _system->addCoeff());    
    
    char ensemble_txt_printer_path[500];
    sprintf(ensemble_txt_printer_path, "%sEnsemblePlotEvapWindTax%.2fcondWindTax%.2f.txt", current_path, _system->removCoeff(), _system->addCoeff());
    
    char snapshot_state_txt_printer_path[500];
    sprintf(snapshot_state_txt_printer_path, "%sSingleStatePlotEvapWindTax%.2fcondWindTax%.2f.txt", current_path, _system->removCoeff(), _system->addCoeff());
    
    
    MyTxtPrinter evolution_state_printer(evolution_txt_printer_path);
    MyTxtPrinter ensemble_state_printer(ensemble_txt_printer_path);
    MyTxtPrinter snapshot_state_printer(snapshot_state_txt_printer_path);
    
    ensemble_state_printer.printSystemHeader(_system);    
    snapshot_state_printer.printSystemHeader(_system);
    evolution_state_printer.printSystemHeader(_system);
    
    
    
    
    
    
    cout<<"At Iterate"<<endl;
    
    _system->sysParams().substrate_mode = EmptySubstrateMode;
    _system->sysParams().diffus = diffus;
    _system->sysParams().bias_diffus = bias_diffus;
    _system->setNParticles(densities_and_inds_to_do[cur_density_ind_ind].second * (_system->area()));
    _system->resetState();
    int n_steps = fmin( _system->nParticles(), _system->area()-_system->nParticles()) * steps_per_particle_or_hole;
    steps_per_txt_print = fmin( _system->nParticles(), _system->area()-_system->nParticles())/4;
    
    
    
    //    n_steps = 400;
    
    int n_not_recording_steps = (n_steps* 1./2.);
    int n_update = max(n_steps/updates_per_run, 1);
    
    
    DataCollector mean_power_density;
    UpdatePrinter my_print;
    
    for (int i = 0; i<n_steps; i++) {
        if(i%n_update == 0){my_print.updateProg(i * 1./n_steps);}
        _system->step();
        //        cout<<"Here"<<endl;
        
        //  printf("Flow is %f, through power it should be %f \n", _system->circuit()->netYFlow(), _system->totalPower()/_system->height());
        //   printf("flow values are %f, %f \n", _system->circuit()->netXFlow(), _system->circuit()->netYFlow());
        //    printf("Driv values are %f, %f \n", _system->circuit()->uniformDriving.x, _system->circuit()->uniformDriving.y);
        //        cout<<"There"<<endl;
        
        if(i > n_not_recording_steps){
            //   cout<<"recording"<<endl;
            
            double append_value = _system->totalPower()/_system->area();
            // double append_value = _system->circuit()->netYFlow();
            //  double append_value = _system->circuit()->netYFlow()* _system->circuit()->uniformDriving.y + _system->circuit()->netXFlow() * _system->circuit()->uniformDriving.x;
            mean_power_density.append(append_value);
            
        }
        
        if(i%steps_per_txt_print == 0){
            evolution_state_printer.printSystem(_system);
            if(i>n_not_recording_steps){
                ensemble_state_printer.printSystem(_system);
            }
        }
        
        
    }
    
    snapshot_state_printer.printSystem(_system);
    
    
    
    
    sprintf(_system->plot_title, "%sSnapshotEvapWindTax%.2fcondWindTax%.2f", current_path, _system->removCoeff(), _system->addCoeff());
    
    
    
    char buff[300];
    
    sprintf(buff, "%sConductivityEvapWindTax%.2fcondWindTax%.2f.txt", current_path, _system->removCoeff(), _system->addCoeff());
    ofstream myfile;
    myfile.open (buff);
    myfile<<mean_power_density.mean();
    myfile.close();
    cout<<"Printing to File "<<buff <<endl;
    
    
}




void MyAutomator::annealingSimulationStep(){
    
    
    char evolution_txt_printer_path[500];
    sprintf(evolution_txt_printer_path, "%sEvolutionPlotEvapWindTax%.2fcondWindTax%.2f.txt", current_path, _system->removCoeff(), _system->addCoeff());    
    
    char ensemble_txt_printer_path[500];
    sprintf(ensemble_txt_printer_path, "%sEnsemblePlotEvapWindTax%.2fcondWindTax%.2f.txt", current_path, _system->removCoeff(), _system->addCoeff());
    
    char snapshot_state_txt_printer_path[500];
    sprintf(snapshot_state_txt_printer_path, "%sSingleStatePlotEvapWindTax%.2fcondWindTax%.2f.txt", current_path, _system->removCoeff(), _system->addCoeff());
    
    
    MyTxtPrinter evolution_state_printer(evolution_txt_printer_path);
    MyTxtPrinter ensemble_state_printer(ensemble_txt_printer_path);
    MyTxtPrinter snapshot_state_printer(snapshot_state_txt_printer_path);
    
    ensemble_state_printer.printSystemHeader(_system);    
    snapshot_state_printer.printSystemHeader(_system);
    evolution_state_printer.printSystemHeader(_system);
    
    
    
    
    
    
    cout<<"At Iterate"<<endl;
    
    _system->sysParams().substrate_mode = EmptySubstrateMode;
    _system->sysParams().diffus = diffus;
    _system->sysParams().bias_diffus = bias_diffus;
    _system->setNParticles(densities_and_inds_to_do[cur_density_ind_ind].second * (_system->area()));
    _system->resetState();
    int n_steps = fmin( _system->nParticles(), _system->area()-_system->nParticles()) * steps_per_particle_or_hole;
    
    //    n_steps = 400;
    
    int n_not_recording_steps = (n_steps* .5);
    int n_update = max(n_steps/updates_per_run, 1);
    
    
    
    
    
    DataCollector mean_power_density;
    UpdatePrinter my_print;
    
    double total_remov_coeff = _system->sysParams().removCoeff;
    double total_add_coeff = _system->sysParams().addCoeff;
    
    for (int i = 0; i<n_steps; i++) {
        if(i%n_update == 0){my_print.updateProg(i * 1./n_steps);}
        _system->step();
        
        double anneal_coeff = fmin(1, i * 1./n_not_recording_steps);
        
        _system->sysParams().removCoeff = anneal_coeff * total_remov_coeff;
        _system->sysParams().addCoeff = anneal_coeff * total_add_coeff;
        
        //        cout<<"Here"<<endl;
        
        //  printf("Flow is %f, through power it should be %f \n", _system->circuit()->netYFlow(), _system->totalPower()/_system->height());
        //   printf("flow values are %f, %f \n", _system->circuit()->netXFlow(), _system->circuit()->netYFlow());
        //    printf("Driv values are %f, %f \n", _system->circuit()->uniformDriving.x, _system->circuit()->uniformDriving.y);
        //        cout<<"There"<<endl;
        
        if(i > n_not_recording_steps){
            //   cout<<"recording"<<endl;
            
            double append_value = _system->totalPower()/_system->area();
            // double append_value = _system->circuit()->netYFlow();
            //  double append_value = _system->circuit()->netYFlow()* _system->circuit()->uniformDriving.y + _system->circuit()->netXFlow() * _system->circuit()->uniformDriving.x;
            mean_power_density.append(append_value);
            
        }
        
        if(i%steps_per_txt_print == 0){
            evolution_state_printer.printSystem(_system);
            if(i>n_not_recording_steps){
                ensemble_state_printer.printSystem(_system);
            }
        }
        
        
    }
    
    _system->sysParams().removCoeff = total_remov_coeff;
    _system->sysParams().addCoeff = total_add_coeff;
    
    snapshot_state_printer.printSystem(_system);
    
   
    
    
    sprintf(_system->plot_title, "%sSnapshotEvapWindTax%.2fcondWindTax%.2f", current_path, _system->removCoeff(), _system->addCoeff());
    
    
    
    char buff[300];
    
    sprintf(buff, "%sConductivityEvapWindTax%.2fcondWindTax%.2f.txt", current_path, _system->removCoeff(), _system->addCoeff());
    ofstream myfile;
    myfile.open (buff);
    
    myfile<<mean_power_density.mean();
    myfile.close();
    cout<<"Printing to File "<<buff <<endl;
    
    
}







void MyAutomator::randomCircuitTest(){
    
    cout<<"At randomCircuitTest "<<endl;
    MyCircuit circuit(30, 30);
    
    
    
    //Sets the resistances and voltages. 
    for(int x = 0; x < circuit.width(); x++){
        for(int y = 0; y<circuit.height(); y++){
            circuit.setUpResist(x, y, .5 + unitRand());
            circuit.setRightResist(x, y, .5 + unitRand());
            circuit.setUpVoltage(x, y, -.5 + unitRand());
            circuit.setRightVoltage(x, y, -.5 + unitRand());
        }
    }
    
    for(int x = 0; x < circuit.width(); x++){
        for(int y = 0; y<circuit.height(); y++){
            double foo = unitRand();
            MyCircuitCell& curr_cell = circuit.getCellAt(x, y);
            curr_cell.leak_cond = 0;
            curr_cell.leak_voltage = 0;
            curr_cell.has_fixed_voltage = false;
            curr_cell.base_in_plane_divergence = 0;
            
            
            if(foo <.2){
                curr_cell.has_fixed_voltage = true;
                curr_cell.fixed_voltage = -.5 + unitRand();
            }
            else if(foo < .5){
                curr_cell.base_in_plane_divergence = -.5 + unitRand();
            }
            else{
                curr_cell.leak_cond = .5 + unitRand();
                curr_cell.leak_voltage = -.5 + unitRand();
            }
        }
    }    
    
    
    circuit.solve();
    circuit.checkSolution();
    cout<<"Finished randomCircuitTest "<<endl;
    
}




void MyAutomator::generateUniformDensVoltSamples(){
    
    
    
    
    cout<<"At generateUniformDensVoltSamples"<<endl;
    SystemParams sysparams_to_use = meanFieldsysParams();    
    MyAPMSystem sys(sysparams_to_use);
    
    
    char curr_dir_path[500];
    sprintf(curr_dir_path, "%s/MFTSamples/Uniform/", base_path);
    
    
    makeFilePath(curr_dir_path);
    
    
    vector<double> MFT_densities_to_do= generateMFTDensityList();
    char dens_list_txt_path[500];
    sprintf(dens_list_txt_path, "%s/DensValues.txt", curr_dir_path);
    MyTxtPrinter dens_list_printer(dens_list_txt_path);
    dens_list_printer.printDoubleVec(MFT_densities_to_do);
    
    
    for (int dens_ind = 0; dens_ind<MFT_densities_to_do.size(); dens_ind++) {
        double density = MFT_densities_to_do[dens_ind];
        char buff_filled_space[500];
        char buff_empty_space[500];
        char buff_perm[500];
        
        
        
        sprintf(buff_filled_space, "%s/MFTSamples/Uniform/FilledVoltageDens%.4f.txt", base_path, density);
        sprintf(buff_empty_space, "%s/MFTSamples/Uniform/EmptyVoltageDens%.4f.txt", base_path, density);
        sprintf(buff_perm, "%s/MFTSamples/Uniform/PermDens%.4f.txt", base_path, density);
        
        MyTxtPrinter cur_filled_print(buff_filled_space);
        MyTxtPrinter cur_empty_print(buff_empty_space);
        MyTxtPrinter perm_print(buff_perm);
        
        
        double print_prob = .1;
        cout<<"Generating samples for dens "<<density<<endl;
        
        double n_systems = n_MFT_systems;
        //MyProgressPrinter prog_print(n_systems, 10);
        
        for (int i = 0; i<n_systems; i++) {
            //  prog_print.step();
            sys.sysParams().N = round(sys.area() * density);
            sys.resetState();
            
            double upcurrent = sys.totalPower()/sys.area();
            for(int x = 0; x<sys.width(); x++){
                for (int y = 0; y<sys.height(); y++) {
                    if (sys.cellTypeAt(x, y) == ParticleCell) {
                        if(unitRand()<print_prob/density){
                            cur_filled_print.printDouble(sys.voltageThroughNode(x, y)/upcurrent);
                            // cout<<"Full"<<endl;
                        }
                    }
                    else{
                        if(unitRand()<print_prob/(1-density)){
                            cur_empty_print.printDouble(sys.voltageThroughNode(x, y)/upcurrent);
                            //cout<<"Empty"<<endl;                        
                        }
                    }
                }
            }
            perm_print.printDouble(upcurrent);
            
            
        }
        
        
    }
}



void MyAutomator::generateDummyUniformDensVoltSamples(){
    
    
    
    cout<<"At generateDummyUniformDensVoltSamples"<<endl;
    SystemParams sysparams_to_use = meanFieldsysParams();    
    MyAPMSystem sys(sysparams_to_use);
    
    
    char curr_dir_path[500];
    sprintf(curr_dir_path, "%s/MFTSamples/DummyUniform/", base_path);
    
    
    makeFilePath(curr_dir_path);
    
    
    vector<double> MFT_densities_to_do= generateMFTDensityList();
    char dens_list_txt_path[500];
    sprintf(dens_list_txt_path, "%s/DensValues.txt", curr_dir_path);
    MyTxtPrinter dens_list_printer(dens_list_txt_path);
    dens_list_printer.printDoubleVec(MFT_densities_to_do);
    
    
    for (int dens_ind = 0; dens_ind<MFT_densities_to_do.size(); dens_ind++) {
        double density = MFT_densities_to_do[dens_ind];
        
        
        
        
        
        
        char buff_filled_space[500];
        char buff_empty_space[500];
        char buff_perm[500];
        
        
        
        sprintf(buff_filled_space, "%s/MFTSamples/Uniform/FilledVoltageDens%.4f.txt", base_path, density);
        sprintf(buff_empty_space, "%s/MFTSamples/Uniform/EmptyVoltageDens%.4f.txt", base_path, density);
        sprintf(buff_perm, "%s/MFTSamples/Uniform/PermDens%.4f.txt", base_path, density);
        
        MyTxtPrinter cur_filled_print(buff_filled_space);
        MyTxtPrinter cur_empty_print(buff_empty_space);
        MyTxtPrinter perm_print(buff_perm);
        
        
        double print_prob = .1;
        cout<<"Generating MFT Samples For Dens: "<<density<<endl;
        
        double n_systems = n_MFT_systems;
        //MyProgressPrinter prog_print(n_systems, 10);
        
        for (int i = 0; i<n_systems; i++) {
            //  prog_print.step();
            sys.sysParams().N = round(sys.area() * density);
            sys.resetState();
            
            double upcurrent = sys.totalPower()/sys.area();
            for(int x = 0; x<sys.width(); x++){
                for (int y = 0; y<sys.height(); y++) {
                    if (sys.cellTypeAt(x, y) == ParticleCell) {
                        if(unitRand()<print_prob/density){
                            cur_filled_print.printDouble(sys.voltageThroughNode(x, y)/upcurrent);
                            // cout<<"Full"<<endl;
                        }
                    }
                    else{
                        if(unitRand()<print_prob/(1-density)){
                            cur_empty_print.printDouble(sys.voltageThroughNode(x, y)/upcurrent);
                            //cout<<"Empty"<<endl;                        
                        }
                    }
                }
            }
            perm_print.printDouble(upcurrent);
            
            
        }
        
        
    }
}





void MyAutomator::generateDummyCrystalDensVoltSamples(double mean_dens, MyRegionHelper reg1, const char* run_name){
    
    MyRegionHelper reg2 = reg1.inverse();
    cout<<"At generateDummyCrystalDensVoltSamples"<<endl;
    SystemParams sysparams_to_use = meanFieldsysParams();    
    MyAPMSystem sys(sysparams_to_use);
    
    char curr_dir_path[500];
    sprintf(curr_dir_path, "%s/MFTSamples/NonUniform/%s/Dens%.4f/", base_path, run_name, mean_dens);
    
    makeFilePath(curr_dir_path);
    
    vector<double> skews_to_do= generateSkewList(mean_dens, reg1.areaFrac());
    char skew_list_txt_path[500];
    sprintf(skew_list_txt_path, "%s/SkewValues.txt", curr_dir_path);
    MyTxtPrinter skew_list_printer(skew_list_txt_path);
    skew_list_printer.printDoubleVec(skews_to_do);
    
    char reg1_frac_txt_path[500];
    sprintf(reg1_frac_txt_path, "%s/Reg1Frac.txt", curr_dir_path);
    MyTxtPrinter reg1frac_txt_printer(reg1_frac_txt_path);
    reg1frac_txt_printer.printDouble(reg1.areaFrac());    
    
    
    
    
    for (int i = 0; i<skews_to_do.size(); i++) {
        double unitless_skew = skews_to_do[i];
        
        char buff_filled_space_reg1[500];
        char buff_empty_space_reg1[500];            
        char buff_filled_space_reg2[500];
        char buff_empty_space_reg2[500];                    
        char buff_perm[500];
        
        sprintf(buff_filled_space_reg1, "%s/reg1FilledVoltageSkew%.4f.txt", curr_dir_path, unitless_skew);
        sprintf(buff_empty_space_reg1, "%s/reg1EmptyVoltageSkew%.4f.txt", curr_dir_path , unitless_skew);            
        sprintf(buff_filled_space_reg2, "%s/reg2FilledVoltageSkew%.4f.txt", curr_dir_path , unitless_skew);
        sprintf(buff_empty_space_reg2, "%s/reg2EmptyVoltageSkew%.4f.txt", curr_dir_path , unitless_skew);            
        sprintf(buff_perm, "%s/PermSkew%.4f.txt", curr_dir_path ,  unitless_skew);
        
        MyTxtPrinter cur_filled_print_reg1(buff_filled_space_reg1);
        MyTxtPrinter cur_empty_print_reg1(buff_empty_space_reg1);            
        MyTxtPrinter cur_filled_print_reg2(buff_filled_space_reg2);
        MyTxtPrinter cur_empty_print_reg2(buff_empty_space_reg2);            
        MyTxtPrinter perm_print(buff_perm);
        
        cur_empty_print_reg1.printDouble(10 + 2 * unitless_skew);
        cur_empty_print_reg2.printDouble(10 -  2 * unitless_skew);
        cur_filled_print_reg1.printDouble(5. + unitless_skew);
        cur_filled_print_reg2.printDouble(5. - unitless_skew);    
        perm_print.printDouble(1);
        
    }
}




void MyAutomator::generateCrystalDensVoltSamples(double mean_dens, MyRegionHelper  reg1, const char* run_name){
    double n_systems = n_MFT_systems;
    
    MyRegionHelper reg2 = reg1.inverse();
    cout<<"At generateCrystalDensVoltSamples"<<endl;
    SystemParams sysparams_to_use = meanFieldsysParams();    
    MyAPMSystem sys(sysparams_to_use);
    
    char curr_dir_path[500];
    sprintf(curr_dir_path, "%s/MFTSamples/NonUniform/%s/Dens%.4f/", base_path, run_name, mean_dens);
    
    
    makeFilePath(curr_dir_path);
    
    
    vector<double> skews_to_do= generateSkewList(mean_dens, reg1.areaFrac());
    char skew_list_txt_path[500];
    sprintf(skew_list_txt_path, "%s/SkewValues.txt", curr_dir_path);
    MyTxtPrinter skew_list_printer(skew_list_txt_path);
    skew_list_printer.printDoubleVec(skews_to_do);
    
    char reg1_frac_txt_path[500];
    sprintf(reg1_frac_txt_path, "%s/Reg1Frac.txt", curr_dir_path);
    MyTxtPrinter reg1frac_txt_printer(reg1_frac_txt_path);
    reg1frac_txt_printer.printDouble(reg1.areaFrac());    
    
    for (int skew_ind = 0; skew_ind<skews_to_do.size(); skew_ind++) {
        double unitless_skew = skews_to_do[skew_ind];
        
        char buff_filled_space_reg1[500];
        char buff_empty_space_reg1[500];            
        char buff_filled_space_reg2[500];
        char buff_empty_space_reg2[500];                    
        char buff_perm[500];
        
        sprintf(buff_filled_space_reg1, "%s/reg1FilledVoltageSkew%.4f.txt", curr_dir_path, unitless_skew);
        sprintf(buff_empty_space_reg1, "%s/reg1EmptyVoltageSkew%.4f.txt", curr_dir_path , unitless_skew);            
        sprintf(buff_filled_space_reg2, "%s/reg2FilledVoltageSkew%.4f.txt", curr_dir_path , unitless_skew);
        sprintf(buff_empty_space_reg2, "%s/reg2EmptyVoltageSkew%.4f.txt", curr_dir_path , unitless_skew);            
        sprintf(buff_perm, "%s/PermSkew%.4f.txt", curr_dir_path ,  unitless_skew);
        
        MyTxtPrinter cur_filled_print_reg1(buff_filled_space_reg1);
        MyTxtPrinter cur_empty_print_reg1(buff_empty_space_reg1);            
        MyTxtPrinter cur_filled_print_reg2(buff_filled_space_reg2);
        MyTxtPrinter cur_empty_print_reg2(buff_empty_space_reg2);            
        MyTxtPrinter perm_print(buff_perm);
        
        double reg1_dens = mean_dens + unitless_skew/reg1.areaFrac();        
        double reg2_dens = mean_dens - unitless_skew/reg2.areaFrac();
        
        double print_prob = .3;
        cout<<"unitless_skew is "<<unitless_skew<<", Measured Reg1 Frac: "<<sys.numberOfCellsOfTypeWithCrystal(ParticleCell, reg1)/(sys.area() * reg1.areaFrac())<<", Reg2 frac: "<<sys.numberOfCellsOfTypeWithCrystal(ParticleCell, reg2)/(sys.area() * reg2.areaFrac())<< ", TotFrac: "<<sys.numberOfCellsOfType(ParticleCell)/sys.area()<< endl;
        //    MyProgressPrinter prog_print(n_systems, 10);   
        
        for (int i = 0; i<n_systems; i++) {
            //      prog_print.step();
            
            sys.setDensitiesOfCrystal(reg1, reg1_dens);
            sys.setDensitiesOfCrystal(reg2, reg2_dens);
            sys.solveForCurrent();
            
            double upcurrent = sys.totalPower()/sys.area();
            for(int x = 0; x<sys.width(); x++){
                
                for (int y = 0; y<sys.height(); y++) {
                    if(reg1(x, y) == reg2(x,y)){
                        cout<<"Par doesn't match inverse!"<<endl;
                    }
                    
                    if (sys.cellTypeAt(x, y) == ParticleCell) {
                        if (reg1(x,y) and unitRand() < print_prob/reg1_dens) {
                            cur_filled_print_reg1.printDouble(sys.voltageThroughNode(x, y)/upcurrent);
                        }
                        if(reg2(x,y) and unitRand() < print_prob/reg2_dens){
                            cur_filled_print_reg2.printDouble(sys.voltageThroughNode(x, y)/upcurrent);                            
                        }
                    }
                    else{
                        if (reg1(x,y) and unitRand() < print_prob/(1-reg1_dens)) {
                            cur_empty_print_reg1.printDouble(sys.voltageThroughNode(x, y)/upcurrent);
                        }
                        if(reg2(x,y) and unitRand() < print_prob/(1-reg2_dens)){
                            cur_empty_print_reg2.printDouble(sys.voltageThroughNode(x, y)/upcurrent);                            
                        }
                        
                    }
                    
                }
                
                
            }
            perm_print.printDouble(upcurrent);
        }        
        
    }
    
}



void MyAutomator::videoImportAndDisplayStep(){
    cout<<"At videoImportAndDisplayStep"<<endl;
    
    
    
    int dummy;
    for (int x = 0; x < _system->width(); x++) {
        //        video_txt_stream.
        video_txt_stream>>dummy;
        //      cout<<dummy<<"   ";
    }
    //  cout<<"End of buff"<<endl;
    
    
    
    int int_cell_value;
    for (int y = _system->height()-1; y>=0; y--) {
        
        for (int x = 0; x < _system->width(); x++) {
            video_txt_stream>>int_cell_value;
            //            cout<<int_cell_value<<"   ";
            _system->setCellType(x, y, (MySysCellType) int_cell_value);
        }    
        //   cout<<endl;
    }
    
    sprintf(_system->plot_title, "%s/FramesEvapWindTax%.2fcondWindTax%.2f/Frame%.05d", current_path, _system->removCoeff(), _system->addCoeff(), current_frame_number);
    
    cout<<_system->plot_title<<endl;
}



SystemParams MyAutomator::meanFieldsysParams(){
    SystemParams params_to_return;
    
    params_to_return.width = 36;
    params_to_return.height = params_to_return.width;
    params_to_return.filled_resistance = 20;
    
    
    
    params_to_return.removFeedback =  false;
    params_to_return.addFeedback = false;
    params_to_return.randDriv = false;
    
    
    params_to_return.N = (params_to_return.width * params_to_return.height) * 1./6.;
    cout<<"N is "<<params_to_return.N<<endl;
    params_to_return.dt = 1;
    
    
    params_to_return.addCoeff = 0;
    
    params_to_return.removCoeff = 0;
    
    params_to_return.substrate_mode = EmptySubstrateMode;
    params_to_return.step_mode = AddAndRemovNonLocalMode;
    
    return params_to_return;
}



void MyAutomator::generateMFTFiles(){
    
    cout<<"At generateMFTFiles()"<<endl;
    
    generateUniformDensVoltSamples();
    for (int i = 0; i<densities_and_inds_to_do.size(); i++) {
        double cur_mean_dens = densities_and_inds_to_do[i].second;
        MyRegionHelper horiz(1, 2, 0);
        MyRegionHelper vert(0, 2, 0);
        
        
        MyRegionHelper horiz_mod4(1, 4, 0);
        MyRegionHelper vert_mod4(0, 4, 0);
        if(1){
            generateCrystalDensVoltSamples(cur_mean_dens, horiz_mod4, "4/ThinChan");
            generateCrystalDensVoltSamples(cur_mean_dens, vert_mod4, "4/ThinWall");            
            
            generateCrystalDensVoltSamples(cur_mean_dens, horiz, "2/ThinChan");
            generateCrystalDensVoltSamples(cur_mean_dens, vert, "2/ThinWall");
            
        }
        else{
            generateDummyCrystalDensVoltSamples(cur_mean_dens, horiz, "2/ThinChan");
            generateDummyCrystalDensVoltSamples(cur_mean_dens, vert, "2/ThinWall");
        }
        
    }
    
    
    cout<<"Reached end of generateMFTFiles()"<<endl;
    
}