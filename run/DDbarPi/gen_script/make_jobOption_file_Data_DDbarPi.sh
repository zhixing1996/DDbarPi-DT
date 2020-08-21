#!/bin/bash

BOSS=$1
if [ "$BOSS" = "703" ]; then
    ECMS=("4600")
    CMS=("4.600")
fi

if [ "$BOSS" = "705" ]; then
    # ECMS=("4612" "4620" "4640" "4660" "4680" "4700")
    # CMS=("4.61208" "4.63129" "4.64366" "4.66414" "4.68271" "4.70044")
    ECMS=("4680")
    CMS=("4.681208")
fi

COUNT=0
for ECM in ${ECMS[@]}; do
    NUM_UP=$(ls -l /besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/samples/data/$ECM | grep "txt" | wc -l)
    JobText_SaveDir=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/jobs_text/data/$ECM
    ROOT_SaveDir=/besfs/groups/cal/dedx/$USER/bes/DDbarPi-DT/run/DDbarPi/rootfile/data/$ECM
    mkdir -p $JobText_SaveDir
    mkdir -p $ROOT_SaveDir
    rm -r $JobText_SaveDir/*txt
    rm -r $ROOT_SaveDir/*.root
    
    for ((num = 0; num <= $NUM_UP; num++))
    do
        file_list=data_${ECM}_DDbarPi_20G-${num}.txt
        rootfile=DDbarPi_data_$ECM-${num}.root
        jobOptions=jobOptions_DDbarPi_data_$ECM-${num}.txt
        echo "#include \"\$ROOTIOROOT/share/jobOptions_ReadRec.txt\"                                        "  > ${JobText_SaveDir}/${jobOptions}
        echo "#include \"\$MAGNETICFIELDROOT/share/MagneticField.txt\"                                      " >> ${JobText_SaveDir}/${jobOptions}
        echo "#include \"\$DTAGALGROOT/share/jobOptions_dTag.txt\"                                          " >> ${JobText_SaveDir}/${jobOptions}
        echo "#include \"\$SIMPLEPIDSVCROOT/share/jobOptions_SimplePIDSvc.txt\"                             " >> ${JobText_SaveDir}/${jobOptions}
        echo "#include \"\$VERTEXFITROOT/share/jobOptions_VertexDbSvc.txt\"                                 " >> ${JobText_SaveDir}/${jobOptions}
        if [ "$BOSS" = "705" ]; then
            echo "#include \"\$MEASUREDECMSSVCROOT/share/anaOptions.txt\"                                   " >> ${JobText_SaveDir}/${jobOptions}
        fi
        echo "#include \"/besfs/groups/cal/dedx/\$USER/bes/DDbarPi-DT/run/DDbarPi/samples/data/$ECM/$file_list\" " >> ${JobText_SaveDir}/${jobOptions}
        echo "                                                                                              " >> ${JobText_SaveDir}/${jobOptions}
        echo "NeutralDReconstruction.ReadBeamEFromDB = false;                                               " >> ${JobText_SaveDir}/${jobOptions}
        echo "NeutralDReconstruction.UseCalibBeamE = false;                                                 " >> ${JobText_SaveDir}/${jobOptions}
        echo "NeutralDReconstruction.BeamE = 2.09;                                                          " >> ${JobText_SaveDir}/${jobOptions}
        echo "NeutralDSelector.UseMbcCuts = false;                                                          " >> ${JobText_SaveDir}/${jobOptions}
        echo "NeutralDSelector.UseDeltaECuts = false;                                                       " >> ${JobText_SaveDir}/${jobOptions}
        echo "NeutralDSelector.UseDeltaMassCuts = true;                                                     " >> ${JobText_SaveDir}/${jobOptions}
        echo "NeutralDSelector.DDeltaMassMinCut = -0.12;                                                    " >> ${JobText_SaveDir}/${jobOptions}
        echo "NeutralDSelector.DDeltaMassMaxCut =  0.12;                                                    " >> ${JobText_SaveDir}/${jobOptions}
        echo "ChargedDReconstruction.ReadBeamEFromDB = false;                                               " >> ${JobText_SaveDir}/${jobOptions}
        echo "ChargedDReconstruction.UseCalibBeamE = false;                                                 " >> ${JobText_SaveDir}/${jobOptions}
        echo "ChargedDSelector.UseMbcCuts = false;                                                          " >> ${JobText_SaveDir}/${jobOptions}
        echo "ChargedDSelector.UseDeltaECuts = false;                                                       " >> ${JobText_SaveDir}/${jobOptions}
        echo "ChargedDSelector.UseDeltaMassCuts = true;                                                     " >> ${JobText_SaveDir}/${jobOptions}
        echo "ChargedDSelector.DDeltaMassMinCut = -0.12;                                                    " >> ${JobText_SaveDir}/${jobOptions}
        echo "ChargedDSelector.DDeltaMassMaxCut =  0.12;                                                    " >> ${JobText_SaveDir}/${jobOptions}
        echo "LocalKaonSelector.useSimplePID=false;                                                         " >> ${JobText_SaveDir}/${jobOptions}
        echo "LocalPionSelector.useSimplePID=false;                                                         " >> ${JobText_SaveDir}/${jobOptions}
        echo "                                                                                              " >> ${JobText_SaveDir}/${jobOptions}
        echo "ApplicationMgr.DLLs += {\"PiD0DmAlg\"};                                                       " >> ${JobText_SaveDir}/${jobOptions}
        echo "ApplicationMgr.TopAlg +={ \"PiD0Dm\" };                                                       " >> ${JobText_SaveDir}/${jobOptions}
        echo "PiD0Dm.AddModesD0 = {0,1,3,4};                                                                " >> ${JobText_SaveDir}/${jobOptions}
        echo "PiD0Dm.AddModesDp = {200,201,202,203,204};                                                    " >> ${JobText_SaveDir}/${jobOptions}
        echo "PiD0Dm.Ecms = ${CMS[$COUNT]};                                                                 " >> ${JobText_SaveDir}/${jobOptions}
        echo "                                                                                              " >> ${JobText_SaveDir}/${jobOptions}
        echo "// Set output level threshold (2=DEBUG, 3=INFO, 4=WARNING, 5=ERROR, 6=FATAL )                 " >> ${JobText_SaveDir}/${jobOptions}
        echo "MessageSvc.OutputLevel = 6;                                                                   " >> ${JobText_SaveDir}/${jobOptions}
        echo "                                                                                              " >> ${JobText_SaveDir}/${jobOptions}
        echo "// Number of events to be processed (default is 10)                                           " >> ${JobText_SaveDir}/${jobOptions}
        echo "ApplicationMgr.EvtMax = -1;                                                                   " >> ${JobText_SaveDir}/${jobOptions}
        echo "                                                                                              " >> ${JobText_SaveDir}/${jobOptions}
        echo "ApplicationMgr.HistogramPersistency = \"ROOT\";                                               " >> ${JobText_SaveDir}/${jobOptions}
        echo "NTupleSvc.Output = {\"FILE1 DATAFILE='$ROOT_SaveDir/$rootfile' OPT='NEW' TYP='ROOT'\"};       " >> ${JobText_SaveDir}/${jobOptions}
    done
    COUNT=$COUNT+1
done
