/**
 * @file /include/hw3/main_window.hpp
 *
 * @brief Qt based gui for %(package)s.
 *
 * @date August 2024
 **/

#ifndef hw3_MAIN_WINDOW_H
#define hw3_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QMainWindow>
#include "QIcon"
#include "qnode.hpp"
#include "ui_mainwindow.h"
#include "hw3/topicSlave.hpp"
#include <memory>

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QNode *qnode;

private slots: // 구독자에서 오는 시그널을 실행하는 슬롯
  void onStr(QString data);
  void onBool(bool data);
  void onInt(int data);

  void onAlive(int idx, bool alive); // 상태 표시
  void on_reset1Button_clicked();   // 버튼 3개
  void on_reset2Button_clicked();
  void on_reset3Button_clicked();

private:
  Ui::MainWindowDesign *ui;
  void closeEvent(QCloseEvent *event);

  std::shared_ptr<TopicSlave> slave_;
};

#endif // hw3_MAIN_WINDOW_H
