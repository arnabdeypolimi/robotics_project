#include "Encoder.h"


using namespace gazebo;
GZ_REGISTER_MODEL_PLUGIN(Encoder)

enum {RIGHT, LEFT};

/////////////////////////////////////////////////
Encoder::Encoder()
{
	
  this->wheelSpeed[LEFT] = this->wheelSpeed[RIGHT] = 0;
  
  //// The following instruction is not executed  
  publisher = nh.advertise<std_msgs::String>("~/angular_vel", 1000, 20.0);
  
}

/////////////////////////////////////////////////
void Encoder::Load(physics::ModelPtr _model,
                           sdf::ElementPtr _sdf)
{
	//////////////////
	ROS_INFO_NAMED("22222vacuum_gripper", "22222Loading gazebo_ros_vacuum_gripper");
	 
	this->model = _model;
  
	if (!_sdf->HasElement("left_joint"))
		gzerr << "Encoder plugin missing <left_joint> element\n";

	if (!_sdf->HasElement("right_joint"))
		gzerr << "Encoder plugin missing <right_joint> element\n";
	
	this->leftJoint = _model->GetJoint(
		_sdf->GetElement("left_joint")->Get<std::string>());
	this->rightJoint = _model->GetJoint(
		_sdf->GetElement("right_joint")->Get<std::string>());

	if (!this->leftJoint)
		gzerr << "Unable to find left joint["
			<< _sdf->GetElement("left_joint")->Get<std::string>() << "]\n";
	if (!this->rightJoint)
		gzerr << "Unable to find right joint["
			<< _sdf->GetElement("right_joint")->Get<std::string>() << "]\n";

	this->connection = event::Events::ConnectWorldUpdateBegin(
          std::bind(&Encoder::OnUpdate, this));
}

/////////////////////////////////////////////////
void Encoder::OnUpdate()
{  
	double leftVel = this->leftJoint->GetVelocity(0);
	double rightVel = this->rightJoint->GetVelocity(0);
	
	std_msgs::String angular_velMsg;
	std::stringstream ss;
	ss << "Angular left velocity = " ;//+ leftVel + "\nAngular right velocity" + rightVel;
	angular_velMsg.data = ss.str();
	
	publisher.publish(angular_velMsg);
  
	/*
	this->prevUpdateTime = currTime;
  
  
	common::Time currTime = this->model->GetWorld()->SimTime();
	common::Time stepTime = currTime - this->prevUpdateTime;
	
	// Print angular rotation when enough time has elapsed
	if (stepTime > this->updatePeriod)
	{
		//print angular speed
	}
	*/
}



