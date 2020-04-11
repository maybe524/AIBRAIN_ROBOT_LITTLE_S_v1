#include "ros/ros.h"
#include "std_msgs/String.h"

void chatterCallback(const std_msgs::String::ConstPtr &msg)
{
    ROS_INFO("I heard: [%s]", "1");
}

int main(int argc, char **argv)
{
    ROS_INFO("motor_user start(%s %s)...", __DATE__, __TIME__);

    ros::init(argc, argv, "motor_user");
    ros::NodeHandle n;    
    ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
    ros::spin();

    return 0;
}