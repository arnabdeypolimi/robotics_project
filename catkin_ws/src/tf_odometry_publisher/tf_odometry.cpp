#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <nav_msgs/Odometry.h>

ros::Publisher odom_pub;
tf::TransformBroadcaster *odom_broadcaster;
tf::TransformListener *odom_listener;

// pose of the robot FIRST BAG
double x = -1.43000364304;
double y = -0.370946109295;
double th = 0.0100438956985;//+1.570796327;//+3.141592654;

ros::Time current_time, last_time;

void velMessageReceived(geometry_msgs::TwistStamped msg){
	double vx=msg.twist.linear.x*10/27; // linear velocity in the x; other values:(5/14) (1/3)
	double vy=msg.twist.linear.y; // linear velocity in the y (our robot doesn't translate laterally so it's always zero)
	double vth=msg.twist.angular.z*1/2; // angular velocity theta; other values; (2/5 + pi) (5/9)
	
	current_time = ros::Time::now();
	
	//compute odometry in a typical way given the velocities of the robot
    double dt = (current_time - last_time).toSec();
    double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
    double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
    double delta_th = vth * dt;

    x += delta_x;
    y += delta_y;
    th += delta_th;
    
    //since all odometry is 6DOF we'll need a quaternion created from yaw
	geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);

	//first, we'll publish the transform over tf
	geometry_msgs::TransformStamped odom_trans;
	odom_trans.header.stamp = current_time;
	odom_trans.header.frame_id = "map";
	odom_trans.child_frame_id = "base_link";

	odom_trans.transform.translation.x = x;
	odom_trans.transform.translation.y = y;
	odom_trans.transform.translation.z = 0.0;
	odom_trans.transform.rotation = odom_quat;		
	
	 //send the transform
	odom_broadcaster->sendTransform(odom_trans);
    
	//next, we'll publish the odometry message over ROS
	nav_msgs::Odometry odom;
	odom.header.stamp = current_time;
	odom.header.frame_id = "map";

	//set the position
	odom.pose.pose.position.x = x;
	odom.pose.pose.position.y = y;
	odom.pose.pose.position.z = 0.0;
	odom.pose.pose.orientation = odom_quat;

	//set the velocity
	odom.child_frame_id = "base_link";
	odom.twist.twist.linear.x = vx;
	odom.twist.twist.linear.y = vy;
	odom.twist.twist.angular.z = vth;

	//publish the message
	odom_pub.publish(odom);
	
	last_time = current_time;   
}

int main(int argc, char** argv){
	ros::init(argc, argv, "odometry_publisher");  // the third parameter is the name of the node 
	
	ros::NodeHandle n;
	odom_broadcaster = new tf::TransformBroadcaster();
	odom_listener = new tf::TransformListener();

	odom_pub = n.advertise<nav_msgs::Odometry>("odometry", 1000);
  
	current_time = ros::Time::now();
	last_time = ros::Time::now();
  
	// Create a subscriber object .
	ros::Subscriber sub = n.subscribe( "/vel", 1000, &velMessageReceived) ;	
	
	ros::Rate r(200.0);
	while(n.ok()){
		ros::spinOnce();  // check for incoming messages		
		r.sleep();
	}
}
