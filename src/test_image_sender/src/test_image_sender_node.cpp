#include <rclcpp/rclcpp.hpp>// подключаем заголовочный файл для ROS2 на C++ юзаем по умолчанию
#include <sensor_msgs/msg/image.hpp>//Подключаем строковые типы сообщений
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.hpp>
#include <opencv2/opencv.hpp>


using namespace std::chrono_literals; // Используем пространство имен чтобы вместо std кракозябры прописывать обычные секунды, милисекунды и тд
class TestImageSender : public rclcpp::Node //Класс узла, всегда для ноды является производной класса (наследником) класса rclcpp::Node со своим функционалом
{
public: // ОткрытаЯ часть класса, тут есть конструктор
    TestImageSender(): //  Конструктор класса, функция вызывающаяся при создании объекта класса
      Node("test_image_sender_node"),
        counter(0)
    {
    pub = image_transport::create_publisher(this,"/image");
    image = cv::imread("/usr/share/backgrounds/ezspain-Ubuntu_Coffee_Mug_Light.png");
    timer = this->create_wall_timer(500ms, std::bind(&TestImageSender::timer_callback, this));// тут мы впихнули таймер и тут же его конфигурируем

    }
private:
    rclcpp::TimerBase::SharedPtr timer; // Указываем на объект таймера TimerBase
    image_transport::Publisher pub;
    cv::Mat image;
    rclcpp::TimerBase::SharedPtr timer;
    unsigned int counter; // Переменная счётчика, беззнаковый инт!!

    void timer_callback(){ // Функция таймера
      counter++;
        sensor_msgs::msg::Image::SharedPtr msg =
            cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", image).toImageMsg();// brg8 - Кодировка цвета БлюГринРэд8
        pub.publish(msg);
    }
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd
    rclcpp::spin(std::make_shared<TestImageSender>()); // Функция спин - цикл с указателем на наш узел
    rclcpp::shutdown(); // Как только узел заканчивает работу (сам или после команды или сбой) система выполняет шатдаун
  return 0;
}
