#/bin/bash

BOSS=$1

SOURCE_PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/rootfile/data
ANA_PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/data
LOG_PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/logfile
mkdir -p $ANA_PATH
mkdir -p $LOG_PATH
if [ "$BOSS" = "703" ]; then
    SAMPLES=("4600")
fi

if [ "$BOSS" = "705" ]; then
    # SAMPLES=("4612" "4620" "4640" "4660" "4680" "4700")
    SAMPLES=("4680")
fi

FILENAME="Get_Info_Data_"$BOSS
echo "#!/usr/bin/env bash" > $FILENAME
echo "cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT" >> $FILENAME 
for SAMPLE in ${SAMPLES[@]}; do
    mkdir -p $ANA_PATH/$SAMPLE
    echo "./python/get_info.py $SOURCE_PATH/$SAMPLE/DDbarPi_data_${SAMPLE}.root $ANA_PATH/$SAMPLE/data_${SAMPLE}.root" >> $FILENAME
done
