/**
 * @file /include/hw1/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef hw1_QNODE_HPP_
#define hw1_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/
#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
#endif
#include <QThread>
#include "My_trutle_remote.hpp" 

/*****************************************************************************
** Class
*****************************************************************************/
class QNode : public QThread
{
  Q_OBJECT
public:
  QNode();
  ~QNode();

  // 함께 spin할 노드 등록
  void set_remote(std::shared_ptr<My_trutle_remote> remote) { remote_ = remote; }

protected:
  void run();

private:
  std::shared_ptr<rclcpp::Node> node;
  std::shared_ptr<My_trutle_remote> remote_; 

Q_SIGNALS:
  void rosShutDown();
};

#endif /* hw1_QNODE_HPP_ */