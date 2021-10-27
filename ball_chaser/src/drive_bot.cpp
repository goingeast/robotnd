#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"

// ROS::Publisher motor commands;
ros::Publisher motor_command_publisher;

// A callback function that executes whenever a drive_bot service is requested
bool handle_drive_request(ball_chaser::DriveToTarget::Request& req,
                          ball_chaser::DriveToTarget::Response& res)
{
  auto linearX = (float)req.linear_x;
  auto angularZ = (float)req.angular_z;

  ROS_INFO("Request received - linear_x:%f, angular_z:%f", linearX, angularZ);

  geometry_msgs::Twist motorCommand;
  motorCommand.linear.x = linearX;
  motorCommand.angular.z = angularZ;
  motor_command_publisher.publish(motorCommand);

  res.msg_feedback = "Robot move command is set - linear x: " + std::to_string(linearX)
                                     + " , angular z: " + std::to_string(angularZ);
  ROS_INFO_STREAM(res.msg_feedback);

  return true;
}

int main(int argc, char** argv)
{
    // Initialize a ROS node
    ros::init(argc, argv, "drive_bot");

    // Create a ROS NodeHandle object
    ros::NodeHandle n;

    // Inform ROS master that we will be publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
    motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);
    ROS_INFO("Ready to send commands for moving the robot");

    // Handle ROS communication events
    ros::spin();

    return 0;
}