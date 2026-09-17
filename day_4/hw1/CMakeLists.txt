cmake_minimum_required(VERSION 3.8)
project(hw1)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED) 
find_package(lifecycle_msgs REQUIRED) 
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

add_executable(lifecycle_talker src/lifecycle_talker.cpp include/hw1/lifecycle_talker.hpp)
target_compile_features(lifecycle_talker PUBLIC cxx_std_17)
target_link_libraries(lifecycle_talker PUBLIC
  rclcpp::rclcpp
  rclcpp_lifecycle::rclcpp_lifecycle
  ${lifecycle_msgs_TARGETS}
  ${std_msgs_TARGETS}
)


add_executable(fake_imu src/fake_imu.cpp include/hw1/fake_imu.hpp)
target_compile_features(fake_imu PUBLIC cxx_std_17)
target_link_libraries(fake_imu PUBLIC
  rclcpp::rclcpp
  rclcpp_lifecycle::rclcpp_lifecycle
  
  ${lifecycle_msgs_TARGETS}
  ${std_msgs_TARGETS}
  ${sensor_msgs_TARGETS} 
)

add_executable(imu_watchdog src/imu_watchdog.cpp include/hw1/imu_watchdog.hpp)
target_compile_features(imu_watchdog PUBLIC cxx_std_17)
target_link_libraries(imu_watchdog PUBLIC
  rclcpp::rclcpp
  rclcpp_lifecycle::rclcpp_lifecycle
  
  ${lifecycle_msgs_TARGETS}
  ${std_msgs_TARGETS}
  ${sensor_msgs_TARGETS} 
  ${lifecycle_msgs_TARGETS} 

)


target_include_directories(lifecycle_talker PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_include_directories(fake_imu PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

target_include_directories(imu_watchdog PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

install(TARGETS lifecycle_talker
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS fake_imu
  DESTINATION lib/${PROJECT_NAME})
install(TARGETS imu_watchdog
  DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME})

ament_package()
