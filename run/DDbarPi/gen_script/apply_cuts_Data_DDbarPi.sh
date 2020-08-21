#/bin/bash

BOSS=$1
SOURCE_PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data
ANA_PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data
mkdir -p $ANA_PATH

if [ "$BOSS" = "703" ]; then
    ECMS=("4600")
fi

if [ "$BOSS" = "705" ]; then
    # ECMS=("4612" "4620" "4640" "4660" "4680" "4700")
    ECMS=("4680")
fi

FILENAME="Apply_Cuts_Data_"$BOSS
echo "#!/usr/bin/env bash" > $FILENAME
echo "cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/python" >> $FILENAME 
for ECM in ${ECMS[@]}; do
    mkdir -p $ANA_PATH/$ECM
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}.root $ANA_PATH/$ECM/data_${ECM}_signal.root signal" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}.root $ANA_PATH/$ECM/data_${ECM}_side1.root side1" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}.root $ANA_PATH/$ECM/data_${ECM}_side2.root side2" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}.root $ANA_PATH/$ECM/data_${ECM}_side3.root side3" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}.root $ANA_PATH/$ECM/data_${ECM}_side4.root side4" >> $FILENAME
done
