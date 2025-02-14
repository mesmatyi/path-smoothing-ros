/*********************************************************************
*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2016, George Kouros.
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
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the the copyright holder nor the names of its
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
*
* Author:  George Kouros
*********************************************************************/

#include "path_smoothing_ros/cubic_spline_interpolator.h"
#include <tf/tf.h>

ros::Publisher smoothedPathPub;

void path_callback(const nav_msgs::PathPtr& msg)
{
  nav_msgs::Path path, smoothedPath;
  path.header.frame_id = "map";
  geometry_msgs::PoseStamped pose;
  pose.header.frame_id = "map";

  for (int i = 0; i < msg->poses.size(); i++)
  {
    pose.pose.position.x = static_cast<double>(msg->poses[i].pose.position.x);
    pose.pose.position.y = static_cast<double>(msg->poses[i].pose.position.y);
    pose.pose.orientation = msg->poses[i].pose.orientation;
    path.poses.push_back(pose);
  }

  // create a cubic spline interpolator
  path_smoothing::CubicSplineInterpolator csi("lala");
    // pointsPerUnit, skipPoints, useEndConditions, useMiddleConditions);
  csi.interpolatePath(path, smoothedPath);

  smoothedPathPub.publish(smoothedPath);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "path_smoothing_ros_demo");
  ros::NodeHandle nh;

  smoothedPathPub = nh.advertise<nav_msgs::Path>("smoothed_path", 1, true);

  ros::Subscriber raw_sub = nh.subscribe("raw_path",1000,path_callback);

  int pointsPerUnit, skipPoints;
  bool useEndConditions, useMiddleConditions;

  nh.param<int>("points_per_unit", pointsPerUnit, 5);
  nh.param<int>("skip_points", skipPoints, 2);
  nh.param<bool>("use_end_conditions", useEndConditions, true);
  nh.param<bool>("use_middle_conditions", useMiddleConditions, true);

  ros::spin();

  return 0;
}
