#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/Twist.h"
#include "math.h"

void ComPoseCallback(const geometry_msgs::Pose2D::ConstPtr& msg);
void CurPoseCallback(const turtlesim::Pose::ConstPtr& msg);
float GetErrorAng(turtlesim::Pose CurPose, geometry_msgs::Pose2D DesPose);
float GetErrorLin(turtlesim::Pose CurPose, geometry_msgs::Pose2D DesPose);


bool STOP = true;
turtlesim::Pose CurPose;
geometry_msgs::Pose2D DesPose;
geometry_msgs::Twist vel;


int main(int argc, char **argv)
{
   	ros::init(argc, argv, "line");
    	ros::NodeHandle n;
 	ros::Subscriber ComPose_sub = n.subscribe("/turtle1/PositionCommand", 10, ComPoseCallback);
	ros::Subscriber CurPose_sub = n.subscribe("/turtle1/pose", 5, CurPoseCallback);
	ros::Publisher Vel_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 100);
	ros::Rate loop_rate(10);
	float ErrorLin = 0;
    	float ErrorAng = 0;
	while(ros::ok())
	{
		ros::spinOnce();
		if(STOP==false)
		{
			ErrorLin = GetErrorLin(CurPose,DesPose);
			ErrorAng = GetErrorAng(CurPose,DesPose);

				vel.linear.x = 0.2 * ErrorLin;
                                vel.angular.z = 1 * ErrorAng;
                                Vel_pub.publish(vel);




		}
		loop_rate.sleep();
	}
}

void ComPoseCallback(const geometry_msgs::Pose2D::ConstPtr& msg)
{
    STOP = false;
    DesPose.x = msg->x;
    DesPose.y = msg->y;
    return;
}

void CurPoseCallback(const turtlesim::Pose::ConstPtr& msg)
{
    CurPose.x = msg->x;
    CurPose.y = msg->y;
    CurPose.theta = msg->theta;
    return;
}

float GetErrorAng(turtlesim::Pose CurPose, geometry_msgs::Pose2D DesPose)
{
   float Ex = DesPose.x - CurPose.x;
   float Ey = DesPose.y - CurPose.y;
   float angle = atan2f(Ey, Ex);
   float Et = angle - CurPose.theta;
   return Et;
}

float GetErrorLin(turtlesim::Pose CurPose, geometry_msgs::Pose2D DesPose)
{
  float El = sqrt((DesPose.x-CurPose.x)*(DesPose.x-CurPose.x) + (DesPose.y-CurPose.y)*(DesPose.y-CurPose.y));
  return El;
}
