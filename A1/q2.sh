#!/bin/sh

function mergeSort(){
    local a=("$@")
    if [ ${#a[@]} -eq 1 ];
    then
        echo ${a[@]}
    elif [ ${#a[@]} -eq 2 ];
    then
        if [ ${a[0]} -gt ${a[1]} ];
        then
            echo ${a[1]} ${a[0]}
        else
            echo ${a[@]}
        fi
    else
        local mid=($(( ${#a[@]} / 2 )))
        local m1=($(mergeSort "${a[@]::mid}"))
        local m2=($(mergeSort "${a[@]:mid}"))
        local ans=()

        while [ ${#m1[@]} -gt 0 ] && [ ${#m2[@]} -gt 0 ];
        do
            if [ ${m1[0]} -gt ${m2[0]} ];
            then
                ans+=("${m2[0]}")
                m2=("${m2[@]:1}")
            else
                ans+=("${m1[0]}")
                m1=("${m1[@]:1}")
            fi
        done
        echo ${ans[@]} ${m1[@]} ${m2[@]}
    fi
}

read -a arr
sorted=($(mergeSort "${arr[@]}"))
echo ${sorted[@]}