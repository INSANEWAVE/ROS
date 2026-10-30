RQT можно запустить через Х11

QT_QPA_PLATFORM=xcb rqt

Вот так инициируем новый терминал

source install/setup.bash

Присвоение параметра командной строкой

$ ros2 run test_parameters test_parameters_node --ros-args -p number_parameter:=100.0

ros2 param list (get)

 ros2 param get /test_parameters_node string_parameter
 

ros2 param dump /test_parameters_node > test_dump.yaml

ros2 param load /test_parameters_node test_dump.yaml
