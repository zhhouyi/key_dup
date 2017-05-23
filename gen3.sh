#!/bin/bash

# path of fsl trace
fsl='/dataset/fsl/2013'

# users considered in backups
users=('004' '007' '008' '012' '013' '015' '022' '026' '028')

# auxiliary information
date_of_month=('2013-01-' '2013-02-' '2013-03-' '2013-04-' '2013-05-' '2013-06-')
date_of_day=('01' '02' '03' '04' '05' '06' '07' '08' '09' '10' '11' '12' '13' '14' '15' '16' '17' '18' '19' '20' '21' '22' '23' '24' '25' '26' '27' '28' '29' '30' '31')
#date_of_aux=('2013-01-22')

# parameters

# count prior backups (as auxiliary information) and launch frequency analysis
for aux1 in ${date_of_month[@]}; do
        for aux2 in ${date_of_day[@]}; do
        for user in ${users[@]}; do
                snapshot="fslhomes-user${user}-${aux1}${aux2}"
                if [ -f "${fsl}"/${snapshot}.tar.gz ]; then
                        tar zxf "${fsl}"/${snapshot}.tar.gz
                        hf-stat -h ${snapshot}/${snapshot}.8kb.hash.anon > tmp2/${snapshot}
                        ./c_key tmp2/${snapshot}
                        rm -rf ${snapshot}
                        rm -rf tmp2/${snapshot}
                fi
        done
        done
         # launch frequency analysis
done

