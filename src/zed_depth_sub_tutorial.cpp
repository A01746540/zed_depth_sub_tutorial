#include <stdio.h>
#include <string>
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include "std_msgs/String.h"

using namespace std;
string obstacle_bool= "";
string deptotal= "";
ros::Publisher bool_pub;

void depthCallback(const sensor_msgs::Image::ConstPtr& msg) {
  ros::NodeHandle n;
  bool_pub = n.advertise<std_msgs::String>("/nav/depths", 10);
  ros::Rate loop_rate(1);
  while (ros::ok())
  {
    float* depths = (float*)(&msg->data[0]);
    int pixelesh=256;
    float dep[pixelesh];
    int u[pixelesh];
    int v = 720*0.5;

    int coordenadas[pixelesh];
    for (int x=0;x<(pixelesh-60);x++){
      u[x] = ((1280 / pixelesh)*x)+150;
      coordenadas[x]= u[x] + msg->width * v;
    }
    for (int x=0;x<pixelesh-60;x++){
      dep[x] = depths[coordenadas[x]];
      if(x==0){
        deptotal="";
      }
      std::string str1 = std::to_string(dep[x]);
      deptotal = deptotal + str1 + "|";
      // Print dep (hay que obtener valores efectivos) 196 valores
      // [0.2, 3, 3, 3, 3, 3, 3, 3, 0.5, 0.5, 0.1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, ...]
    }
    ROS_INFO("%s",deptotal.c_str());
    //Publicar
    std_msgs::String msg;
    std::stringstream ss;
    ss << deptotal;
    msg.data = ss.str();
    bool_pub.publish(msg);
    ros::spinOnce();
    loop_rate.sleep();
  }
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "zed_video_subscriber");
  ros::NodeHandle n;
  ros::Subscriber subDepth = n.subscribe("/zed2/zed_node/depth/depth_registered", 10, depthCallback);
  ros::spin();
  return 0;
}