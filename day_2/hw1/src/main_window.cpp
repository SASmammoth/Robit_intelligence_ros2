/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date August 2024
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/hw1/main_window.hpp"
#include "My_trutle_remote.hpp" // 내가 만든 터틀 조종 노드 추가


const static double baseFspeed = 1.0;
const static double baseRspeed = 0.5;
const static double rotScale = 0.4;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindowDesign)
{
  ui->setupUi(this);

  QIcon icon("://ros-icon.png");
  this->setWindowIcon(icon);

  
  qnode = new QNode();

  timer_ = new QTimer(this); // 도형 그리기용 타이머

  my_remote = std::make_unique<My_trutle_remote>();
  QObject::connect(qnode, SIGNAL(rosShutDown()), this, SLOT(close()));

}

void MainWindow::publish_func( double frontSpeed, double rotateSpeed)
{
  // 출력용 QString 변환 및 출력
  QString BaseFspeed_str = QString::fromStdString(std::to_string(frontSpeed));
  QString BaseRspeed_str = QString::fromStdString(std::to_string(rotateSpeed));
  ui->textBrowser->setText("전진 속도 : " + BaseFspeed_str + "   회전 속도 : " + BaseRspeed_str);
  
  // 퍼블리셔 실행
  my_remote->push_publisher(frontSpeed, rotateSpeed);
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
        case Qt::Key_W:
            on_wButton_clicked();       // 전진
            break;
        case Qt::Key_S:
            on_sButton_clicked();      // 후진
            break;
        case Qt::Key_A:
            on_aButton_clicked();   // 좌회전
            break;
        case Qt::Key_D:
            on_dButton_clicked();  // 우회전
            break;
        default:
            break;
    }
}

void MainWindow::on_wButton_clicked()
{
  // 전진
  publish_func(baseFspeed, 0);
}

void MainWindow::on_sButton_clicked()
{
  // 후진
    publish_func(-baseFspeed, 0);
}

void MainWindow::on_aButton_clicked()
{
  // 좌회전
    publish_func(baseFspeed*rotScale, baseRspeed);
}

void MainWindow::on_dButton_clicked()
{
  // 우회전
    publish_func(baseFspeed*rotScale, -baseRspeed);
}

void MainWindow::on_triangleButton_clicked()
{
  timer_->stop();
  disconnect(timer_, &QTimer::timeout, nullptr, nullptr);
  step_ = 0;
  connect(timer_, &QTimer::timeout, this, &MainWindow::drow_triangle);
  timer_->start(1000);
}

void MainWindow::drow_triangle()
{
  ui->textBrowser->setText("triangle circle\nstep : " + QString::fromStdString(std::to_string(step_)));

  switch (step_) {
        case 0: my_remote->push_publisher(2.0, 0.0); break;
        case 1: my_remote->push_publisher(0.0, 2.094); break; // x rad/s * 1s = 120deg, x = 2.094 
        case 2: my_remote->push_publisher(2.0, 0); break;
        case 3: my_remote->push_publisher(0.0, 2.094); break;
        case 4: my_remote->push_publisher(2.0, 0.0); break;
        case 5: my_remote->push_publisher(0.0, 2.094); break;
        default:
            timer_->stop();
            return;
    }
    step_++;
}

void MainWindow::on_squareButton_clicked()
{
  timer_->stop();
  disconnect(timer_, &QTimer::timeout, nullptr, nullptr);
  step_ = 0;
  connect(timer_, &QTimer::timeout, this, &MainWindow::drow_square);
  timer_->start(1000);
}

void MainWindow::drow_square()
{

  ui->textBrowser->setText("square circle\nstep : " + QString::fromStdString(std::to_string(step_)));
  switch (step_) {
        case 0: my_remote->push_publisher(2.0, 0.0); break;
        case 1: my_remote->push_publisher(0.0, 1.5795); break; // x rad/s * 1s = 90deg, x = 1.5708  인데 찌그러져서 손으로 수정함.
        case 2: my_remote->push_publisher(2.0, 0); break;
        case 3: my_remote->push_publisher(0.0, 1.5795); break;
        case 4: my_remote->push_publisher(2.0, 0.0); break;
        case 5: my_remote->push_publisher(0.0, 1.5795); break;
        case 6: my_remote->push_publisher(2.0, 0.0); break;
        case 7: my_remote->push_publisher(0.0, 1.5795); break;
        default:
            timer_->stop();
            return;
    }
    step_++;
}

void MainWindow::on_circleButton_clicked()
{
  timer_->stop();
  disconnect(timer_, &QTimer::timeout, nullptr, nullptr);
  step_ = 0;
  connect(timer_, &QTimer::timeout, this, &MainWindow::drow_circle);
  timer_->start(1000);
}

void MainWindow::drow_circle()
{
  ui->textBrowser->setText("drow circle\nstep : " + QString::fromStdString(std::to_string(step_)));
  switch (step_) {
        case 0: my_remote->push_publisher(10.0, 6.283); break; // x rad/s * 1s = 360deg x = 6.28
        default:
            timer_->stop();
            return;
    }
    step_++;
}

void MainWindow::on_thicknessButton_plus_clicked()
{
  width_++; // 두깨 추가
  my_remote->push_Service(color_index_, width_);
  ui->textBrowser->setText("pen setting\color_index_ : " + QString::fromStdString(std::to_string(color_index_)) + "width_ : " + QString::fromStdString(std::to_string(width_)));

}

void MainWindow::on_thicknessButton_minus_clicked()
{
  if(width_ > 1) width_--; // 두깨 감소
  my_remote->push_Service(color_index_, width_);
  ui->textBrowser->setText("pen setting\color_index_ : " + QString::fromStdString(std::to_string(color_index_)) + "width_ : " + QString::fromStdString(std::to_string(width_)));

}

void MainWindow::on_colorButton_plus_clicked()
{
  color_index_++; // 색 인덱스 추가
  if(color_index_ > 17) color_index_ = 0;
  my_remote->push_Service(color_index_, width_);
  ui->textBrowser->setText("pen setting\color_index_ : " + QString::fromStdString(std::to_string(color_index_)) + "width_ : " + QString::fromStdString(std::to_string(width_)));

}

void MainWindow::on_colorButton_minus_clicked()
{
  color_index_--; // 색 인덱스 감소
  if(color_index_ < 0) color_index_ = 17;
  my_remote->push_Service(color_index_, width_);
  ui->textBrowser->setText("pen setting\color_index_ : " + QString::fromStdString(std::to_string(color_index_)) + "width_ : " + QString::fromStdString(std::to_string(width_)));

}

void MainWindow::closeEvent(QCloseEvent* event)
{
  QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{

  delete ui;
}
