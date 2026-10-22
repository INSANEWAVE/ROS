#include <rclcpp/rclcpp.hpp> // подключаем заголовочный файл для ROS2 на C++ юзаем по умолчанию
#include <test_custom_srv/srv/triangle_square.hpp>



using namespace std::chrono_literals; // Используем пространство имен литералов для удобных нам интервалов времени

class TestCustomService : public rclcpp::Node
{
public: // Открытая часть класса, тут есть конструктор
    TestCustomService(): // Конструктор класса
      Node("test_custom_service_node")
    {

        service = this->create_service<test_custom_srv::srv::TriangleSquare>("triangle_square",
                                                                        std::bind(&TestCustomService::calculateSquare,                                                                                   this,
                                                                                  std::placeholders::_1, // Поле запроса
                                                                                  std::placeholders::_2) // Поле ответа
        );
    }
private:

    rclcpp::Service<test_custom_srv::srv::TriangleSquare>::SharedPtr service;// Указатель на сервис

    void calculateSquare(const std::shared_ptr<test_custom_srv::srv::TriangleSquare::Request> request, std::shared_ptr<test_custom_srv::srv::TriangleSquare::Response> response)
    {
        double a = sqrt(pow(request->pt1.x - request->pt2.x, 2) +
                            pow(request->pt1.y - request->pt2.y, 2) +
                            pow(request->pt1.z - request->pt2.z, 2));
            double b = sqrt(pow(request->pt2.x - request->pt3.x, 2) +
                            pow(request->pt2.y - request->pt3.y, 2) +
                            pow(request->pt2.z - request->pt3.z, 2));
            double c = sqrt(pow(request->pt3.x - request->pt1.x, 2) +
                            pow(request->pt3.y - request->pt1.y, 2) +
                            pow(request->pt3.z - request->pt1.z, 2));
            double p = (a + b + c) / 2;
            response->square = sqrt(p * (p - a) * (p - b) * (p - c));
            RCLCPP_INFO_STREAM(this->get_logger(), "Square: "<<response->square);
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd

    rclcpp::spin(std::make_shared<TestCustomService>()); // Функция спин - цикл с указателем на наш узел

    rclcpp::shutdown(); // Как только узел заканчивает работу (сам или после команды или сбой) система выполняет шатдаун
    return 0;
}
