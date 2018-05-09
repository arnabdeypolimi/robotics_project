#include <gazebo/gazebo.hh>
#include <boost/bind.hpp>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <stdio.h>
#include <gazebo/physics/Joint.hh> 


namespace gazebo
{
	
  class EncoderPlugin : public ModelPlugin
  {    	
	  
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
	{
		this->model = _parent;
		
		this->leftJoint = this->model->GetJoint("joint_left_wheel");
      	this->rightJoint = this->model->GetJoint("joint_right_wheel");
		
		this->updateConnection = event::Events::ConnectWorldUpdateBegin(
		std::bind(&EncoderPlugin::OnUpdate, this));
	}
      	
     
	private: virtual void OnUpdate()
	{
		double rightWheel = this->rightJoint->GetVelocity(0);
		double leftWheel =this->leftJoint->GetVelocity(0);
		printf("Speed left:%f and right:%f\n",leftWheel,rightWheel);
	}
	
	private: physics::ModelPtr model;
	private: physics::JointPtr leftJoint;
	private: physics::JointPtr rightJoint;
	private: event::ConnectionPtr updateConnection;	
	
  };
  GZ_REGISTER_MODEL_PLUGIN(EncoderPlugin);
}


