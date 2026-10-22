#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "tf2/LinearMath/Transform.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

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

        prefix_ = this->get_parameter("prefix").as_string();
        world_frame_ = this->get_parameter("world_frame").as_string();
        tag_frame_ = this->get_parameter("tag_frame").as_string();
        tag_link_ = this->get_parameter("tag_link").as_string();
        base_link_ = this->get_parameter("base_link").as_string();
        publish_rate_ = this->get_parameter("publish_rate").as_double();

        tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

        floatbot_pose_pub_ = this->create_publisher<geometry_msgs::msg::PoseStamped>(
            "floatbot_pose", 
            10
        );

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(static_cast<int>(1000.0 / publish_rate_)),
            std::bind(&FloatbotPosePublisher::timer_callback, this)
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Floatbot pose publisher initialized. Publishing measured pose of robot frame: '%s' in world frame: '%s' using tag: '%s'",
            base_link_.c_str(), world_frame_.c_str(), tag_frame_.c_str()
        );
    }
    
private:
    void timer_callback(){
        try{
            // get measured tag transform in world frame
            geometry_msgs::msg::TransformStamped gm_T_wt = tf_buffer_->lookupTransform(
                world_frame_,
                tag_frame_,
                tf2::TimePointZero
            );
            
            // get known tag transform in robot frame
            geometry_msgs::msg::TransformStamped gm_T_tb = tf_buffer_->lookupTransform(
                prefix_ + tag_link_,
                prefix_ + base_link_,
                tf2::TimePointZero
            );

            // convert from geometry_msgs to tf2::transform structures
            tf2::Transform tf2_T_wt;
            tf2::Transform tf2_T_tb;
            tf2::fromMsg(gm_T_wt.transform, tf2_T_wt);
            tf2::fromMsg(gm_T_tb.transform, tf2_T_tb);

            // multiply transforms
            tf2::Transform tf2_T_wb = tf2_T_wt * tf2_T_tb;

            // publish the resulting floatbot pose message
            geometry_msgs::msg::PoseStamped floatbot_pose;
            floatbot_pose.header.stamp = this->get_clock()->now();
            floatbot_pose.header.frame_id = world_frame_;
            tf2::toMsg(tf2_T_wb, floatbot_pose.pose);
            floatbot_pose_pub_->publish(floatbot_pose);
        }
        
        // return an error if one of the needed transforms is not found
        catch(const tf2::TransformException & ex){
            RCLCPP_INFO(this->get_logger(), "Could not get transform: %s", ex.what());

            return;
        }
    }

    std::string prefix_;
    std::string world_frame_;
    std::string tag_frame_;
    std::string tag_link_;
    std::string base_link_;
    double publish_rate_;

    std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr floatbot_pose_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<FloatbotPosePublisher>());
    rclcpp::shutdown();
    return 0;
}