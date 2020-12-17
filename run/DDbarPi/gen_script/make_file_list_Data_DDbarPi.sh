#/bin/bash

BOSS=$1

if [ "$BOSS" = "703" ]; then
    ECMS=("4600")
    PATH="/besfs3/offline/data/703-1/xyz/round07/"
fi

if [ "$BOSS" = "705" ]; then
    # ECMS=("4612" "4620" "4640" "4660" "4680" "4700")
    ECMS=("4680" "4700")
    PATH="/besfs4/offline/data/705-1/xyz/round13/"
fi

FILENAME="Gen_FileList_Data_"$BOSS
echo "#!/usr/bin/env bash" > $FILENAME
echo "cd /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT" >> $FILENAME 
for ECM in ${ECMS[@]}; do
    echo "rm -r /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/samples/data/$ECM/*txt" >> $FILENAME
    echo "./python/get_samples.py $PATH$ECM/dst ./run/DDbarPi/samples/data/$ECM/data_${ECM}_DDbarPi.txt 20G" >> $FILENAME
done
