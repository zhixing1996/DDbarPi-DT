#/bin/bash

BOSS=$1
TYPE=$2

SOURCE_PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/rootfile/mc/$TYPE
ANA_PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/anaroot/mc/$TYPE
LOG_PATH=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/logfile
mkdir -p $ANA_PATH
mkdir -p $LOG_PATH
if [ "$BOSS" = "703" ]; then
    SAMPLES=("4600")
fi

if [ "$BOSS" = "705" ]; then
    SAMPLES=("4680")
fi

FILENAME="Get_Info_incMC_${TYPE}_"$BOSS
echo "#!/usr/bin/env bash" > $FILENAME
echo "cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT" >> $FILENAME 
for SAMPLE in ${SAMPLES[@]}; do
    mkdir -p $ANA_PATH/$SAMPLE
    echo "./python/get_info.py $SOURCE_PATH/$SAMPLE/DDbarPi_incMC_${TYPE}_${SAMPLE}.root $ANA_PATH/$SAMPLE/incMC_${TYPE}_${SAMPLE}.root" >> $FILENAME
done
