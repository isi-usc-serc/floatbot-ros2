#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "geometry_msgs/msg/pose_stamped.hpp"

class FloatbotPosePublisher : public rclcpp::Node {
    // Reads pose of an Apriltag in the world frame and publishes the pose of the floatbot's base link in the same frame

    public:
        FloatbotPosePublisher() : Node("floatbot_pose_publisher"){

            this->declare_parameter("prefix", "");
            this->declare_parameter("world_frame", "map"); // Frame to measure pose with respect to
            this->declare_parameter("tag_frame", "tag36h11:0"); // Tag to use for pose measurement
            this->declare_parameter("tag_link", "tag_0_link"); // Link in URDF defining the location of the above tag
            this->declare_parameter("base_link", "base_link"); // Robot link to compute the pose of
            this->declare_parameter("publish_rate", 10.0); // Hz

            std::string prefix = this->get_parameter("prefix").as_string();
            std::string world_frame = this->get_parameter("world_frame").as_string();
            std::string tag_frame = this->get_parameter("tag_frame").as_string();
            std::string tag_link = this->get_parameter("tag_link").as_string();
            std::string base_link = this->get_parameter("base_link").as_string();
            double publish_rate = this->get_parameter("publish_rate").as_double();

            std::unique_ptr<tf2_ros::Buffer> tf_buffer = std::make_unique<tf2_ros::Buffer>(this->get_clock());
            std::shared_ptr<tf2_ros::TransformListener> tf_listener = std::make_shared<tf2_ros::TransformListener>(*tf_buffer);

            rclcpp:Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr floatbot_pose_pub = this->create_publisher<geometry_msgs::msg::PoseStamped>(
                "floatbot_pose", 
                10
            );

            rclcpp::TimerBase::SharedPtr timer = this->create_wall_time(
                std::chrono::milliseconds(static_cast<int>(1000.0 / publish_rate)),
                std::bind(&FloatbotPosePublisher::timer_callback, this)
            );

            RCLCPP_INFO(
                this->get_logger(),
                "Floatbot pose publisher initialized. Publishing measured pose of robot frame: '%s' in world frame: '%s' using tag: '%s'",
                base_link.c_str(), world_frame.c_str(), tag_frame.c_str()
            );
        }
    
    private:
        void timer_callback(){
            // create pose message
            geometry_msgs::msg::PoseStamped floatbot_pose;
            floatbot_pose.header.stamp = this->get_clock()->now();
            floatbot_pose.header.frame_id = world_frame;

            try{
                // get measured tag transform in world frame
                geometry_msgs::msg::TransformStamped tag_transform = tf_buffer->lookupTransform(
                    world_frame,
                    tag_frame,
                    tf2::TimePointZero
                );
                
                // get known tag transform in robot frame
                geometry_msgs::msg::TransformStamped base_transform = tf_buffer_->lookupTransform(
                    prefix + tag_link,
                    prefix + base_link
                    tf2::TimePointZero
                );
                
                // add transforms to get robot pose in world frame
                floatbot_pose.pose.position.x = tag_transform.transform.translation.x + base_transform.transform.translation.x;
                floatbot_pose.pose.position.y = tag_transform.transform.translation.y + base_transform.transform.translation.y;
                floatbot_pose.pose.position.z = tag_transform.transform.translation.z + base_transform.transform.translation.z;

                // TO DO: add functionality in case where robot and tag frames have different orientation
                floatbot_pose.pose.orientation = tag_transform.transform.rotation;

                // publish computed pose
                floatbot_pose_pub->publish(floatbot_pose);
            }

            catch(const tf2::TransformException & ex){
                RCLCPP_INFO(this->get_logger(), "Could not get transform: %s", ex.what());

                return;
            }
        }
};

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FloatbotPosePublisher>());
    rclcpp::shutdown();
    return 0;
}