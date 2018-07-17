#include <ros/ros.h>
#include <tf/transform_broadcaster.h>

/* #### INITIAL POSITIONS ####
 * 
 * FIRST BAG 
 * x: -1.43000364304
 * y: -0.370946109295
 * theta: 0.0100438956985
 * 
 * 
 * 
*/
 
int main(int argc, char** argv){
  ros::init(argc, argv, "map_tf_publisher");
  ros::NodeHandle n;

  ros::Rate r(100);

  tf::TransformBroadcaster broadcaster;

  while(n.ok()){
    broadcaster.sendTransform(
      tf::StampedTransform(
        tf::Transform(tf::Quaternion(0, 0, 0.0100438956985, 1), tf::Vector3(-1.43000364304, -0.370946109295, 0.0))
        
        , ros::Time::now(),"map", "base_link")        
    );
    r.sleep();
  }
}
