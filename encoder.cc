#include <gazebo/gazebo.hh>
#include <boost/bind.hpp>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>
#include "gazebo/common/Plugin.hh"
#include <sstream>
#include <stdio.h>
#include <gazebo/physics/Joint.hh> 


namespace gazebo
{
  class EncoderPlugin : public ModelPlugin
  {
    	public: EncoderPlugin();
    	public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
	{
		this->model = _parent;
		this->leftJoint = this->model->GetJoint("left_joint");
      		this->rightJoint = this->model->GetJoint("right_joint");
		this->updateConnection = event::Events::ConnectWorldUpdateBegin(
		std::bind(&EncoderPlugin::OnUpdate, this));
		printf("inside load method\n");
	}
      	
     
	private: virtual void OnUpdate(){
		double rightWheel = this->rightJoint->GetVelocity(0);
		double leftWheel =this->leftJoint->GetVelocity(0);
		printf("Speed left:%d and right:%d\n",leftWheel,rightWheel);
	}
	
	private: physics::ModelPtr model;
	private: physics::JointPtr leftJoint;
	private: physics::JointPtr rightJoint;
	
	private: event::ConnectionPtr updateConnection;	
	
  };
GZ_REGISTER_MODEL_PLUGIN(EncoderPlugin)
}


