#/bin/bash

BOSS=$1
TYPE=$2
SOURCE_PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/$TYPE
ANA_PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/$TYPE
mkdir -p $ANA_PATH

if [ "$BOSS" = "703" ]; then
    ECMS=("4600")
fi

if [ "$BOSS" = "705" ]; then
    # ECMS=("4620" "4640" "4660" "4680" "4700")
    ECMS=("4680")
fi

FILENAME="Apply_Cuts_${TYPE}_"$BOSS
echo "#!/usr/bin/env bash" > $FILENAME
echo "cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/python" >> $FILENAME 
for ECM in ${ECMS[@]}; do
    mkdir -p $ANA_PATH/$ECM
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/mc_${TYPE}_${ECM}.root $ANA_PATH/$ECM/mc_${TYPE}_${ECM}_signal.root signal after_mass" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/mc_${TYPE}_${ECM}.root $ANA_PATH/$ECM/mc_${TYPE}_${ECM}_signal_before_mass.root signal before_mass" >> $FILENAME
done
