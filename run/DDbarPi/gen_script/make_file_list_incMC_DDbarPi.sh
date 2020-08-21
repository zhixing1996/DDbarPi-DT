#/bin/bash

BOSS=$1
TYPE=$2

if [ "$BOSS" = "703" ]; then
    if [ "$TYPE" = "qq" ]; then
        PATH=/besfs/groups/psip/psipgroup/664p01-MC/4600/qqbar
    fi
    if [ "$TYPE" = "DD" ]; then
        PATH=/besfs/groups/psip/psipgroup/664p01-MC/4600/DD
    fi
    if [ "$TYPE" = "LcLc" ]; then
        PATH=/besfs/groups/psip/psipgroup/664p01-MC/4600/LL
    fi
    ECMS=4600
fi

if [ "$BOSS" = "705" ]; then
    if [ "$TYPE" = "hadron" ]; then
        PATH=/besfs/groups/psipp/psippgroup/service/705/genericMC/4680/hadron/dst
    fi
    if [ "$TYPE" = "LcLc" ]; then
        PATH=/besfs/groups/psipp/psippgroup/service/705/genericMC/4680/LcLc1/dst
    fi
    ECMS=4680
fi

FILENAME="Gen_FileList_incMC_"${BOSS}"_"$TYPE
echo "#!/usr/bin/env bash" > $FILENAME
echo "cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT" >> $FILENAME 
echo "rm -r /besfs/groups/cal/dedx/$USER/bes/DDarPi-DT/run/DDbarPi/samples/mc/$TYPE/$ECMS/*txt" >> $FILENAME
echo "./python/get_samples.py $PATH ./run/DDbarPi/samples/mc/$TYPE/$ECMS/incMC_${TYPE}_${ECMS}_DDbarPi.txt 20G" >> $FILENAME
