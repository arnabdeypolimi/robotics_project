#include <gazebo/gazebo.hh>
#include <boost/bind.hpp>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <stdio.h>
#include <gazebo/physics/Joint.hh> 


namespace gazebo
{
	
  class CoGPlugin : public ModelPlugin
  {    	
	  
    public: void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf)
	{
		this->model = _parent;
		
		this->links = this->model->GetLinks();
		
		this->updateConnection = event::Events::ConnectWorldUpdateBegin(
		std::bind(&CoGPlugin::OnUpdate, this));		
	}
	
	
	
	
	private: virtual void OnUpdate()
	{
		for (int i = 0; i < links.size(); i++){
			// CoG of a single link
			math::Vector3 cog = links[i]->GetWorldCoGPose().pos;
			// mass of the link
			double mass = links[i]->GetInertial()->GetMass();
			
			// calculation of the numerator of the CoG formula for a body composed of various elements
			numerator += mass*cog;
			
			// calculation of the total mass of the body
			total_mass += mass;
		}
		
		// formula to compute the CoG of a body composed of various elements
		model_cog = numerator/total_mass; 
		
		if (model_cog != previous_model_cog){
			printf("Model CoG: X : %f ; Y : %f; Z: %f\n", model_cog[0], model_cog[1], model_cog[2]);	
			previous_model_cog = model_cog;
		}
	}
  
	private: physics::ModelPtr model;
	private: gazebo::physics::Link_V links;
	private: math::Vector3 numerator;
	private: double total_mass = 0;
	private: math::Vector3 model_cog;
	private: math::Vector3 previous_model_cog;
	private: event::ConnectionPtr updateConnection;	
	
	
  };
  GZ_REGISTER_MODEL_PLUGIN(CoGPlugin);
}


