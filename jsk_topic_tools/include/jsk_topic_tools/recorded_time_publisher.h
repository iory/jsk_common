/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2022, JSK Lab
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


#ifndef JSK_TOPIC_TOOLS_RECORDED_TIME_PUBLISHER_H_
#define JSK_TOPIC_TOOLS_RECORDED_TIME_PUBLISHER_H_

#include <ros/ros.h>
#include <pluginlib/class_loader.hpp>
#include <image_transport/image_transport.h>
#include "image_transport/publisher_plugin.h"
#include "image_transport/subscriber_plugin.h"


namespace jsk_topic_tools
{
  class RecordedTimePublisher : public ros::Publisher
  {
  public:
    RecordedTimePublisher();
    RecordedTimePublisher(const ros::Publisher& rhs);
    RecordedTimePublisher(const RecordedTimePublisher& rhs);
    template <typename M>
    void publish(const boost::shared_ptr<M>& message) {
      last_published_time_ = ros::Time::now();
      Publisher::publish<M>(message);
    }

    template <typename M>
    void publish(const M& message)
    {
      last_published_time_ = ros::Time::now();
      ROS_ERROR("publish %lf", last_published_time_.toSec());
      Publisher::publish<M>(message);
    }

    ros::Time last_published_time_;
  };

  class RecordedTimeImagePublisher : public image_transport::Publisher
  {
  public:
    RecordedTimeImagePublisher(const ros::NodeHandle& nh);

    ros::Time last_published_time_;

  private:
    RecordedTimeImagePublisher(ros::NodeHandle& nh, const std::string& base_topic, uint32_t queue_size,
                               const image_transport::SubscriberStatusCallback& connect_cb,
                               const image_transport::SubscriberStatusCallback& disconnect_cb,
                               const ros::VoidPtr& tracked_object, bool latch,
                               const image_transport::PubLoaderPtr& loader) {

    }
    friend class image_transport::ImageTransport;
  };

  struct Impl
  {
    ros::NodeHandle nh_;
    image_transport::PubLoaderPtr pub_loader_;
    image_transport::SubLoaderPtr sub_loader_;

    Impl(const ros::NodeHandle& nh)
      : nh_(nh),
        pub_loader_( boost::make_shared<image_transport::PubLoader>("image_transport", "image_transport::PublisherPlugin") ),
        sub_loader_( boost::make_shared<image_transport::SubLoader>("image_transport", "image_transport::SubscriberPlugin") )
    {
    }
  };


  class RecordedTimeImageTransport : public image_transport::ImageTransport
  {
  public:
    explicit RecordedTimeImageTransport(const ros::NodeHandle& nh)
      : ImageTransport(nh), recorded_time_impl_(new Impl(nh)) {};

    RecordedTimeImageTransport advertise(const std::string& base_topic, uint32_t queue_size,
                                         const image_transport::SubscriberStatusCallback& connect_cb,
                                         const image_transport::SubscriberStatusCallback& disconnect_cb = image_transport::SubscriberStatusCallback(),
                                         const ros::VoidPtr& tracked_object = ros::VoidPtr(), bool latch = false) {
      return RecordedTimeImagePublisher(recorded_time_impl_->nh_, base_topic, queue_size, connect_cb, disconnect_cb, tracked_object, latch, recorded_time_impl_->pub_loader_);
    }

    // CameraPublisher advertiseCamera(const std::string& base_topic, uint32_t queue_size,
    //                                 const SubscriberStatusCallback& image_connect_cb,
    //                                 const SubscriberStatusCallback& image_disconnect_cb = SubscriberStatusCallback(),
    //                                 const ros::SubscriberStatusCallback& info_connect_cb = ros::SubscriberStatusCallback(),
    //                                 const ros::SubscriberStatusCallback& info_disconnect_cb = ros::SubscriberStatusCallback(),
    //                                 const ros::VoidPtr& tracked_object = ros::VoidPtr(), bool latch = false) {
    //   return RecordedTimeCameraPublisher(*this, impl_->nh_, base_topic, queue_size, image_connect_cb, image_disconnect_cb,
    //                                      info_connect_cb, info_disconnect_cb, tracked_object, latch);
    // }
  private:
    struct Impl;
    typedef boost::shared_ptr<Impl> ImplPtr;
    typedef boost::weak_ptr<Impl> ImplWPtr;

    ImplPtr recorded_time_impl_;
  };

}

#endif
