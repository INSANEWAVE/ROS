cmake_minimum_required(VERSION 3.20)
project(custom_tf_broadcaster)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(tf2 REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(test_custom_message REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)



if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


add_executable(${PROJECT_NAME}_node src/${PROJECT_NAME}_node.cpp)
target_link_libraries(${PROJECT_NAME}_node
  PRIVATE
    rclcpp::rclcpp
    tf2::tf2
    std_msgs::std_msgs
    tf2_ros::tf2_ros
    nav_msgs::nav_msgs
    geometry_msgs::geometry_msgs
    ${test_custom_message_TARGETS}
)
install(
    TARGETS
        ${PROJECT_NAME}_node
        DESTINATION lib/${PROJECT_NAME}
)

add_executable(custom_tf_listener_node src/custom_tf_listener_node.cpp)


target_link_libraries(custom_tf_listener_node
  PRIVATE
    rclcpp::rclcpp
    tf2::tf2
    tf2_ros::tf2_ros
    geometry_msgs::geometry_msgs
)

install(TARGETS
    custom_tf_listener_node
    DESTINATION lib/${PROJECT_NAME}
)


install(
    DIRECTORY launch
    DESTINATION share/${PROJECT_NAME}
)

ament_package()
