#ifndef ENCODERPLUGIN_HH_
#define ENCODERPLUGIN_HH_

#include "gazebo/common/Plugin.hh"
#include <ros/ros.h>
#include "gazebo/physics/physics.hh"
#include "gazebo/gazebo.hh"
#include <std_msgs/String.h>
#include <sstream>
#include "gazebo/common/Events.hh"


namespace gazebo
{
  class GAZEBO_VISIBLE Encoder : public ModelPlugin
  {
    	public: Encoder();
    	public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);
		private: virtual void OnUpdate();
	
	
    	private: physics::ModelPtr model;
    	private: physics::JointPtr leftJoint, rightJoint;
    	private: event::ConnectionPtr connection;
		private: double wheelSpeed[2];
		ros::NodeHandle nh;
		ros::Publisher publisher;
   
		/*
    	private: common::Time prevUpdateTime;
    	private: common::Time updatePeriod;
    	*/
  };
}
#endif
