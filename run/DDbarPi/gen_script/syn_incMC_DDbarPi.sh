#/bin/bash

BOSS=$1
TYPE=$2

if [ "$BOSS" = "703" ]; then
    ECMS=("4600")
fi

if [ "$BOSS" = "705" ]; then
    ECMS=("4680")
fi

for ECM in ${ECMS[@]}; do
    cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/rootfile/mc/$TYPE/$ECM
    if [ "$ECM" == "4600" ]; then
        if [ "$TYPE" == "DD" ]; then
            rm -rf DDbarPi_incMC_DD_4600-0.root
        fi
    fi
    if [ "$ECM" == "4680" ]; then
        if [ "$TYPE" == "hadron" ]; then
            rm -rf DDbarPi_incMC_hadron_4680-0.root
        fi
    fi
    rm -rf DDbarPi_incMC_${TYPE}_${ECM}.root
    hadd DDbarPi_incMC_${TYPE}_${ECM}.root *.root
done
