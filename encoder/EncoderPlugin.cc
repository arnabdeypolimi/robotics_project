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
		
		this->prevUpdateTime = this->model->GetWorld()->GetSimTime();
		
		this->updateConnection = event::Events::ConnectWorldUpdateBegin(
		std::bind(&EncoderPlugin::OnUpdate, this));
	}
      	
     
	private: virtual void OnUpdate()
	{
		
		currTime = this->model->GetWorld()->GetSimTime();
		common::Time stepTime = currTime - this->prevUpdateTime;
		
		// Print angular rotation when enough time has elapsed
		if (stepTime > 1/20)
		{
			double rightWheel = this->rightJoint->GetVelocity(0);
			double leftWheel =this->leftJoint->GetVelocity(0);
			printf("Speed left:%f and right:%f\n",leftWheel,rightWheel);
			this->prevUpdateTime = currTime;
		}
		
	}
	
	private: physics::ModelPtr model;
	private: physics::JointPtr leftJoint;
	private: physics::JointPtr rightJoint;
	private: event::ConnectionPtr updateConnection;	
	
	private: common::Time prevUpdateTime;
	private: common::Time currTime;
	
  };
  GZ_REGISTER_MODEL_PLUGIN(EncoderPlugin);
}


