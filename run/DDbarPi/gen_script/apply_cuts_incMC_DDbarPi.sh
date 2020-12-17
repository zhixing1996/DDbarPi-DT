#/bin/bash

BOSS=$1
TYPE=$2
ANA_PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/$TYPE

if [ "$BOSS" = "703" ]; then
    ECMS=("4600")
fi

if [ "$BOSS" = "705" ]; then
    ECMS=("4680")
fi

FILENAME="Apply_Cuts_${TYPE}_"$BOSS
echo "#!/usr/bin/env bash" > $FILENAME
echo "cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/python" >> $FILENAME 
for ECM in ${ECMS[@]}; do
    echo "./apply_cuts.py $ANA_PATH/$ECM/incMC_${TYPE}_${ECM}.root $ANA_PATH/$ECM/incMC_${TYPE}_${ECM}_signal.root signal after_masss" >> $FILENAME
    echo "./apply_cuts.py $ANA_PATH/$ECM/incMC_${TYPE}_${ECM}.root $ANA_PATH/$ECM/incMC_${TYPE}_${ECM}_signal_before_mass.root signal before_masss" >> $FILENAME
done
