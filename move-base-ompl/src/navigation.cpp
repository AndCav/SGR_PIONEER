#include "ros/ros.h"
#include "boost/thread.hpp"
#include "boost/lexical_cast.hpp"
#include "nav_msgs/Path.h"
#include "geometry_msgs/PoseStamped.h"
#include <string>
#include<sstream>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>


using namespace std;

class NAVIGATION {
	public:
		NAVIGATION();
		void run();
		void send_goal();

	private:
		ros::NodeHandle _nh;
		move_base_msgs::MoveBaseGoal goal;
		nav_msgs::Path path;
};


NAVIGATION::NAVIGATION(){

}

void NAVIGATION::send_goal(){
	ros::Rate rate(1000);
	bool done = false;
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> ac ("/move_base", true);
	goal.target_pose.header.frame_id = "map";

	ROS_INFO_STREAM("WAITING FOR THE SERVER");
	ac.waitForServer(); //will wait for infinite time
	
	while (ros::ok()){
			

			goal.target_pose.pose.position.x = 1.5;
			goal.target_pose.pose.position.y = 0.5;
			goal.target_pose.pose.position.z = 0.0;
			goal.target_pose.pose.orientation.w = 0.1;

			ROS_INFO_STREAM("POINT TO REACH ["<< goal.target_pose.pose.position.x <<"; "<< goal.target_pose.pose.position.y <<"]");

			ac.sendGoal(goal);

			done = false;
			while ( !done ) {
			if ( ( ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED ) || (ac.getState() == actionlib::SimpleClientGoalState::ABORTED) ){
				done = true;
			}
			rate.sleep();
			}

	}


}


void NAVIGATION::run () {
	boost::thread send_goal_t( &NAVIGATION::send_goal, this );
	ros::spin();
}

int main( int argc, char** argv) {
	ros::init(argc, argv, "NAVIGATION");
	NAVIGATION client;
	client.run();
  	
}
