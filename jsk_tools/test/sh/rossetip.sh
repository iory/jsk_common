#!/bin/bash

_THIS_DIR=$(builtin cd "`dirname "${BASH_SOURCE[0]}"`" > /dev/null && pwd)
source $_THIS_DIR/../../env-hooks/99.jsk_tools.bash

ECHO_OUTPUT="$1"
ip() {
    echo "$ECHO_OUTPUT"
}
CATKIN_SHELL="bash"
rossetip
