#/bin/bash

BOSS=$1

if [ "$BOSS" = "703" ]; then
    ECMS=("4600")
fi

if [ "$BOSS" = "705" ]; then
    # ECMS=("4612" "4620" "4640" "4660" "4680" "4700")
    ECMS=("4680")
fi

for ECM in ${ECMS[@]}; do
    cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/rootfile/data/$ECM
    if [ "$ECM" == "4600" ]; then
        rm -rf DDbarPi_data_4600-0.root
    fi
    if [ "$ECM" == "4680" ]; then
        rm -rf DDbarPi_data_4680-0.root
    fi
    rm -rf DDbarPi_data_${ECM}.root
    hadd DDbarPi_data_${ECM}.root *.root
done
