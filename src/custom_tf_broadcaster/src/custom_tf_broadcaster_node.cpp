#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2/LinearMath/Quaternion.hpp>
#include <cmath>
#include <nav_msgs/msg/path.hpp> // НОВОЕ: для траектории

using namespace std::chrono_literals;

class TestTfBroadcaster : public rclcpp::Node
{
public:
    TestTfBroadcaster():
        Node("test_tf_broadcaster_node"),
        A_fig8(3.0),                      // Амплитуда по X (размер восьмёрки)
        B_fig8(2.0),                      // Амплитуда по Y (размер восьмёрки)
        t_param(0.0)                      // Параметр времени для движения
    {
        tf_broadcaster = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

        // для траектории. Топик называется "trajectory"
        path_publisher_ = this->create_publisher<nav_msgs::msg::Path>("trajectory", 10);
        // Задаем фрейм для сообщения с траекторией один раз
        path_msg_.header.frame_id = "world";

        timer = this->create_wall_timer(100ms,
                                std::bind(&TestTfBroadcaster::timer_callback,
                                        this));
    }

private:
    rclcpp::TimerBase::SharedPtr timer;
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster;
    double A_fig8, B_fig8, t_param;

    // НОВОЕ: Объявляем публикатор и само сообщение с траекторией
    rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr path_publisher_;
    nav_msgs::msg::Path path_msg_;

    void timer_callback() {
        geometry_msgs::msg::TransformStamped t;
        t.header.stamp = this->get_clock()->now();
        t.header.frame_id = "world";
        t.child_frame_id = "base_link";

        double x = A_fig8 * sin(t_param);
        double y = B_fig8 * sin(2.0 * t_param);

        t.transform.translation.x = x;
        t.transform.translation.y = y;
        t.transform.translation.z = 0.0;

        double dx = A_fig8 * cos(t_param);
        double dy = 2.0 * B_fig8 * cos(2.0 * t_param);
        double yaw = atan2(dy, dx);

        tf2::Quaternion q;
        q.setRPY(0, 0, yaw);
        t.transform.rotation.x = q.x();
        t.transform.rotation.y = q.y();
        t.transform.rotation.z = q.z();
        t.transform.rotation.w = q.w();

        tf_broadcaster->sendTransform(t);

        // НОВОЕ: Заполняем точку для траектории теми же данными, что ушли в TF
        geometry_msgs::msg::PoseStamped pose;
        pose.header.stamp = t.header.stamp;
        pose.header.frame_id = "world";
        pose.pose.position.x = x;
        pose.pose.position.y = y;
        pose.pose.position.z = 0.0;
        pose.pose.orientation.x = q.x();
        pose.pose.orientation.y = q.y();
        pose.pose.orientation.z = q.z();
        pose.pose.orientation.w = q.w();

        // Добавляем точку в массив и публикуем
        path_msg_.header.stamp = t.header.stamp;
        path_msg_.poses.push_back(pose);
        path_publisher_->publish(path_msg_);

        t_param += 0.02;
    }
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TestTfBroadcaster>());
    rclcpp::shutdown();
    return 0;
}
