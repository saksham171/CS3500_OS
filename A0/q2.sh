#!/bin/sh
read pswd
len=$((${#pswd}))
c1=0
c2=0
c3=0
c4=0
c5=0
if [ $len -ge 8 ]
then 
    c4=1
    #echo $c4
elif [ $len -ge 6 ]
then    
    c4=-1
    #echo $c4
fi
for (( counter=0; counter<$len; counter++ ))
do
    if [[ ( ${pswd:counter:1} == "!" || ${pswd:counter:1} == \* || ${pswd:counter:1} == \& || ${pswd:counter:1} == \% || ${pswd:counter:1} == \@ || ${pswd:counter:1} == \$ || ${pswd:counter:1} == \# || ${pswd:counter:1} == \^ || ${pswd:counter:1} == \( || ${pswd:counter:1} == \) || ${pswd:counter:1} == \- || ${pswd:counter:1} == \+ ) ]];
    then
        c3=1
        #echo $c3
    elif [[ ( ${pswd:counter:1} == [[:lower:]] ) ]];
    then
        c2=1
        #echo $c2
    elif [[ ( ${pswd:counter:1} == [[:upper:]] ) ]];
    then
        c1=1
        #echo $c1
    elif [[ ( ${pswd:counter:1} -ge '0' && ${pswd:counter:1} -le '9' ) ]];
    then
        c5=1
        #echo $c5
    else
        echo "Invalid Character"
    fi
done
#echo "$c1 $c2 $c3 $c4 $c5"
if [[ ( $c1 -ge 1 && $c2 -ge 1 && $c3 -ge 1 && $c4 -ge 1 && $c5 -ge 1 ) ]];
then
    echo "Strong password"
elif [[ ( $c1 -ge 1 && $c2 -ge 1 && $c3 -ge 1 && $c4 -le 0 ) ]];
then
    echo "Moderate password"
else
    echo "Weak password"
fi