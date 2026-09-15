/**
 * @file /include/hw1/main_window.hpp
 *
 * @brief Qt based gui for %(package)s.
 *
 * @date August 2024
 **/

#ifndef hw1_MAIN_WINDOW_H
#define hw1_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QMainWindow>
#include <memory>
#include "QIcon"
#include "qnode.hpp"
#include "ui_mainwindow.h"
#include "My_trutle_remote.hpp"
#include "QTimer"
#include <QKeyEvent>

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
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();
  QNode* qnode;

  // 내가 만든 거북이 리모컨 클래스 포인터
  // 포인터 형이 필요한 거 같아서 포인터형으로 함
  std::unique_ptr<My_trutle_remote> my_remote;

  // 타이머를 이용한 도형 그리기 함수
  void drow_triangle();
  void drow_square();
  void drow_circle();

private slots:
  // WASD 버튼 클릭 슬롯 + 키보드 입력 으로도 접근가능
  void on_wButton_clicked();
  void on_aButton_clicked();
  void on_sButton_clicked();
  void on_dButton_clicked();

  // 도형 버튼 클릭
  void on_triangleButton_clicked();
  void on_squareButton_clicked();
  void on_circleButton_clicked();

  // 굵기 버튼 클릭
  void on_thicknessButton_plus_clicked();
  void on_thicknessButton_minus_clicked();

  // 색상 버튼 클릭
  void on_colorButton_plus_clicked();
  void on_colorButton_minus_clicked();

private:
  Ui::MainWindowDesign* ui;
  void closeEvent(QCloseEvent* event);
  
  // 퍼블리시 명령 함수 (거북이 속도 용)
  void publish_func(double frontSpeed, double rotateSpeed); 

  // 키보드 입력용
  void keyPressEvent(QKeyEvent* event) override;


  QTimer* timer_; // 도형 그리기 용 타이머
  int step_ = 0;

  int color_index_ = 0; // 펜 색깔 인덱스
  int width_ = 2; // 펜 크기 인덱스
};

#endif  // hw1_MAIN_WINDOW_H
