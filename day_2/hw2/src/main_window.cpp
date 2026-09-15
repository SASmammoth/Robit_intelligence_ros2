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

#include "../include/hw2/main_window.hpp"
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

  my_remote = std::make_shared<My_trutle_remote>(); // shared_ptr로 생성
  qnode->set_remote(my_remote); // QNode에 등록해서 함께 spin
  qnode->start();               // 등록 후 스레드 시작

    // 상태 출력 타이머 (100ms마다 textBrowser_2 갱신)
  status_timer_ = new QTimer(this);
  connect(status_timer_, &QTimer::timeout, this, &MainWindow::update_status);
  status_timer_->start(100);

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

void MainWindow::on_recodeStartButton_clicked()
{
  my_remote->recode_init();
}

void MainWindow::on_playRecodeButton_clicked()
{
  my_remote->recode_view();
}

void MainWindow::on_recodeStopButton_clicked()
{
  my_remote->recode_end();
}

void MainWindow::update_status()
{
  QString status;

  // 현재 상태 판별
  if (my_remote->get_is_recoding())
    status = "상태 : ● 기록 중\n";
  else if (my_remote->get_is_playing())
    status = "상태 : ▶ 재생 중\n";
  else
    status = "상태 : 대기\n";

  // 기록 프레임 수
  status += "기록 프레임 : " +
            QString::fromStdString(std::to_string(my_remote->get_frame_count())) + "\n";

  // 총 이동 거리 (소수점 2자리)
  status += "총 이동 거리 : " +
            QString::number(my_remote->get_total_distance(), 'f', 2) + "\n";

  // 총 소요 시간 (소수점 2자리)
  status += "소요 시간 : " +
            QString::number(my_remote->get_recode_seconds(), 'f', 2) + " s\n";

  // 재생 중이면 현재 재생 프레임 표시
  if (my_remote->get_is_playing())
    status += "재생 프레임 : " +
              QString::fromStdString(std::to_string(my_remote->get_play_frame())) + " / " +
              QString::fromStdString(std::to_string(my_remote->get_frame_count()));

  ui->textBrowser_2->setText(status);
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
        case 0: my_remote->push_publisher(10.0, 6.283); break; // x rad/s * 3s = 360deg x = 2.094
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
  ui->textBrowser->setText("pen setting\ncolor_index_ : " + QString::fromStdString(std::to_string(color_index_)) + "width_ : " + QString::fromStdString(std::to_string(width_)));

}

void MainWindow::on_thicknessButton_minus_clicked()
{
  if(width_ > 1) width_--; // 두깨 감소
  my_remote->push_Service(color_index_, width_);
  ui->textBrowser->setText("pen setting\ncolor_index_ : " + QString::fromStdString(std::to_string(color_index_)) + "width_ : " + QString::fromStdString(std::to_string(width_)));

}

void MainWindow::on_colorButton_plus_clicked()
{
  color_index_++; // 색 인덱스 추가
  if(color_index_ > 17) color_index_ = 0;
  my_remote->push_Service(color_index_, width_);
  ui->textBrowser->setText("pen setting\ncolor_index_ : " + QString::fromStdString(std::to_string(color_index_)) + "width_ : " + QString::fromStdString(std::to_string(width_)));

}

void MainWindow::on_colorButton_minus_clicked()
{
  color_index_--; // 색 인덱스 감소
  if(color_index_ < 0) color_index_ = 17;
  my_remote->push_Service(color_index_, width_);
  ui->textBrowser->setText("pen setting\ncolor_index_ : " + QString::fromStdString(std::to_string(color_index_)) + "width_ : " + QString::fromStdString(std::to_string(width_)));

}

void MainWindow::closeEvent(QCloseEvent* event)
{
  QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{

  delete ui;
}
