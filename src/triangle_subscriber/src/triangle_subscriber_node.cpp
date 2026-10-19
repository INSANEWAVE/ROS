#include <rclcpp/rclcpp.hpp>
#include <triangle_message/msg/triangle.hpp> // Наше кастомное сообщение
#include <cmath> // Для функции std::abs

using namespace std::chrono_literals;

class TriangleSubscriber : public rclcpp::Node
{
public:
    TriangleSubscriber() : Node("triangle_subscriber"), received_count(0)
    {
        subscription_geometry = this->create_subscription<triangle_message::msg::Triangle>(
            "/triangle", 10,
            std::bind(&TriangleSubscriber::topic_callback, this, std::placeholders::_1));
    }

private:
    rclcpp::Subscription<triangle_message::msg::Triangle>::SharedPtr subscription_geometry;
    unsigned int received_count;

    void topic_callback(const triangle_message::msg::Triangle &msg)
    {
        // Извлекаем координаты вершин для
        double x1 = msg.a.x, y1 = msg.a.y;
        double x2 = msg.b.x, y2 = msg.b.y;
        double x3 = msg.c.x, y3 = msg.c.y;

        // Вычисление площади треугольника по координатам (формула через определитель)
        double area = 0.5 * std::abs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));

        received_count++;

        // Красивый вывод в лог
        RCLCPP_INFO_STREAM(this->get_logger(),
            "Получен треугольник #" << received_count << " | " <<
            "A(" << x1 << ", " << y1 << ") " <<
            "B(" << x2 << ", " << y2 << ") " <<
            "C(" << x3 << ", " << y3 << ") " <<
            "--> Площадь: " << area);
    }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TriangleSubscriber>());
    rclcpp::shutdown();
    return 0;
}
