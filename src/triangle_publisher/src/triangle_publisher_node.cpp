#include <rclcpp/rclcpp.hpp>// подключаем заголовочный файл для ROS2 на C++ юзаем по умолчанию
#include <geometry_msgs/msg/point.hpp>// Для кастомного типа сообщений
#include <triangle_message/msg/triangle.hpp> // наше кастомное сообщение
#include <random> //Библиотека генерации рандомных чисел

using namespace std::chrono_literals; // Используем пространство имен чтобы вместо std кракозябры прописывать обычные секунды, милисекунды и тд
class TrianglePublisher : public rclcpp::Node //Класс узла, всегда для ноды является производной класса (наследником) класса rclcpp::Node со своим функционалом
{
public: // ОткрытаЯ часть класса, тут есть конструктор
    TrianglePublisher(): //  Конструктор класса, функция вызывающаяся при создании объекта класса
      Node("test_publisher"),
        counter(0)
    {
    publisher_triangle_random  = this->create_publisher<std_msgs::msg::String>("/test_topic", 10); // Вот тут мы говорим чё объявленный публикатор делает


    timer = this->create_wall_timer(1000ms, std::bind(&TrianglePublisher::timer_callback, this));// тут мы впихнули таймер и тут же его конфигурируем

    }
private:
    rclcpp::TimerBase::SharedPtr timer; // Указываем на объект таймера TimerBase
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_triangle_random;// Вот тут объявляются публикаторы тут же добавляются новые
    unsigned int counter; // Переменная счётчика, беззнаковый инт!!


    void timer_callback(){ // Функция которая создаёт

      std_msgs::msg::String msg1;std_msgs::msg::Int32 msg2;// !!ФАН ФАКТ это сообщение вызовет переполнение при работе публикатора более 11 дней подряд. :)Ы

      msg1.data = "Counter " + std::to_string(counter);
      msg2.data = static_cast<int32_t>(counter);// Вот тут мы воткнём статик кастер потому что нам следует ЯВНО указать преобразование, типа правила этикета.




      RCLCPP_INFO_STREAM(this->get_logger(),"Сгенерировали треугольник: "<<msg1.data<< " | "<<msg2.data);
    }
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd
    rclcpp::spin(std::make_shared<TrianglePublisher>()); // Функция спин - цикл с указателем на наш узел
    rclcpp::shutdown(); // Как только узел заканчивает работу (сам или после команды или сбой) система выполняет шатдаун
  return 0;
}
