#!/bin/bash
# used for config kernel options.
# ==========================================
# ABOUT CONFIG LANGUAGE
# *** config kernel module.
#     m: modules, y: build-in, s: separate.
# line 1:   VALUE-NAME.
# line 2:   OPTIONS.
# line 3:   default value index in OPTIONS
# line 4:   comment(optional).
# eg. 
# MFS-CONFIG
# "m" "y" "s"
# 0
# 
#
# 
#
# 

CONFIGFILE=configs.menu
LINE1=
VALUE_NAME=
LINE2=
OPTIONS=
LINE3=
DEFAULT=
LINE4=
COMMENT=

function main(){
    while read line
    do
        if [ ${#line} == 0 ]; then continue; else
            echo $line | grep '^ \+#' > /dev/null;
            if [ 0 == $? ]; then continue; else
                echo ${#line}
            fi
        fi
    done < $CONFIGFILE
}
main

exit
