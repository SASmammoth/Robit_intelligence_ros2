/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date August 2024
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/hw3/qnode.hpp"

QNode::QNode()
{
  int argc = 0;
  char** argv = NULL;
  rclcpp::init(argc, argv);
  node = rclcpp::Node::make_shared("hw3");
  // this->start(); ← 삭제. set_nodes 후 main_window에서 start() 호출
}

QNode::~QNode()
{
  if (rclcpp::ok())
  {
    rclcpp::shutdown();
  }
}

void QNode::run()
{
  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(node);
  if (slave_) executor.add_node(slave_);

  rclcpp::WallRate loop_rate(20);
  while (rclcpp::ok())
  {
    executor.spin_some();
    loop_rate.sleep();
  }
  rclcpp::shutdown();
  Q_EMIT rosShutDown();
}