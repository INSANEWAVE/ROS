#include <rclcpp/rclcpp.hpp> // подключаем заголовочный файл для ROS2 на C++ юзаем по умолчанию
#include <std_srvs/srv/set_bool.hpp>
#include <string>

using namespace std::chrono_literals; // Используем пространство имен литералов для удобных нам интервалов времени

class TestBasicService : public rclcpp::Node
{
public: // Открытая часть класса, тут есть конструктор
    TestBasicService(): // Конструктор класса
      Node("test_basic_service_node"),
      flag(false) // Флаг для проверки раьоты сервиса, начальное значение в скобках - false
    {

        service = this->create_service<std_srvs::srv::SetBool>("bool_service",
                                                                        std::bind(&TestBasicService::setFlag,                                                                                   this,
                                                                                  std::placeholders::_1, // Поле запроса
                                                                                  std::placeholders::_2) // Поле ответа
        );
    }
private:
    bool flag;

    rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr service;// Указатель на сервис

    void setFlag(const std::shared_ptr<std_srvs::srv::SetBool::Request> request, std::shared_ptr<std_srvs::srv::SetBool::Response> response)
    {
        flag = request->data;


        response->success = true;


        response->message = "Флаг изменился на " + std::string(flag ? "true" : "false");


        RCLCPP_INFO_STREAM(this->get_logger(),response->message);
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv); // подключаемся к системе с передачей параметров cmd

    // ИСПРАВЛЕНО: было TestService, а класс называется TestBasicService
    rclcpp::spin(std::make_shared<TestBasicService>()); // Функция спин - цикл с указателем на наш узел

    rclcpp::shutdown(); // Как только узел заканчивает работу (сам или после команды или сбой) система выполняет шатдаун
    return 0;
}
