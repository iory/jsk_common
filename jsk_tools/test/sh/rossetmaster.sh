#!/bin/bash

JSK_TOOLS_PATH=$(rospack find jsk_tools)
if [ "$JSK_TOOLS_PATH" = "/opt/ros/${ROS_DISTRO}/share/jsk_tools" ]; then
    source /opt/ros/${ROS_DISTRO}/setup.bash
else
    source `catkin locate --shell-verbs`
    cd $(rospack find jsk_tools) && source $(catkin locate)/devel/etc/catkin/profile.d/99.jsk-tools.sh
fi

rossetmaster "$1" "${2-11311}" > /dev/null 2>&1
echo $ROS_MASTER_URI
