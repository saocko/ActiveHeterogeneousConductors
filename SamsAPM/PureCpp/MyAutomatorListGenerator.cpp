//
//  MyAutomatorListGenerator.cpp
//  MyAPM
//
//  Created by Samuel Ocko on 1/15/13.
//  Copyright 2013 MIT. All rights reserved.
//

#include "MyAutomator.h"
#include <vector>
#include <algorithm>
#include <math.h>
#import "CppHelper.h"



using namespace std;

void MyAutomator::fillSingleRunParamsList(){
    
    //Fills a list of run parameters which will then be iterated through. When simulating, it calls for the next simulation, and then displays the system snapshot. Therefore, I put the for loop here, rather than in the code running the simulations, because this gives me a list that's easy to iterate through. 
    
    cout<<"At fillSingleRunParamsList"<<endl;
    char dir_info[500];
    if(sys_params.local_model){
        sprintf(dir_info, "%s/LocalDiff%.2fBias%.2f", mode.run_name, sys_params.diffus, sys_params.bias_diffus);
    }
    else{
        sprintf(dir_info, "%s/NonLocal", mode.run_name);
    }    
    
    for(int sweep_num = 0; sweep_num<mode.n_sweeps; sweep_num++){
        for (int dens_ind_ind = 0; dens_ind_ind<densities_and_inds_to_do.size(); dens_ind_ind++) {
            
            
            for(double remov_coeff = mode.min_remov_coeff; remov_coeff<=(mode.max_remov_coeff + MY_EPS); remov_coeff+=mode.delt_remov_coeff){
                
                
                for(double add_coeff = mode.min_add_coeff; add_coeff<=(mode.max_add_coeff + MY_EPS); add_coeff+=mode.delt_add_coeff){
                    cout<<"Filling evap windtax of "<<remov_coeff<<", size is "<< single_run_params_list.size()<< endl;
                    
                    MySingleRunParams params_to_add;
                    params_to_add.sweep_num = sweep_num;
                    params_to_add.density = densities_and_inds_to_do[dens_ind_ind].second;
                    params_to_add.dens_ind = densities_and_inds_to_do[dens_ind_ind].first;
                    params_to_add.remov_coeff = remov_coeff;
                    params_to_add.add_coeff = add_coeff;
                    params_to_add.frac_not_recording_steps = mode.frac_not_recording_steps;
                    params_to_add.steps_per_particle_or_hole = mode.steps_per_particle_or_hole;
                    params_to_add.steps_per_txt_print = mode.steps_per_txt_print;
                    
                    sprintf(params_to_add.path_name, "%s/%s/Sweep%d/DensInd%d/", mode.base_path, dir_info, params_to_add.sweep_num, params_to_add.dens_ind);
                    single_run_params_list.push_back(params_to_add);                    
                    
                   // cout<<"Adding path "<<params_to_add.path_name<<endl;
                }
                
                
            }
        }
        
    }
    
    cout<<"Final List size is "<<single_run_params_list.size()<<endl;
    
    current_frame_number = 1000000000;
}



vector<double> MyAutomator:: generateMFTDensityList(){
    
    
    
    double epsilon = .0001;
    double small_delta_dens = .001;
    double big_delta_dens = .02;
    
    
    vector<double> list_to_return;
    
    for (int i = 0; i<densities_and_inds_to_do.size(); i++) {
        list_to_return.push_back(densities_and_inds_to_do[i].second);
    }    
    
    double dens;
    for(dens = small_delta_dens; dens < big_delta_dens - epsilon; dens+=small_delta_dens){
        list_to_return.push_back(dens);
        list_to_return.push_back(1-dens);   
    }
    
    for(dens = big_delta_dens; dens <= .5; dens+=big_delta_dens){
        list_to_return.push_back(dens);
        list_to_return.push_back(1-dens);
    }
    
    sort( list_to_return.begin(), list_to_return.end() );
    list_to_return.erase( unique( list_to_return.begin(), list_to_return.end() ), list_to_return.end() );
    
    if(0){
        cout<<"generateMFTDensityList"<< endl;
        for (int i = 0; i<list_to_return.size(); i++) {
            cout<<list_to_return[i]<<endl;
        }
    }
    //  cout<<"Here "<<endl;
    return list_to_return;
    
    
}


vector<double> MyAutomator:: generateSkewList(double meandens, double reg1frac){
    double reg2frac = 1 - reg1frac;
    
    double min_skew = max(-meandens*reg1frac, -(1-meandens)*reg2frac); //When either region 1 becomes empty or region 2 becomes full
    double max_skew = min((1-meandens)*reg1frac, meandens*reg2frac);//When either region 1 becomes full or region 2 becomes empty
    
    double big_delt_skew = .02;
    double small_delt_skew = .001;
    
    double bound_tol = .5;
    
    vector<double> list_to_return;
    
    //Generates the coarse skews
    for(double abs_unitless_skew = 0; abs_unitless_skew<=(1.); abs_unitless_skew += big_delt_skew){
        for (int unitless_skew_sign = -1; unitless_skew_sign <=1; unitless_skew_sign+=2) {
            double unitless_skew = abs_unitless_skew * unitless_skew_sign;
            if(unitless_skew < (max_skew - big_delt_skew) and unitless_skew>(min_skew + big_delt_skew)){             
                list_to_return.push_back(unitless_skew);
            }
        }        
    }
    
    double max_coarse = *max_element(list_to_return.begin(), list_to_return.end());
    double min_coarse = *min_element(list_to_return.begin(), list_to_return.end());
    
    //Generates the fine skews, which need to be within range, but outside the coarse range
    for(double abs_unitless_skew = 0; abs_unitless_skew<=(1.); abs_unitless_skew += small_delt_skew){
        for (int unitless_skew_sign = -1; unitless_skew_sign <=1; unitless_skew_sign+=2) {
            double unitless_skew = abs_unitless_skew * unitless_skew_sign;
            bool out_of_coarse_bounds = unitless_skew<(min_coarse-.5 * small_delt_skew) or unitless_skew>max_coarse+.5 * small_delt_skew;
            
            if(unitless_skew < (max_skew - bound_tol * small_delt_skew) and unitless_skew>(min_skew + bound_tol * small_delt_skew) and out_of_coarse_bounds){             
                list_to_return.push_back(unitless_skew);
            }
        }        
    }
    
    //Sorts and prints the list
    sort(list_to_return.begin(), list_to_return.end());
    
    
    if(0){
        cout<<"generateSkewList"<<"Mean Dens is "<<meandens <<", reg1frac is "<<reg1frac<< endl;
        cout<<"Min, Max skew are ("<<min_skew<<", "<<max_skew<<")"<<endl;
        for (int i = 0; i<list_to_return.size(); i++) {
            cout<<list_to_return[i]<<endl;
        }
    }
    return list_to_return;
    
    
}
