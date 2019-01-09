close all;
clear all;

float_string = '%.2f'; %Helper for string processing
int_str = '%d'; %Helper for string processing
addAllPaths;



dens_run_names = {'One Fourth Filling', 'Half Filling', 'Three Fourths Filling'};
sys_params.dens_values = [.25];
sys_params.n_sweeps = 3;
sys_params.input_dir = 'Jan11TestingAutomator/ChanFileOutput/';
%sys_params.evap = -6:12:6;
sys_params.evap = -6:6:6;
sys_params.cond = fliplr(sort(sys_params.evap));
%sys_params.cond = 0;


%Sometimes, we only want to perform certain functions. We can choose which things to do by setting these flags.
things_to_do.do_snapshots = 1;
things_to_do.do_histo_chanwall = 1;
things_to_do.do_histo_clump = 1;
things_to_do.do_crystal = 1;
things_to_do.do_uniform = 1;
things_to_do.do_fourier = 1;
things_to_do.do_two_point_corr = 1;
things_to_do.do_conductivity = 1;
things_to_do.do_cartoon_MFT = 1;

%%%%%%%%%%%%%%%%%%%%%%%%%%

output_dir = sprintf('%s/MatlabPlotOutput', sys_params.input_dir);
system(sprintf('rm -rf %s', output_dir));
system(sprintf('mkdir %s', output_dir));
white_shading_im = WhiteShadingArray(sys_params);
for dens_ind = 1:length(sys_params.dens_values)
    
    fprintf('\n ******Doing stuff for DensInd%d *******\n ', dens_ind);
    cur_dens = sys_params.dens_values(dens_ind);
    
    %Pack a few extra temporary values onto sys_params
    sys_params.mean_dens = cur_dens;
    sys_params.dens_ind = dens_ind;
    
    %    input_file_path = sprintf('%s/LocalDiff1.00Bias0.00/Sweep%s/DensInd%d/', input_dir, int_str, dens_ind-1);
    input_file_path = sprintf('%s/NonLocal/Sweep%s/DensInd%d/', sys_params.input_dir, int_str, dens_ind-1);
    cross_dens_output_path = sprintf('%s/CrossDens/', output_dir);
    output_file_path = sprintf('%s/DensInd%d/', output_dir, dens_ind);
    phase_file_path = sprintf('%s/PhaseIms/', output_file_path);
    
    
    if(things_to_do.do_snapshots)
    %Makes the png images, and then puts them into a phase diagram image.        
        fprintf('\n *** Starting snapshots \n');
        for sweep_num = 0:(sys_params.n_sweeps - 1)
            fprintf('Doing snapshots for sweep %d \n', sweep_num);
            png_snapshot_path = sprintf('%s/tmpfiles/Sweep%d/PngSnapshots/', output_file_path, sweep_num);
            ConvertAllToPng(sys_params, sprintf(input_file_path, sweep_num),  png_snapshot_path);
            MakeGridImage(sprintf('%s/EvapInd%sCondInd%s.png', png_snapshot_path, int_str, int_str), sys_params, WhiteShadingArray(sys_params), ...
                sprintf('%s/SweepWise/PhaseDiagramSweep%d.png', phase_file_path, sweep_num));
        end
        fprintf('*** Finished snapshots*** \n');
        
    end
    
    
    if(things_to_do.do_histo_chanwall)
        fprintf('\n***Starting HistoChanWall*** \n');
        chanwall_histo_shading_image = GenerateWallAndChanHistogramImages(sys_params, input_file_path, sprintf('%s/Hist/', output_file_path));
        %Generates white and histo images
        MakeCheckerBoardGridImage(sys_params,  chanwall_histo_shading_image, sprintf('%s/ShadeOnly/HistoChanWallChecker.png', phase_file_path));
        MakeShadeOnlyGridImage(sys_params,  chanwall_histo_shading_image, sprintf('%s/ShadeOnly/HistoShade.png', phase_file_path));
        
        %  histo_shading_image = white_shading_im;
        
        %Generates histo phase diagrams with shading
        MakeGridImage(sprintf('%s/Hist/Chan/ChanHistEvapInd%sCondInd%s.png', output_file_path, int_str, int_str), sys_params, chanwall_histo_shading_image, ...
            sprintf('%s/ShadedHistChanPhaseDiagram.png', phase_file_path));
        MakeGridImage(sprintf('%s/Hist/Wall/WallHistEvapInd%sCondInd%s.png', output_file_path, int_str, int_str), sys_params, chanwall_histo_shading_image, ...
            sprintf('%s/ShadedHistWallPhaseDiagram.png', phase_file_path));
        
        MakeGridImage(sprintf('%s/Hist/Chan/ChanHistEvapInd%sCondInd%s.png', output_file_path, int_str, int_str), sys_params, white_shading_im, ...
            sprintf('%s/UnshadedHistChanPhaseDiagram.png', phase_file_path));
        MakeGridImage(sprintf('%s/Hist/Wall/WallHistEvapInd%sCondInd%s.png', output_file_path, int_str, int_str), sys_params, white_shading_im, ...
            sprintf('%s/UnshadedHistWallPhaseDiagram.png', phase_file_path));
        
        
        MakeGridImage(sprintf('%s/Hist/Chan/ChanHistEvapInd%sCondInd%s.png', output_file_path, int_str, int_str), sys_params, white_shading_im, ...
            sprintf('%s/UnshadedHistChanPhaseDiagramDensInd%d.png', cross_dens_output_path, dens_ind));
        MakeGridImage(sprintf('%s/Hist/Wall/WallHistEvapInd%sCondInd%s.png', output_file_path, int_str, int_str), sys_params, white_shading_im, ...
            sprintf('%s/UnshadedHistWallPhaseDiagramDensInd%d.png', cross_dens_output_path, dens_ind));
        
        
        
        chan_binder_input_file_path = sprintf('%s/Hist/Chan/ChanHistEvapInd%sCondInd%s.mat', output_file_path, int_str, int_str);
        PlotBinderCumulants(sys_params, chan_binder_input_file_path, sprintf('%s/ChanBinder', phase_file_path));
        
        wall_binder_input_file_path = sprintf('%s/Hist/Wall/WallHistEvapInd%sCondInd%s.mat', output_file_path, int_str, int_str);
        PlotBinderCumulants(sys_params, wall_binder_input_file_path, sprintf('%s/WallBinder', phase_file_path));
        fprintf('***Finished HistoChanWall*** \n');
    end
    
    
    if(things_to_do.do_histo_clump)
        fprintf('\n***Starting HistoClump*** \n');
        clump_histo_shading_image = GenerateClumpHistogramImages(sys_params, input_file_path, sprintf('%s/Hist/', output_file_path));
        
        MakeGridImage(sprintf('%s/Hist/Clump/ClumpHistEvapInd%sCondInd%s.png', output_file_path, int_str, int_str), sys_params, white_shading_im, ...
            sprintf('%s/UnshadedClumpPhaseDiagram.png', phase_file_path));
        
        MakeGridImage(sprintf('%s/Hist/Clump/ClumpHistEvapInd%sCondInd%s.png', output_file_path, int_str, int_str), sys_params, clump_histo_shading_image, ...
            sprintf('%s/ShadedHistClumpPhaseDiagram.png', phase_file_path));
        
        
        MakeGridImage(sprintf('%s/Hist/Clump/ClumpHistEvapInd%sCondInd%s.png', output_file_path, int_str, int_str), sys_params, white_shading_im, ...
            sprintf('%s/UnshadedClumpPhaseDiagramDensInd%d.png', cross_dens_output_path, dens_ind));
        
        MakeGridImage(sprintf('%s/Hist/Clump/ClumpHistEvapInd%sCondInd%s.png', output_file_path, int_str, int_str), sys_params, clump_histo_shading_image, ...
            sprintf('%s/ShadedHistClumpPhaseDiagramDensInd%d.png', cross_dens_output_path, dens_ind));
        
        
        MakeCheckerBoardGridImage(sys_params,  clump_histo_shading_image, sprintf('%s/ShadeOnly/HistoClumpChecker.png', phase_file_path));
        
        clump_binder_input_file_path = sprintf('%s/Hist/Clump/ClumpHistEvapInd%sCondInd%s.mat', output_file_path, int_str, int_str);
        PlotBinderCumulants(sys_params, wall_binder_input_file_path, sprintf('%s/ClumpBinder', phase_file_path));
        fprintf('***Finished HistoClump*** \n');
    end
    
    
    if(things_to_do.do_histo_clump && things_to_do.do_histo_chanwall)
        fprintf('\n***Starting MakeWallChanClumpCheckerboardGridImage*** \n');
        MakeWallChanClumpCheckerboardGridImage(sys_params, chanwall_histo_shading_image, clump_histo_shading_image, sprintf('%s/ShadeOnly/HistoWallChanClumpChecker.png', phase_file_path));
        fprintf('***Finished MakeWallChanClumpCheckerboardGridImage*** \n');
        
        %        MakeCheckerBoardGridImage(sys_params,  histo_shading_image, sprintf('%s/ShadeOnly/HistoClumpChecker.png', phase_file_path));
        
    end
    
    
    
    if(things_to_do.do_crystal)
        fprintf('\n***Starting Crystal MFT*** \n');
        
        [quart_crys_MFT_shade_im quart_crys_free_en] = CrystalMFT(cur_dens, sys_params, 4, sprintf('%s/MFT/CrystalQuad/', output_file_path));
        MakeGridImage(sprintf('%s/MFT/CrystalQuad/%s', output_file_path, 'ChanEvapInd%dCondInd%d.png'), sys_params, quart_crys_MFT_shade_im, sprintf('%s/MFTQuarterChan.png', phase_file_path));
        MakeGridImage(sprintf('%s/MFT/CrystalQuad/%s', output_file_path, 'WallEvapInd%dCondInd%d.png'), sys_params, quart_crys_MFT_shade_im, sprintf('%s/MFTQuarterWall.png', phase_file_path));
        MakeShadeOnlyGridImage(sys_params,  quart_crys_MFT_shade_im, sprintf('%s/ShadeOnly/QuartCrystShade.png', phase_file_path));
        MakeCheckerBoardGridImage(sys_params,  quart_crys_MFT_shade_im, sprintf('%s/ShadeOnly/QuartCrystChecker.png', phase_file_path));
        
        
        [half_crys_MFT_shade_im half_crys_free_en] = CrystalMFT(cur_dens, sys_params, 2, sprintf('%s/MFT/CrystalHalf/', output_file_path));
        MakeGridImage(sprintf('%s/MFT/CrystalHalf/%s', output_file_path, 'ChanEvapInd%dCondInd%d.png'), sys_params, half_crys_MFT_shade_im, sprintf('%s/MFTHalfChan.png', phase_file_path));
        MakeGridImage(sprintf('%s/MFT/CrystalHalf/%s', output_file_path, 'WallEvapInd%dCondInd%d.png'), sys_params, half_crys_MFT_shade_im, sprintf('%s/MFTHalfWall.png', phase_file_path));
        MakeShadeOnlyGridImage(sys_params,  half_crys_MFT_shade_im, sprintf('%s/ShadeOnly/HalfCrystShade.png', phase_file_path));
        MakeCheckerBoardGridImage(sys_params,  half_crys_MFT_shade_im, sprintf('%s/ShadeOnly/HalfCrystChecker.png', phase_file_path));
        fprintf('***Finished Crystal MFT*** \n');
    end
    
    if(things_to_do.do_uniform)
        fprintf('\n***Starting Uniform MFT*** \n');
        
        [unif_MFT_shade_im  unif_MFT_free_en] = UniformMFT(cur_dens, sys_params, sprintf('%s/MFT/Uniform/', output_file_path));
        MakeGridImage(sprintf('%s/MFT/Uniform/%s', output_file_path, 'ChanEvapInd%dCondInd%d.png'), sys_params, unif_MFT_shade_im, sprintf('%s/MFTUniformChan.png', phase_file_path));
        MakeGridImage(sprintf('%s/MFT/Uniform/%s', output_file_path, 'WallEvapInd%dCondInd%d.png'), sys_params, unif_MFT_shade_im, sprintf('%s/MFTUniformWall.png', phase_file_path));
        MakeShadeOnlyGridImage(sys_params,  unif_MFT_shade_im, sprintf('%s/ShadeOnly/UniformShade.png', phase_file_path));
        MakeCheckerBoardGridImage(sys_params,  unif_MFT_shade_im, sprintf('%s/ShadeOnly/UniformChecker.png', phase_file_path));
        
        fprintf('***Finished Uniform MFT*** \n');
    end
    
    
    if(things_to_do.do_cartoon_MFT)
        fprintf('***Starting cartoon MFT*** \n');
        
        [cartoon_unif_MFT_shade_im ] = CartoonUniformMeanFieldStab(cur_dens, sys_params, sprintf('%s/MFT/CartoonUniform/', output_file_path));
        
        MakeGridImage(sprintf('%s/MFT/CartoonUniform/MFTGenPlots/%s', output_file_path, 'CartoonChanEvapInd%dCondInd%d.png'), sys_params, cartoon_unif_MFT_shade_im, sprintf('%s/MFTcartoonUniformChan.png', phase_file_path));
        MakeGridImage(sprintf('%s/MFT/CartoonUniform/MFTGenPlots/%s', output_file_path, 'CartoonWallEvapInd%dCondInd%d.png'), sys_params, cartoon_unif_MFT_shade_im, sprintf('%s/MFTcartoonUniformWall.png', phase_file_path));
        MakeShadeOnlyGridImage(sys_params,  cartoon_unif_MFT_shade_im, sprintf('%s/ShadeOnly/CartoonUniformShade.png', phase_file_path));
        MakeCheckerBoardGridImage(sys_params,  cartoon_unif_MFT_shade_im, sprintf('%s/ShadeOnly/CartoonUniformChecker.png', phase_file_path));
        fprintf('***Finished cartoon MFT*** \n');
        
    end
    
    if(things_to_do.do_crystal && things_to_do.do_uniform)
        fprintf('***Starting MaxFreeEnComparision*** \n');
        MakeMaxFreeEnImage(sys_params, unif_MFT_free_en, half_crys_free_en, quart_crys_free_en, sprintf('%s/MFT/MostFreeEn/', output_file_path));
        MakeGridImage(sprintf('%s/MFT/MostFreeEn/%s', output_file_path, 'EvapInd%dCondInd%d.png'), sys_params, white_shading_im, sprintf('%s/MaxFreeEnMFT.png', phase_file_path));
        fprintf('***Finishing MaxFreeEnComparision*** \n');
        
    end
    
    if(things_to_do.do_fourier)
        fprintf('\n***Starting FFT*** \n');
        
        GenerateFFTImages(sys_params, input_file_path, sprintf('%s/FFT/', output_file_path));
        MakeGridImage(sprintf('%s/FFT/FFTIms/FFTEvapInd%sCondInd%s.png', output_file_path, int_str, int_str), sys_params, white_shading_im, ...
            sprintf('%s/FFTIms/FFTPhaseDiagram.png', phase_file_path));
        MakeGridImage(sprintf('%s/FFT/FFTScatter/AngleDependFFTEvapInd%sCondInd%s.png', output_file_path, int_str, int_str), sys_params, white_shading_im, ...
            sprintf('%s/AngleFFTPhaseDiagram.png', phase_file_path));
        fprintf('***Finished FFT*** \n');
    end
    
    if(things_to_do.do_two_point_corr)
        fprintf('\n***Starting TPC*** \n');
        
        GenerateTwoPointCorrImages(sys_params, input_file_path, sprintf('%s/TPC/', output_file_path));
        MakeGridImage(sprintf('%s/TPC/TPCEvapInd%sCondInd%s.png', output_file_path, int_str, int_str), sys_params, white_shading_im, ...
            sprintf('%s/TPCPhaseDiagram.png', phase_file_path));
        fprintf('***Finished TPC*** \n');
    end
    
    if(things_to_do.do_conductivity)
        fprintf('\n***Starting Conductivity*** \n');
        PlotConductivity(sys_params, input_file_path, sprintf('%s/Conduct', phase_file_path));
        fprintf('***Finished Conductivity*** \n');
    end
    
    if(things_to_do.do_snapshots)
        fprintf('\n*** Starting to juxtapose snapshots \n');
        for sweep_num = 0:(sys_params.n_sweeps - 1)
            phase_diag_path =  sprintf('%s/SweepWise/PhaseDiagramSweep%d.png', phase_file_path, sweep_num);
            unshaded_wall_path =  sprintf('%s/UnshadedHistWallPhaseDiagram.png', phase_file_path);
            shaded_wall_path =  sprintf('%s/ShadedHistWallPhaseDiagram.png', phase_file_path);
            unshaded_wall_output_path = sprintf('%s/SweepWise/WallVsSnapShotSweep%d.png', phase_file_path, sweep_num);
            shaded_wall_output_path = sprintf('%s/SweepWise/ShadedWallVsSnapShotSweep%d.png', phase_file_path, sweep_num);
            if(things_to_do.do_histo_chanwall)
                CatIms(phase_diag_path, unshaded_wall_path, unshaded_wall_output_path, 1);
                CatIms(phase_diag_path, shaded_wall_path, shaded_wall_output_path, 1);
            end
            
            if(things_to_do.do_histo_clump)
                unshaded_clump_path =  sprintf('%s/UnshadedClumpPhaseDiagram.png', phase_file_path);
                unshaded_clump_output_path = sprintf('%s/SweepWise/ClumpVsSnapShotSweep%d.png', phase_file_path, sweep_num);
                CatIms(phase_diag_path, unshaded_clump_path, unshaded_clump_output_path, 1);
            end
        end
        fprintf('*** Finished juxtapose snapshots \n');
    end
    
end




