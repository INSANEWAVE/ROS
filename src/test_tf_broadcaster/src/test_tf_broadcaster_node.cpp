#include <rclcpp/rclcpp.hpp> // подключаем заголовочный файл для ROS2 на C++
#include <geometry_msgs/msg/transform_stamped.hpp> // Подключаем тип сообщения для трансформаций
#include <tf2_ros/transform_broadcaster.h> // Подключаем класс для публикации трансформаций
#include <tf2/LinearMath/Quaternion.hpp> // Подключаем математику для работы с кватернионами для лирической росы тут будет hpp
#include <cmath> // математика

using namespace std::chrono_literals; // Используем пространство имен чтобы вместо std кракозябры прописывать обычные секунды, милисекунды и тд

class TestTfBroadcaster : public rclcpp::Node // Класс узла, всегда для ноды является производной класса (наследником) класса rclcpp::Node со своим функционалом
{
public: // Открытая часть класса, тут есть конструктор
    TestTfBroadcaster(): // Конструктор класса
        Node("test_tf_broadcaster_node"), // Инициализируем имя ноды
        r_orbit(3.0),                     // Инициализируем радиус орбиты
        theta_orbit(0.0)                  // Инициализируем начальный угол
    {
        // Создаем бродкастер и передаем ему указатель на текущий узел (*this)
        tf_broadcaster = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

        // Создаем таймер на 500 мс, который вызывает наш коллбэк
        timer = this->create_wall_timer(100ms,
                                std::bind(&TestTfBroadcaster::timer_callback,
                                        this));
    }

private:
    rclcpp::TimerBase::SharedPtr timer; // Указатель на объект таймера
    std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster; // Умный указатель на бродкастер трансформаций
    double r_orbit, theta_orbit; // Переменные для радиуса и угла орбиты

    void timer_callback() { // Функция таймера
        geometry_msgs::msg::TransformStamped t; // Создаем сообщение трансформации
        t.header.stamp = this->get_clock()->now(); // Устанавливаем текущее время
        t.header.frame_id = "world"; // Родительский фрейм
        t.child_frame_id = "base_link"; // Дочерний фрейм (то, что мы двигаем)

        // Считаем позицию по окружности (x, y) и ставим z в 0
        t.transform.translation.x = r_orbit * cos(theta_orbit);
        t.transform.translation.y = r_orbit * sin(theta_orbit);
        t.transform.translation.z = 0;

        // Задаем поворот через кватернион (разворот по оси Z + 90 градусов = M_PI_2)
        tf2::Quaternion q;
        q.setRPY(0, 0, theta_orbit + M_PI_2);
        t.transform.rotation.x = q.x();
        t.transform.rotation.y = q.y();
        t.transform.rotation.z = q.z();
        t.transform.rotation.w = q.w();

        // Отправляем трансформацию в топик /tf
        tf_broadcaster->sendTransform(t);

        // Увеличиваем угол для следующего шага (чтобы объект двигался по кругу)
        theta_orbit += 0.01;
    }
}; // Конец класса

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd
    rclcpp::spin(std::make_shared<TestTfBroadcaster>()); // Функция спин - цикл с указателем на наш узел
    rclcpp::shutdown(); // Как только узел заканчивает работу, система выполняет шатдаун
    return 0;
}
