#!/bin/sh
BOSS=$1
if [ "$BOSS" = "703" ]; then
    ECMS=("4600")
fi
if [ "$BOSS" = "705" ]; then
    # ECMS=("4620" "4640" "4660" "4680" "4700")
    ECMS=("4680")
fi

WORKAREA=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT
for ECM in ${ECMS[@]}; do
    mkdir -p $WORKAREA/run/DDbarPi/rootfile/mc/DDPI/$ECM
    mkdir -p $WORKAREA/run/DDbarPi/jobs_text/mc/DDPI/$ECM
    cd $WORKAREA/run/DDbarPi/jobs_text/mc/DDPI/$ECM
    rm -rf mc_D_D_PI_PHSP_$ECM*txt
    cp -rf $WORKAREA/python/make_mc.py ./
    cp -rf $WORKAREA/python/tools.py ./
    ./make_mc.py /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/dst/mc/DDPI/$ECM mc D_D_PI PHSP DDPI $ECM 2
    cp -rf /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/gen_script/gen_mc/subAna.sh ./
    rm -rf *boss*
    rm -rf $WORKAREA/run/DDbarPi/rootfile/mc/DDPI/$ECM/*root
    ./subAna.sh $ECM D_D_PI_PHSP
done
