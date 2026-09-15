/**
 * @file /include/hw3/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef hw3_QNODE_HPP_
#define hw3_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/
#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
#endif
#include <QThread>
#include <memory>
#include "hw3/topicSlave.hpp"


/*****************************************************************************
** Class
*****************************************************************************/
class QNode : public QThread
{
  Q_OBJECT
public:
  void set_nodes(std::shared_ptr<TopicSlave> s) { slave_ = s; }
  QNode();
  ~QNode();

protected:
  void run();

private:
  std::shared_ptr<rclcpp::Node> node; 
  std::shared_ptr<TopicSlave> slave_;

Q_SIGNALS:
  void rosShutDown();
};

#endif /* hw3_QNODE_HPP_ */
