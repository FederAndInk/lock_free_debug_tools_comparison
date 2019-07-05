#!/bin/env bash

inspxe-cl -collect=ti-3 -r=inspxe -- "$@"

export SAN_LAUNCH_SEARCH=$HOME

export GREP_COLOR="1;95"
inspxe-cl -report=problems -report-all -r=inspxe | grep --color=always -E "$SAN_LAUNCH_SEARCH.*\\.(cpp|hpp|h|cc|cxx|c)(:[0-9]*)*|$"
/bin/rm -r inspxe
