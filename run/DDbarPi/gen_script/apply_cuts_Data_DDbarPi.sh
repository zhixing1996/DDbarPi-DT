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
    ECMS=("4680" "4700")
fi

FILENAME="Apply_Cuts_Data_"$BOSS
echo "#!/usr/bin/env bash" > $FILENAME
echo "cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/python" >> $FILENAME 
for ECM in ${ECMS[@]}; do
    mkdir -p $ANA_PATH/$ECM
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}.root $ANA_PATH/$ECM/data_${ECM}_signal.root signal after_mass" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}_side1_low_raw.root $ANA_PATH/$ECM/data_${ECM}_side1_low.root side1_low after_mass" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}_side2_low_raw.root $ANA_PATH/$ECM/data_${ECM}_side2_low.root side2_low after_mass" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}_side3_low_raw.root $ANA_PATH/$ECM/data_${ECM}_side3_low.root side3_low after_mass" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}_side4_low_raw.root $ANA_PATH/$ECM/data_${ECM}_side4_low.root side4_low after_mass" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}_side1_up_raw.root $ANA_PATH/$ECM/data_${ECM}_side1_up.root side1_up after_mass" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}_side2_up_raw.root $ANA_PATH/$ECM/data_${ECM}_side2_up.root side2_up after_mass" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}_side3_up_raw.root $ANA_PATH/$ECM/data_${ECM}_side3_up.root side3_up after_mass" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}_side4_up_raw.root $ANA_PATH/$ECM/data_${ECM}_side4_up.root side4_up after_mass" >> $FILENAME
    echo "rm -rf $ANA_PATH/$ECM/data_${ECM}_side1.root" >> $FILENAME
    echo "rm -rf $ANA_PATH/$ECM/data_${ECM}_side2.root" >> $FILENAME
    echo "rm -rf $ANA_PATH/$ECM/data_${ECM}_side3.root" >> $FILENAME
    echo "rm -rf $ANA_PATH/$ECM/data_${ECM}_side4.root" >> $FILENAME
    echo "hadd $ANA_PATH/$ECM/data_${ECM}_side1.root $ANA_PATH/$ECM/data_${ECM}_side1_low.root $ANA_PATH/$ECM/data_${ECM}_side1_up.root" >> $FILENAME
    echo "hadd $ANA_PATH/$ECM/data_${ECM}_side2.root $ANA_PATH/$ECM/data_${ECM}_side2_low.root $ANA_PATH/$ECM/data_${ECM}_side2_up.root" >> $FILENAME
    echo "hadd $ANA_PATH/$ECM/data_${ECM}_side3.root $ANA_PATH/$ECM/data_${ECM}_side3_low.root $ANA_PATH/$ECM/data_${ECM}_side3_up.root" >> $FILENAME
    echo "hadd $ANA_PATH/$ECM/data_${ECM}_side4.root $ANA_PATH/$ECM/data_${ECM}_side4_low.root $ANA_PATH/$ECM/data_${ECM}_side4_up.root" >> $FILENAME
    echo "rm -rf $ANA_PATH/$ECM/data_${ECM}_side1_low.root $ANA_PATH/$ECM/data_${ECM}_side1_up.root" >> $FILENAME
    echo "rm -rf $ANA_PATH/$ECM/data_${ECM}_side2_low.root $ANA_PATH/$ECM/data_${ECM}_side2_up.root" >> $FILENAME
    echo "rm -rf $ANA_PATH/$ECM/data_${ECM}_side3_low.root $ANA_PATH/$ECM/data_${ECM}_side3_up.root" >> $FILENAME
    echo "rm -rf $ANA_PATH/$ECM/data_${ECM}_side4_low.root $ANA_PATH/$ECM/data_${ECM}_side4_up.root" >> $FILENAME
    # echo "rm -rf $ANA_PATH/$ECM/data_${ECM}_side1_low_raw.root $ANA_PATH/$ECM/data_${ECM}_side1_up_raw.root" >> $FILENAME
    # echo "rm -rf $ANA_PATH/$ECM/data_${ECM}_side2_low_raw.root $ANA_PATH/$ECM/data_${ECM}_side2_up_raw.root" >> $FILENAME
    # echo "rm -rf $ANA_PATH/$ECM/data_${ECM}_side3_low_raw.root $ANA_PATH/$ECM/data_${ECM}_side3_up_raw.root" >> $FILENAME
    # echo "rm -rf $ANA_PATH/$ECM/data_${ECM}_side4_low_raw.root $ANA_PATH/$ECM/data_${ECM}_side4_up_raw.root" >> $FILENAME
    echo "./apply_cuts.py $SOURCE_PATH/$ECM/data_${ECM}.root $ANA_PATH/$ECM/data_${ECM}_signal_before_mass.root signal before_mass" >> $FILENAME
done
