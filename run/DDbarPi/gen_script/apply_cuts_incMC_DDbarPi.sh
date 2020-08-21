#/bin/bash

BOSS=$1
TYPE=$2
PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/$TYPE

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
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/incMC_${TYPE}_${ECM}.root $ANA_PATH/$ECM/incMC_${TYPE}_${ECM}_signal.root signal" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/incMC_${TYPE}_${ECM}.root $ANA_PATH/$ECM/incMC_${TYPE}_${ECM}_side1.root side1" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/incMC_${TYPE}_${ECM}.root $ANA_PATH/$ECM/incMC_${TYPE}_${ECM}_side2.root side2" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/incMC_${TYPE}_${ECM}.root $ANA_PATH/$ECM/incMC_${TYPE}_${ECM}_side3.root side3" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/incMC_${TYPE}_${ECM}.root $ANA_PATH/$ECM/incMC_${TYPE}_${ECM}_side4.root side4" >> $FILENAME
done
