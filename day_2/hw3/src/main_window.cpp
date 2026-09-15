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

#include "hw3/main_window.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindowDesign)
{
  ui->setupUi(this);

  QIcon icon("://ros-icon.png");
  this->setWindowIcon(icon);

  qnode = new QNode();

  QObject::connect(qnode, SIGNAL(rosShutDown()), this, SLOT(close()));

  slave_ = std::make_shared<TopicSlave>();

  connect(slave_.get(), &TopicSlave::strReceived,
          this, &MainWindow::onStr);
  connect(slave_.get(), &TopicSlave::boolReceived,
          this, &MainWindow::onBool);
  connect(slave_.get(), &TopicSlave::intReceived,
          this, &MainWindow::onInt);
  connect(slave_.get(), &TopicSlave::aliveChanged,
        this, &MainWindow::onAlive);

  qnode->set_nodes(slave_);
  qnode->start();
}

void MainWindow::onAlive(int idx, bool alive)
{
    QString text = alive ? "OnSignal" : "LostSignal";
    switch (idx) {
        case 0: ui->textBrowser1->setText("String: " + text); break;
        case 1: ui->textBrowser2->setText("Bool: "   + text); break;
        case 2: ui->textBrowser3->setText("Int: "    + text); break;
    }
}

void MainWindow::on_reset1Button_clicked() { slave_->request_revive(0); }
void MainWindow::on_reset2Button_clicked() { slave_->request_revive(1); }
void MainWindow::on_reset3Button_clicked() { slave_->request_revive(2); }

void MainWindow::onStr(QString data)
{
  ui->textBrowser1->append("String: " + data);
}

void MainWindow::onBool(bool data)
{
  ui->textBrowser2->append(QString("Bool: ") + (data ? "true" : "false"));
}

void MainWindow::onInt(int data)
{
  ui->textBrowser3->append("Int: " + QString::number(data));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
  QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
  delete ui;
}
