#/bin/bash

BOSS=$1

SOURCE_PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/rootfile/data
ANA_PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data
LOG_PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/logfile
mkdir -p $ANA_PATH
mkdir -p $LOG_PATH
if [ "$BOSS" = "703" ]; then
    SAMPLES=("4600")
    CMS=("4.600")
fi

if [ "$BOSS" = "705" ]; then
    # SAMPLES=("4612" "4620" "4640" "4660" "4680" "4700")
    # CMS=("4.61208" "4.63129" "4.64366" "4.66414" "4.68271" "4.70044")
    SAMPLES=("4680" "4700")
    CMS=("4.681208" "4.70044")
fi

COUNT=0
FILENAME="Get_Info_Data_"$BOSS
echo "#!/usr/bin/env bash" > $FILENAME
echo "cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT" >> $FILENAME 
for SAMPLE in ${SAMPLES[@]}; do
    mkdir -p $ANA_PATH/$SAMPLE
    echo "./python/get_info.py $SOURCE_PATH/$SAMPLE/DDbarPi_data_${SAMPLE}.root $ANA_PATH/$SAMPLE/data_${SAMPLE}.root ${CMS[$COUNT]} signal" >> $FILENAME
    echo "./python/get_info.py $SOURCE_PATH/$SAMPLE/DDbarPi_data_${SAMPLE}.root $ANA_PATH/$SAMPLE/data_${SAMPLE}_side1_low_raw.root ${CMS[$COUNT]} side1_low" >> $FILENAME
    echo "./python/get_info.py $SOURCE_PATH/$SAMPLE/DDbarPi_data_${SAMPLE}.root $ANA_PATH/$SAMPLE/data_${SAMPLE}_side2_low_raw.root ${CMS[$COUNT]} side2_low" >> $FILENAME
    echo "./python/get_info.py $SOURCE_PATH/$SAMPLE/DDbarPi_data_${SAMPLE}.root $ANA_PATH/$SAMPLE/data_${SAMPLE}_side3_low_raw.root ${CMS[$COUNT]} side3_low" >> $FILENAME
    echo "./python/get_info.py $SOURCE_PATH/$SAMPLE/DDbarPi_data_${SAMPLE}.root $ANA_PATH/$SAMPLE/data_${SAMPLE}_side4_low_raw.root ${CMS[$COUNT]} side4_low" >> $FILENAME
    echo "./python/get_info.py $SOURCE_PATH/$SAMPLE/DDbarPi_data_${SAMPLE}.root $ANA_PATH/$SAMPLE/data_${SAMPLE}_side1_up_raw.root ${CMS[$COUNT]} side1_up" >> $FILENAME
    echo "./python/get_info.py $SOURCE_PATH/$SAMPLE/DDbarPi_data_${SAMPLE}.root $ANA_PATH/$SAMPLE/data_${SAMPLE}_side2_up_raw.root ${CMS[$COUNT]} side2_up" >> $FILENAME
    echo "./python/get_info.py $SOURCE_PATH/$SAMPLE/DDbarPi_data_${SAMPLE}.root $ANA_PATH/$SAMPLE/data_${SAMPLE}_side3_up_raw.root ${CMS[$COUNT]} side3_up" >> $FILENAME
    echo "./python/get_info.py $SOURCE_PATH/$SAMPLE/DDbarPi_data_${SAMPLE}.root $ANA_PATH/$SAMPLE/data_${SAMPLE}_side4_up_raw.root ${CMS[$COUNT]} side4_up" >> $FILENAME
    COUNT=$COUNT+1
done
