#include <rclcpp/rclcpp.hpp> // подключаем заголовочный файл для ROS2 на C++
#include <geometry_msgs/msg/transform_stamped.hpp> // Подключаем тип сообщения для трансформаций
#include <tf2_ros/transform_listener.h> // Подключаем класс для прослушивания трансформаций
#include <tf2/exceptions.hpp> // Для обработки исключений TF2
#include <tf2_ros/buffer.hpp> // Буфер, где хранятся трансформации
#include <tf2/LinearMath/Quaternion.hpp> // Подключаем математику для работы с кватернионами (на фото .h, для Lyrical может понадобиться .hpp)
#include <cmath> // математика

using namespace std::chrono_literals; // Используем пространство имен чтобы вместо std кракозябры прописывать обычные секунды, милисекунды и тд

class TestTfListener : public rclcpp::Node // Класс узла, всегда для ноды является производной класса (наследником) класса rclcpp::Node со своим функционалом
{
public: // Открытая часть класса, тут есть конструктор
    TestTfListener(): // Конструктор класса
        Node("test_tf_listener_node"), // Инициализируем имя ноды (исправлено с broadcaster на listener)
        target_frame("sensor_link"),   // Инициализируем целевой фрейм
        source_frame("world")          // Инициализируем исходный фрейм
    {
        // Создаем буфер для хранения трансформаций и передаем ему часы узла
        tf_buffer = std::make_unique<tf2_ros::Buffer>(this->get_clock());

        // Создаем листенер, который будет слушать топик /tf и складывать данные в буфер
        tf_listener = std::make_shared<tf2_ros::TransformListener>(*tf_buffer);

        // Создаем таймер на 500 мс, который вызывает наш коллбэк
        timer = this->create_wall_timer(500ms,
                                std::bind(&TestTfListener::timer_callback,
                                        this));
    }

private:
    rclcpp::TimerBase::SharedPtr timer; // Указатель на объект таймера
    std::shared_ptr<tf2_ros::TransformListener> tf_listener; // Указатель на листенер
    std::unique_ptr<tf2_ros::Buffer> tf_buffer; // Указатель на буфер
    std::string target_frame, source_frame; // Переменные для имен фреймов

    void timer_callback() { // Функция таймера
        geometry_msgs::msg::TransformStamped t;

        try {
            // Ищем трансформацию между фреймами
            t = tf_buffer->lookupTransform(source_frame, target_frame, tf2::TimePointZero);

            // Переводим кватернион (поворот) в углы Эйлера (roll, pitch, yaw) для удобства чтения
            tf2::Quaternion q(
                t.transform.rotation.x,
                t.transform.rotation.y,
                t.transform.rotation.z,
                t.transform.rotation.w);
            tf2::Matrix3x3 m(q);
            double roll, pitch, yaw;
            m.getRPY(roll, pitch, yaw);

            // Выводим информацию в консоль
            RCLCPP_INFO_STREAM(this->get_logger(),
                               "Tranformation: " <<
                               "x: " << t.transform.translation.x <<
                               "y: " << t.transform.translation.y <<
                               "z: " << t.transform.translation.z <<
                               "roll: " << roll <<
                               "pitch: " << pitch <<
                               "yaw: " << yaw);

        } catch (const tf2::TransformException &ex) {
            // Если трансформации нет, выводим предупреждение
            RCLCPP_WARN_STREAM(this->get_logger(),
                               "There are no transform: " << ex.what());
            return;
        }
    }
}; // Конец класса

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd
    rclcpp::spin(std::make_shared<TestTfListener>()); // Функция спин - цикл с указателем на наш узел
    rclcpp::shutdown(); // Как только узел заканчивает работу, система выполняет шатдаун
    return 0;
}
