#!/bin/env bash

# for more precise file search edit PATH_COLORIZER_SEARCH variable
# eg: PATH_COLORIZER_SEARCH="$HOME" path_colorizer.sh ./a.out
# eg: PATH_COLORIZER_SEARCH="$PWD" path_colorizer.sh ./a.out

if (($# < 1)); then
  echo $(basename $0) executable [args...]
  echo "you can set PATH_COLORIZER_SEARCH variable to show specific path"
  echo 'eg: PATH_COLORIZER_SEARCH="$HOME" '$exe' ./a.out'
  exit 1
fi

if [ -z "$PATH_COLORIZER_SEARCH" ]; then
  PATH_COLORIZER_SEARCH="/"
elif [[ "$PATH_COLORIZER_SEARCH" != /* ]]; then
  PATH_COLORIZER_SEARCH="/.*$PATH_COLORIZER_SEARCH"
fi

export GREP_COLOR="1;95"
args="$@"
script /dev/null -q -c "$args" | grep --color=always -E "$PATH_COLORIZER_SEARCH.*\\.(cpp|hpp|h|cc|cxx|c)(:[0-9]*)*|$"
