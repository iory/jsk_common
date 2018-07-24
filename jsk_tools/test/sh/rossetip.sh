#!/bin/bash

_THIS_DIR=$(builtin cd "`dirname "${BASH_SOURCE[0]}"`" > /dev/null && pwd)
source $_THIS_DIR/../../env-hooks/99.jsk_tools.bash

ECHO_OUTPUT="$1"
ip() {
    echo "$ECHO_OUTPUT"
}
rossetip > /dev/null 2>&1
echo $ROS_IP
