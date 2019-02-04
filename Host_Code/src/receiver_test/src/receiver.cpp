#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Float32MultiArray.h"

void callback (const std_msgs::Float32::ConstPtr& rec_data) {
	ROS_INFO("I hear: [%f]", rec_data->data);
}

int main(int argc, char **argv) {
	std_msgs::Float32 coord;
	std_msgs::Float32MultiArray coord_array;
	coord.data = 0.0;
	
	coord_array.data.clear();

	ros::init(argc, argv, "receiver");

	ros::NodeHandle sub_nh;
	ros::NodeHandle pub_nh;

	ros::Subscriber sub = sub_nh.subscribe("ESP_Data", 1000, callback);
	ros::Publisher pub = pub_nh.advertise<std_msgs::Float32MultiArray>("Pos_Data", 1000);

	ros::Rate loop_rate(2);

	while (ros::ok() ) {
		coord.data += 1.0;

		coord_array.data.clear();

		for (int i=0; i<3; i++) {
			coord_array.data.push_back(coord.data + i);
		}

		pub.publish(coord_array);
		ros::spinOnce();

		ROS_INFO("I say: [%f]", coord.data);
		loop_rate.sleep();
	}

	return 0;
}
