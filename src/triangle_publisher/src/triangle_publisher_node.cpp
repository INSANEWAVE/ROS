#include <rclcpp/rclcpp.hpp>// подключаем заголовочный файл для ROS2 на C++ юзаем по умолчанию
#include <geometry_msgs/msg/point.hpp>// Для кастомного типа сообщений
#include <triangle_message/msg/triangle.hpp> // наше кастомное сообщение
#include <random> //Библиотека генерации рандомных чисел

using namespace std::chrono_literals; // Используем пространство имен чтобы вместо std кракозябры прописывать обычные секунды, милисекунды и тд

class TrianglePublisher : public rclcpp::Node //Класс узла, всегда для ноды является производной класса (наследником) класса rclcpp::Node со своим функционалом
{
public: // ОткрытаЯ часть класса, тут есть конструктор
    TrianglePublisher(): //  Конструктор класса, функция вызывающаяся при создании объекта класса
      Node("triangle_publisher") // Поменяли имя ноды

    {
        publisher_triangle_random = this->create_publisher<triangle_message::msg::Triangle>("/triangle", 10); // Вот тут мы говорим чё объявленный публикатор делает
        timer = this->create_wall_timer(1000ms, std::bind(&TrianglePublisher::timer_callback, this)); // тут мы впихнули таймер и тут же его конфигурируем
    }

private:
    rclcpp::TimerBase::SharedPtr timer; // Указываем на объект таймера TimerBase
    rclcpp::Publisher<triangle_message::msg::Triangle>::SharedPtr publisher_triangle_random; // Вот тут объявляются публикаторы тут же добавляются новые


    // Инструменты для рандома (инициализируем правильно):
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<int> dis{1, 100}; // От 1 до 100


    void timer_callback(){ // Функция которая создаёт случайные вершины для треугольника
        triangle_message::msg::Triangle msg;//Обозначаем что работаем со спец типом сообщения, выдумываем двумерный треугольник

        msg.a.x = dis(gen);
        msg.a.y = dis(gen);
        msg.a.z = 0;

        msg.b.x = dis(gen);
        msg.b.y = dis(gen);
        msg.b.z = 0;

        msg.c.x = dis(gen);
        msg.c.y = dis(gen);
        msg.c.z = 0;

        RCLCPP_INFO_STREAM(this->get_logger(), "Выдумали треугольник:" << " A(" << msg.a.x << ", " << msg.a.y << ", " << msg.a.z << ") | B(" << msg.b.x << ", " << msg.b.y << ", " << msg.b.z << ") | C(" << msg.c.x << ", " << msg.c.y << ", " << msg.c.z << ")");
        publisher_triangle_random->publish(msg);
    }
};

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd
    rclcpp::spin(std::make_shared<TrianglePublisher>()); // Функция спин - цикл с указателем на наш узел
    rclcpp::shutdown(); // Как только узел заканчивает работу (сам или после команды или сбой) система выполняет шатдаун
    return 0;
}
