#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/Twist.h"
#include "math.h"
#include "std_msgs/Float32.h"

void CtrPoseCallback(const geometry_msgs::Pose2D::ConstPtr& msg);
void CurPoseCallback(const turtlesim::Pose::ConstPtr& msg);
void RadiusCallback(const std_msgs::Float32::ConstPtr& msg);
float GetErrorAng(turtlesim::Pose CurPose, geometry_msgs::Pose2D DesPose);
float GetErrorLin(turtlesim::Pose CurPose, geometry_msgs::Pose2D DesPose);


bool STOP = true;
turtlesim::Pose CurPose;
geometry_msgs::Pose2D CtrPose;
geometry_msgs::Pose2D DesPose;
geometry_msgs::Pose2D ExpPose;
geometry_msgs::Twist vel;
std_msgs::Float32 rad;
double ang = 0;


int main(int argc, char **argv)
{
  ros::init(argc, argv, "circle");
  ros::NodeHandle n;
  ros::Subscriber Rad_sub = n.subscribe("/turtle1/Radius" , 10 , RadiusCallback);
  ros::Subscriber ComPose_sub = n.subscribe("/turtle1/PositionCommand", 10, CtrPoseCallback);
	ros::Subscriber CurPose_sub = n.subscribe("/turtle1/pose", 5, CurPoseCallback);
	ros::Publisher Vel_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 100);
	ros::Rate loop_rate(10);
	float ErrorLin = 0;
  float ErrorAng = 0;
//  while(ErrorLin >= 0.0001)
//  {
//    ros::spinOnce();
	//	if(STOP==false)
		//{



			//vel.linear.x = 0.2 * ErrorLin;

      //vel.angular.z = 1 * ErrorAng;

      //Vel_pub.publish(vel);
		//}
		//loop_rate.sleep();
  //}

  while(ros::ok())
  {

    for(ang = 0;ang<6.3;ang = ang + 0.01)
    {

      ErrorLin = GetErrorLin(CurPose,DesPose);
      ErrorAng = GetErrorAng(CurPose,DesPose);
      ROS_INFO("%f", ErrorLin);
      //ROS_INFO("%f",ErrorLin);
      //ROS_INFO("/n %f", ErrorAng);
      while(ErrorLin >= 0.00001)
      {

      ExpPose.x = CtrPose.x + rad.data * (cos(ang));
      ExpPose.y = CtrPose.y + rad.data * (sin(ang));

      ErrorLin = GetErrorLin(CurPose,ExpPose);
      ErrorAng = GetErrorAng(CurPose,ExpPose);

      vel.linear.x = 1 * ErrorLin;
      vel.angular.z = 2 * ErrorAng;

      Vel_pub.publish(vel);
      //ROS_INFO("%f",vel.linear.x);
      loop_rate.sleep();
      }
     ros::spinOnce();
    }
  }
}

void RadiusCallback(const std_msgs::Float32::ConstPtr& msg)
{
    rad.data = msg->data;
    return;
}

void CtrPoseCallback(const geometry_msgs::Pose2D::ConstPtr& msg)
{
    STOP = false;
    CtrPose.x = msg->x;
    CtrPose.y = msg->y;
    DesPose.x = msg->x + rad.data;
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
//  ROS_INFO("%f",DesPose.y);
  //ROS_INFO("%f",CurPose.y);
  float El = sqrt((DesPose.x-CurPose.x)*(DesPose.x-CurPose.x) + (DesPose.y-CurPose.y)*(DesPose.y-CurPose.y));
  return El;
}
