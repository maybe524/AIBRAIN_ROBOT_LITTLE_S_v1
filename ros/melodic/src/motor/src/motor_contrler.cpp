#include <sstream>
#include "ros/ros.h"
#include "std_msgs/String.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "motor_contrler");
    ros::NodeHandle n;
    int count = 0;

    ros::Publisher motor_pub = n.advertise<std_msgs::String>("chatter", 1000);
    ros::Rate loop_rate(10);
    
    while (1) {
        std_msgs::String msg;
        std::stringstream ss;
        ss << "hello world " << count;
        msg.data = ss.str();
        
        ROS_INFO("%s", msg.data.c_str());
        ros::spinOnce();
        
        loop_rate.sleep();
        count++;
    }
    
    return 0;
}