#!/usr/bin/env bash

# Main driver to submit jobs 
# Author SHI Xin <shixin@ihep.ac.cn>
# Modified by JING Maoqiang <jingmq@ihep.ac.cn>
# Created [2019-12-11 Dec 14:56]

usage() {
    printf "NAME\n\tsubmit.sh - Main driver to submit jobs\n"
    printf "\nSYNOPSIS\n"
    printf "\n\t%-5s\n" "./submit.sh [OPTION]" 
    printf "\nOPTIONS\n" 
    printf "\n\t%-9s  %-40s"  "0.1"       "[run on data sample for DDbarPi @703]"
    printf "\n\t%-9s  %-40s"  "0.2"       "[run on data sample for DDbarPi @705]"
    printf "\n\t%-9s  %-40s"  "0.3"       "[run on inclusive MC sample for DDbarPi @703]"
    printf "\n\t%-9s  %-40s"  "0.4"       "[run on  MC samples for DDbarPi @705]"
    printf "\n\n" 
}

usage_0_1() {
    printf "\n\t%-9s  %-40s"  ""          ""
    printf "\n\t%-9s  %-40s"  "0.1.1"     "Split data sample with each group 20G"
    printf "\n\t%-9s  %-40s"  "0.1.2"     "Generate Condor jobs on data ---- 1"
    printf "\n\t%-9s  %-40s"  "0.1.3"     "Test for data"
    printf "\n\t%-9s  %-40s"  "0.1.4"     "Submit Condor jobs on data ---- 2"
    printf "\n\t%-9s  %-40s"  "0.1.5"     "Synthesize data root files"
    printf "\n\t%-9s  %-40s"  "0.1.6"     "Get necessary info"
    printf "\n\t%-9s  %-40s"  "0.1.7"     "Apply cuts"
    printf "\n\t%-9s  %-40s"  ""           ""
    printf "\n"
}

usage_0_2() {
    printf "\n\t%-9s  %-40s"  ""          ""
    printf "\n\t%-9s  %-40s"  "0.2.1"     "Split data sample with each group 20G"
    printf "\n\t%-9s  %-40s"  "0.2.2"     "Generate Condor jobs on data ---- 1"
    printf "\n\t%-9s  %-40s"  "0.2.3"     "Test for data"
    printf "\n\t%-9s  %-40s"  "0.2.4"     "Submit Condor jobs on data ---- 2"
    printf "\n\t%-9s  %-40s"  "0.2.5"     "Synthesize data root files"
    printf "\n\t%-9s  %-40s"  "0.2.6"     "Get necessary info"
    printf "\n\t%-9s  %-40s"  "0.2.7"     "Apply cuts"
    printf "\n\t%-9s  %-40s"  ""           ""
    printf "\n"
}

usage_0_3() {
    printf "\n\t%-9s  %-40s"  ""          ""
    printf "\n\t%-9s  %-40s"  "0.3.1"     "Split inclusive MC samples with each group 20G"
    printf "\n\t%-9s  %-40s"  "0.3.2"     "Generate Condor jobs on inclusive MC samples ---- 1"
    printf "\n\t%-9s  %-40s"  "0.3.3"     "Test for inclusive MC samples"
    printf "\n\t%-9s  %-40s"  "0.3.4"     "Submit Condor jobs on inclusive MC samples ---- 2"
    printf "\n\t%-9s  %-40s"  "0.3.5"     "Synthesize inclusive MC samples root files"
    printf "\n\t%-9s  %-40s"  "0.3.6"     "Get necessary info"
    printf "\n\t%-9s  %-40s"  "0.3.7"     "Apply cuts"
    printf "\n\t%-9s  %-40s"  ""           ""
    printf "\n"
}

usage_0_4() {
    printf "\n\t%-9s  %-40s"  ""          ""
    printf "\n\t%-9s  %-40s"  "0.4.1"     "Generate MC samples ---- Simulation && Reconstruction"
    printf "\n\t%-9s  %-40s"  "0.4.2"     "Generate MC samples ---- Event Selection"
    printf "\n\t%-9s  %-40s"  "0.4.3"     "Synthesize inclusive MC samples root files"
    printf "\n\t%-9s  %-40s"  "0.4.4"     "Get necessary info"
    printf "\n\t%-9s  %-40s"  "0.4.5"     "Apply cuts"
    printf "\n\t%-9s  %-40s"  ""           ""
    printf "\n"
}

if [[ $# -eq 0 ]]; then
    usage
    echo "Please enter your option: "
    read option
else
    option=$1
fi

sub_0_1() {

case $option in
    
    # --------------------------------------------------------------------------
    #  run on data sample for DDbarPi @703 
    # --------------------------------------------------------------------------

    0.1.1) echo "Split data sample with each group 20G ..."
           cd ./run/DDbarPi/gen_script
           ./make_file_list_Data_DDbarPi.sh 703
           chmod u+x Gen_FileList_Data_703
           bash Gen_FileList_Data_703
           rm -r Gen_FileList_Data_703
           cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT
	       ;;

    0.1.2) echo "Generate Condor jobs on data ---- 1..." 
	       cd ./run/DDbarPi/gen_script
	       ./make_jobOption_file_Data_DDbarPi.sh 703
	       cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/jobs_text/data/4600
	       cp -r jobOptions_DDbarPi_data_4600-1.txt jobOptions_DDbarPi_data_4600-0.txt
           sed -i "s/ApplicationMgr\.EvtMax = -1/ApplicationMgr\.EvtMax = 20000/g" jobOptions_DDbarPi_data_4600-0.txt
           sed -i "s/DDbarPi_data_4600-1\.root/DDbarPi_data_4600-0\.root/g" jobOptions_DDbarPi_data_4600-0.txt
	       ;;

    0.1.3) echo "Test for data" 
           echo "have you changed test number?(yes / no)
           ./run/DDbarPi/jobs_text/data/4600/jobOptions_DDbarPi_data_4600-0.txt"
           read opt
           if [ $opt == "yes" ]
               then
               echo "now in yes"  
               cd ./run/DDbarPi/jobs_text/data/4600
               boss.exe jobOptions_DDbarPi_data_4600-0.txt
           else
               echo "Default value is 'no', please change test number."
           fi
           ;;

    0.1.4) echo "Submit Condor jobs on data ---- 2..." 
	       cd ./run/DDbarPi/gen_script
           ./sub_jobOption_file_Data_DDbarPi.sh 703
	       ;;

    0.1.5) echo "Synthesize data root files..."
	       cd ./run/DDbarPi/gen_script
           ./syn_Data_DDbarPi.sh 703
	       ;;

    0.1.6) echo "Get necessary info..."
	       cd ./run/DDbarPi/gen_script
           ./get_info_Data_DDbarPi.sh 703
           mkdir -p ../logfile
           mv Get_Info_Data_703 ../logfile
           cd ../logfile
           chmod u+x Get_Info_Data_703
           bash Get_Info_Data_703
           rm -r Get_Info_Data_703
           cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT
	       ;;

    0.1.7) echo "Apply cuts ..."
           cd ./run/DDbarPi/gen_script
           ./apply_cuts_Data_DDbarPi.sh 703
           mv Apply_Cuts_Data_703 ../logfile
           cd ../logfile
           chmod u+x Apply_Cuts_Data_703
           bash Apply_Cuts_Data_703
           rm -r Apply_Cuts_Data_703
           cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT
	       ;;

esac

}

sub_0_2() {

case $option in
    
    # --------------------------------------------------------------------------
    #  run on data sample for DDbarPi @705 
    # --------------------------------------------------------------------------

    0.2.1) echo "Split data sample with each group 20G ..."
           cd ./run/DDbarPi/gen_script
           ./make_file_list_Data_DDbarPi.sh 705
           chmod u+x Gen_FileList_Data_705
           bash Gen_FileList_Data_705
           rm -r Gen_FileList_Data_705
           cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT
	       ;;

    0.2.2) echo "Generate Condor jobs on data ---- 1..." 
	       cd ./run/DDbarPi/gen_script
	       ./make_jobOption_file_Data_DDbarPi.sh 705
	       cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/jobs_text/data/4680
	       cp -r jobOptions_DDbarPi_data_4680-1.txt jobOptions_DDbarPi_data_4680-0.txt
           sed -i "s/ApplicationMgr\.EvtMax = -1/ApplicationMgr\.EvtMax = 20000/g" jobOptions_DDbarPi_data_4680-0.txt
           sed -i "s/DDbarPi_data_4680-1\.root/DDbarPi_data_4680-0\.root/g" jobOptions_DDbarPi_data_4680-0.txt
	       ;;

    0.2.3) echo "Test for data" 
           echo "have you changed test number?(yes / no)
           ./run/DDbarPi/jobs_text/data/4680/jobOptions_DDbarPi_data_4680-0.txt"
           read opt
           if [ $opt == "yes" ]
               then
               echo "now in yes"  
               cd ./run/DDbarPi/jobs_text/data/4680
               boss.exe jobOptions_DDbarPi_data_4680-0.txt
           else
               echo "Default value is 'no', please change test number."
           fi
           ;;

    0.2.4) echo "Submit Condor jobs on data ---- 2..." 
	       cd ./run/DDbarPi/gen_script
           ./sub_jobOption_file_Data_DDbarPi.sh 705
	       ;;

    0.2.5) echo "Synthesize data root files..."
	       cd ./run/DDbarPi/gen_script
           ./syn_Data_DDbarPi.sh 705
	       ;;

    0.2.6) echo "Get necessary info..."
	       cd ./run/DDbarPi/gen_script
           ./get_info_Data_DDbarPi.sh 705
           mkdir -p ../logfile
           mv Get_Info_Data_705 ../logfile
           cd ../logfile
           chmod u+x Get_Info_Data_705
           bash Get_Info_Data_705
           rm -r Get_Info_Data_705
           cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT
	       ;;

    0.2.7) echo "Apply cuts ..."
           cd ./run/DDbarPi/gen_script
           ./apply_cuts_Data_DDbarPi.sh 705
           mkdir -p ../logfile
           mv Apply_Cuts_Data_705 ../logfile
           cd ../logfile
           chmod u+x Apply_Cuts_Data_705
           bash Apply_Cuts_Data_705
           rm -r Apply_Cuts_Data_705
           cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT
	       ;;

esac

}

sub_0_3() {

case $option in
    
    # --------------------------------------------------------------------------
    #  run on inclusive MC samples for DDbarPi @703
    # --------------------------------------------------------------------------

    0.3.1) echo "Split inclusive MC samples with each group 20G ..."
           cd ./run/DDbarPi/gen_script
           ./make_file_list_incMC_DDbarPi.sh 705 hadron
           chmod u+x Gen_FileList_incMC_705_hadron
           bash Gen_FileList_incMC_705_hadron
           rm -r Gen_FileList_incMC_705_hadron
           cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT
	       ;;

    0.3.2) echo "Generate Condor jobs on inclusive MC samples ---- 1..." 
	       cd ./run/DDbarPi/gen_script
	       ./make_jobOption_file_incMC_DDbarPi.sh 705 hadron
	       cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/jobs_text/mc/hadron/4680
	       cp -r jobOptions_DDbarPi_incMC_hadron_4680-1.txt jobOptions_DDbarPi_incMC_hadron_4680-0.txt
           sed -i "s/ApplicationMgr\.EvtMax = -1/ApplicationMgr\.EvtMax = 500/g" jobOptions_DDbarPi_incMC_hadron_4680-0.txt
           sed -i "s/DDbarPi_incMC_hadron_4680-1\.root/DDbarPi_incMC_hadron_4680-0\.root/g" jobOptions_DDbarPi_incMC_hadron_4680-0.txt
	       ;;

    0.3.3) echo "Test for inclusive MC samples" 
           echo "have you changed test number?(yes / no)
           ./run/DDbarPi/jobs_text/mc/hadron/4680/jobOptions_DDbarPi_incMC_hadron_4680-0.txt"
           read opt
           if [ $opt == "yes" ]
               then
               echo "now in yes"  
               cd ./run/DDbarPi/jobs_text/mc/hadron/4680
               boss.exe jobOptions_DDbarPi_incMC_hadron_4680-0.txt
           else
               echo "Default value is 'no', please change test number."
           fi
           ;;

    0.3.4) echo "Submit Condor jobs on inclusive MC ---- 2..." 
	       cd ./run/DDbarPi/gen_script
           ./sub_jobOption_file_incMC_DDbarPi.sh 705 hadron
	       ;;

    0.3.5) echo "Synthesize inclusive MC samples root files..."
	       cd ./run/DDbarPi/gen_script
           ./syn_incMC_DDbarPi.sh 705 hadron
	       ;;

    0.3.6) echo "Get necessary info..."
	       cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/gen_script
           ./get_info_incMC_DDbarPi.sh 705 hadron
           mv Get_Info_incMC_hadron_705 ../logfile
           cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/logfile
           chmod u+x Get_Info_incMC_hadron_705
           mkdir -p jobs.out jobs.err
           hep_sub -g physics Get_Info_incMC_hadron_705 -o jobs.out -e jobs.err
	       ;;

    0.3.7) echo "Apply cuts ..."
           cd ./run/DDbarPi/gen_script
           ./apply_cuts_incMC_DDbarPi.sh 705 hadron
           chmod u+x Apply_Cuts_hadron_705
           bash Apply_Cuts_hadron_705
           rm -r Apply_Cuts_hadron_705
           cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT
	       ;;

esac

}

sub_0_4() {

case $option in
    
    # --------------------------------------------------------------------------
    #  run on MC samples for DDbarPi @705 
    #      DDPI     --> e+e- --> D-D0-barpi+
    # --------------------------------------------------------------------------

    0.4.1) echo "Generate MC samples ---- Simulation && Reconstruction ..."
           echo "which MC sample do you want to simulate?"
           read opt
           if [ $opt == "DDPI" ]; then
               cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/gen_script/gen_mc/DDPI
               ./sub_jobOption_D_D_PI.sh 705
           else
               echo "Please add the MC simulation joboption files!"
           fi
	       ;;

    0.4.2) echo "Generate MC samples ---- Event Selection ..."
           echo "which MC sample do you want to select?"
           read opt
           if [ $opt == "DDPI" ]; then
               cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/gen_script/gen_mc/DDPI
               ./sub_Ana_D_D_PI.sh 705
           else
               echo "Please add the MC simulation joboption files!"
           fi
	       ;;

    0.4.3) echo "Synthesize inclusive MC samples root files ..."
           cd ./run/DDbarPi/gen_script
           echo "which MC sample do you want to synthesize?"
           read opt
           if [ $opt == "DDPI" ]; then
               ./syn_MC_DDbarPi.sh 705 DDPI
           else
               echo "Please add the MC simulation joboption files!"
           fi
	       ;;

    0.4.4) echo "Get necessary info..."
           echo "which MC sample's info do you want to get?"
           read opt
           if [ $opt == "DDPI" ]; then
	           cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/gen_script
               ./get_info_MC_DDbarPi.sh 705 DDPI
               mv Get_Info_MC_DDPI_705 ../logfile
               cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/logfile
               chmod u+x Get_Info_MC_DDPI_705
               mkdir -p jobs.out jobs.err
               hep_sub -g physics Get_Info_MC_DDPI_705 -o jobs.out -e jobs.err
           else
               echo "Please add the MC simulation joboption files!"
           fi
           ;;

    0.4.5) echo "Apply cuts ..."
           cd ./run/DDbarPi/gen_script
           echo "which MC sample do you want to apply cuts?"
           read opt
           if [ $opt == "DDPI" ]; then
               ./apply_cuts_MC_DDbarPi.sh 705 DDPI
               chmod u+x Apply_Cuts_DDPI_705
               bash Apply_Cuts_DDPI_705
               rm -r Apply_Cuts_DDPI_705
           else
               echo "Please add the MC simulation joboption files!"
           fi
	       ;;

esac

}


case $option in
   
    # --------------------------------------------------------------------------
    #  Data @703 
    # --------------------------------------------------------------------------

    0.1) echo "Running on data sample @703..."
         usage_0_1 
         echo "Please enter your option: " 
         read option  
         sub_0_1 option 
	     ;;

    0.1.*) echo "Running on data sample @703..."
           sub_0_1 option  
           ;;  
        
    # --------------------------------------------------------------------------
    #  Data @705 
    # --------------------------------------------------------------------------

    0.2) echo "Running on data sample @705..."
         usage_0_2 
         echo "Please enter your option: " 
         read option  
         sub_0_2 option 
	     ;;

    0.2.*) echo "Running on data sample @705..."
           sub_0_2 option  
           ;;  

    # --------------------------------------------------------------------------
    #  Inclusive MC samples @703
    # --------------------------------------------------------------------------

    0.3) echo "Running on inclusive MC samples @703 ..."
         usage_0_3 
         echo "Please enter your option: " 
         read option  
         sub_0_3 option 
	     ;;

    0.3.*) echo "Running on inclusive MC samples @703 ..."
           sub_0_3 option  
           ;;  

    # --------------------------------------------------------------------------
    #  MC samples @705
    # --------------------------------------------------------------------------

    0.4) echo "Running on MC samples for DDbarPi @705 ..."
         usage_0_4 
         echo "Please enter your option: " 
         echo "    DDPI    --> e+e- --> D-D0-barpi+"
         read option  
         sub_0_4 option 
	     ;;

    0.4.*) echo "Running on MC samples for DDbarPi @705 ..."
           sub_0_4 option  
           ;;  

esac
