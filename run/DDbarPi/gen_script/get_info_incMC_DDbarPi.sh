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
    CMS=("4.600")
fi

if [ "$BOSS" = "705" ]; then
    # SAMPLES=("4612" "4620" "4640" "4660" "4680" "4700")
    # CMS=("4.61208" "4.63129" "4.64366" "4.66414" "4.68271" "4.70044")
    SAMPLES=("4680")
    CMS=("4.681208")
fi

COUNT=0
FILENAME="Get_Info_incMC_${TYPE}_"$BOSS
echo "#!/usr/bin/env bash" > $FILENAME
echo "cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT" >> $FILENAME 
for SAMPLE in ${SAMPLES[@]}; do
    mkdir -p $ANA_PATH/$SAMPLE
    echo "./python/get_info.py $SOURCE_PATH/$SAMPLE/DDbarPi_incMC_${TYPE}_${SAMPLE}.root $ANA_PATH/$SAMPLE/incMC_${TYPE}_${SAMPLE}.root ${CMS[$COUNT]} signal" >> $FILENAME
    COUNT=$COUNT+1
done
