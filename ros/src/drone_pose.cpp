#include <tf2_ros/transform_listener.h>
#include <ros/ros.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/PoseStamped.h>

int main(int argc, char** argv){
    ros::init(argc, argv, "drone_pose");

    ros::NodeHandle node;
    tf2_ros::Buffer buffer;
    tf2_ros::TransformListener listener(buffer);

    ros::Rate rate(10.0);
    ros::Publisher drone_pose_publisher = node.advertise<geometry_msgs::PoseStamped> ("/orb_slam2_rgbd/drone_pose", 1);
    while (node.ok()){
    geometry_msgs::TransformStamped transform;
    try{
        ros::Time now = ros::Time::now();
        if(buffer.canTransform("map","slam_base_link",now,ros::Duration(0.05)))
        {transform = buffer.lookupTransform( "map","slam_base_link",  
                               ros::Time(0) );}
        
    }
    catch (tf2::TransformException ex){
      ROS_ERROR("%s",ex.what());
    }

    geometry_msgs::PoseStamped drone_position;

    drone_position.pose.position.x = transform.transform.translation.x;
    drone_position.pose.position.y = transform.transform.translation.y;
    drone_position.pose.position.z = transform.transform.translation.z;

    drone_position.pose.orientation = transform.transform.rotation;
    //tf2::fromMsg(transform.transform.translation,drone_position.pose.position);
    //tf2::fromMsg(transform.transform.rotation,drone_position.pose.orientation);
    drone_position.header = transform.header;

    drone_pose_publisher.publish(drone_position);
    rate.sleep();
  }
  return 0;

};
