function[skew_info, perm_list, reg1_filled_volt_list, reg1_empty_volt_list, reg2_filled_volt_list, reg2_empty_volt_list] = LoadCrystalMFTData(partial_path, mean_dens)


fprintf('Starting LoadCrystalMFTData ...');

%Loads the samples and returns them so we can predict phase separations. 
skew_txt_path = sprintf('%s/SkewValues.txt', partial_path);

skew_info.values = unique(importdata(skew_txt_path));

skew_info.reg1frac = importdata(sprintf('%s/Reg1Frac.txt', partial_path));
skew_info.reg2frac = 1-skew_info.reg1frac;

    
for skew_ind = 1:length(skew_info.values)
    
    curr_skew = skew_info.values(skew_ind);
    perm_txt_path = sprintf('%s/PermSkew%.4f.txt', partial_path,  curr_skew);
 %       fprintf('\n   %s         \n' ,perm_txt_path);
    
    perm_list{skew_ind} = importdata(perm_txt_path);
    mean_perm(skew_ind)= mean(perm_list{skew_ind});
    
    reg1_filled_volt_list{skew_ind} = importdata(sprintf('%s/reg1FilledVoltageSkew%.4f.txt', partial_path,  curr_skew));
    reg1_empty_volt_list{skew_ind} = importdata(sprintf('%s/reg1EmptyVoltageSkew%.4f.txt', partial_path,  curr_skew));
    reg2_filled_volt_list{skew_ind} = importdata(sprintf('%s/reg2FilledVoltageSkew%.4f.txt', partial_path,  curr_skew));
    reg2_empty_volt_list{skew_ind} = importdata(sprintf('%s/reg2EmptyVoltageSkew%.4f.txt', partial_path,  curr_skew));
    
end



skew_info.pad(1) = max(-mean_dens*skew_info.reg1frac, -(1-mean_dens)*skew_info.reg2frac); %When either region 1 becomes empty or region 2 becomes full.
skew_info.pad(2) = min((1-mean_dens)*skew_info.reg1frac, mean_dens*skew_info.reg2frac); %When either region 1 becomes full or region 2 becomes empty

skew_info.reg1dens =  mean_dens + (skew_info.values/skew_info.reg1frac);
skew_info.reg2dens =  mean_dens - (skew_info.values/skew_info.reg2frac);

skew_info.unskewed_index = find(skew_info.values ==0);

fprintf('Finished \n');

