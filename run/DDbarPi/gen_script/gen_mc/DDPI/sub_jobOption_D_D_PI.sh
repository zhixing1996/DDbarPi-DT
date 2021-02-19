#/bin/bash
BOSS=$1
if [ "$BOSS" = "703" ]; then
    ECMS=("4600")
    CMS=("4.600")
    RUNNO_LOW=("35227")
    RUNNO_UP=("36213")
fi
if [ "$BOSS" = "705" ]; then
    # ECMS=("4620" "4640" "4660" "4680" "4700")
    # CMS=("4.626" "4.640" "4.660" "4.680" "4.700")
    # RUNNO_LOW=("63075" "63516" "63718" "63867" "64028")
    # RUNNO_UP=("63515" "63715" "63852" "64015" "64289")
    ECMS=("4680")
    CMS=("4.681208")
    RUNNO_LOW=("63867")
    RUNNO_UP=("64015")
fi

COUN=0
FILENAME="Sub_jobOption_"$BOSS
for ECM in ${ECMS[@]}; do
    JobText_SaveDir=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/jobs_text/mc/DDPI/$ECM
    Script_Dir=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/gen_script/gen_mc/DDPI
    mkdir -p $JobText_SaveDir
    rm -rf $JobText_SaveDir/jobOptions*txt
    rm -rf $JobText_SaveDir/subSimRec_*.sh
    rm -rf $JobText_SaveDir/fort.*
    rm -rf $JobText_SaveDir/phokhara*
    rm -rf $JobText_SaveDir/xs_user.dat 
    cd $JobText_SaveDir
    cp -r $Script_Dir/jobOptions_sim_sig_D_D_PI_PHSP_tempE_705.sh jobOptions_sim_sig_D_D_PI_PHSP_${ECM}_705.sh
    cp -r $Script_Dir/jobOptions_rec_sig_D_D_PI_PHSP_tempE_705.sh jobOptions_rec_sig_D_D_PI_PHSP_${ECM}_705.sh
    threshold=4.42
    sh jobOptions_sim_sig_D_D_PI_PHSP_${ECM}_705.sh 0 49 $ECM ${CMS[$COUNT]} 5000 $threshold ${RUNNO_LOW[$COUNT]} ${RUNNO_UP[$COUNT]}
    sh jobOptions_rec_sig_D_D_PI_PHSP_${ECM}_705.sh 0 49 $ECM
    rm -rf /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/rtraw/DDPI/$ECM/*.rtraw
    rm -rf /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/dst/DDPI/$ECM/*.dst
    cp -rf /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/gen_script/gen_mc/subSimRec.sh ./
    cp $Script_Dir/xs_user.dat ./
    sh subSimRec.sh jobOptions_sim_sig_D_D_PI_PHSP_$ECM jobOptions_rec_sig_D_D_PI_PHSP_$ECM subSimRec_D_D_PI_$ECM 0 49
    COUNT=$((${COUNT}+1))
done
