#!/bin/bash

start=10000000
base=100000
for i in `seq 1 20`
do
    end=$[base]
    end=$[start+end]
    echo $start $end
    cat ../../adfea_control_oversea_increa/history_adfea/train_ins_new_3k.shuffle | head -n $end | tail -n +$start > train.ins.incre
    cd ../gbdt
    ./bin/gbdt_predict ./conf/gbdt.conf
    cd ../fm
    ./bin/fmftrl ./conf/fmftrl.conf
    ./bin/predict_fmftrl ./conf/fmftrl.conf
    python ./bin/cal_metric.py result.dat >> test.result
    start=$end
done
