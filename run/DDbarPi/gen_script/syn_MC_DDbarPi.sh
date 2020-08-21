#/bin/bash

BOSS=$1
TYPE=$2

if [ "$BOSS" = "703" ]; then
    ECMS=("4600")
fi

if [ "$BOSS" = "705" ]; then
    # ECMS=("4620" "4640" "4660" "4680" "4700")
    ECMS=("4680")
fi

for ECM in ${ECMS[@]}; do
    cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/rootfile/mc/$TYPE/$ECM
    rm -rf DDbarPi_mc_${TYPE}_${ECM}.root
    hadd DDbarPi_mc_${TYPE}_${ECM}.root *.root
done
