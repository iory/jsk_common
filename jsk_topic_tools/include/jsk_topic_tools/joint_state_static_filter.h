// -*- mode: c++ -*-
/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2021, JSK Lab
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/o2r other materials provided
 *     with the distribution.
 *   * Neither the name of the JSK Lab nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/


#ifndef JSK_TOPIC_TOOLS_JOINT_STATE_STATIC_FILTER_H_
#define JSK_TOPIC_TOOLS_JOINT_STATE_STATIC_FILTER_H_

#include <jsk_topic_tools/diagnostic_nodelet.h>
#include <boost/tuple/tuple.hpp>
#include <float.h>
#include <sensor_msgs/JointState.h>
#include <boost/circular_buffer.hpp>
#include <jsk_topic_tools/vital_checker.h>
#include <topic_tools/shape_shifter.h>

namespace jsk_topic_tools
{

  class JointStateStaticFilter: public jsk_topic_tools::DiagnosticNodelet
  {
  public:
    typedef boost::tuple<ros::Time, bool> StampedBool;
    JointStateStaticFilter(): DiagnosticNodelet("JointStateStaticFilter"),
                              buf_(100),
                              eps_(0.00001) { }
  protected:
    ////////////////////////////////////////////////////////
    // methods
    ////////////////////////////////////////////////////////
    virtual void onInit();
    virtual void filter(const ros::MessageEvent<topic_tools::ShapeShifter>& msg_event);
    virtual void jointStateCallback(const sensor_msgs::JointState::ConstPtr& msg);
    virtual bool isStatic(const ros::Time& stamp);
    virtual void updateDiagnostic(
      diagnostic_updater::DiagnosticStatusWrapper &stat);
    virtual std::vector<double> filterJointState(
      const sensor_msgs::JointState::ConstPtr& msg);
    virtual void subscribe();
    virtual void unsubscribe();
    virtual ros::Publisher advertise(
      boost::shared_ptr<topic_tools::ShapeShifter const> msg,
      const std::string& topic);
    ////////////////////////////////////////////////////////
    // ROS variables
    ////////////////////////////////////////////////////////
    ros::Subscriber sub_input_;
    ros::Subscriber sub_joint_;
    ros::Publisher pub_;
    boost::circular_buffer<StampedBool> buf_;
    std::vector<double> previous_joints_;
    jsk_topic_tools::VitalChecker::Ptr joint_vital_;
    boost::mutex mutex_;
    ////////////////////////////////////////////////////////
    // parameters
    ////////////////////////////////////////////////////////
    std::vector<std::string> joint_names_;
    double eps_;
  private:
  };
}

#endif
