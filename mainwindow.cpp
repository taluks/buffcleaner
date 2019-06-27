#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QMenu>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createUi();
    createActions();
    createTrayIcon();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);

    setWindowTitle(tr("Buffcleaner"));
    setFixedSize(width(), height());

    timer = new QTimer(this);

    QClipboard * const clipboard =  QApplication::clipboard();
    connect(clipboard, SIGNAL(dataChanged()), this, SLOT(onClipboardChanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setVisible(bool visible)
{
    restoreAction->setEnabled(isMaximized() || !visible);
    QMainWindow::setVisible(visible);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}

void MainWindow::onClipboardChanged(){
    timer->singleShot(timeout, this, SLOT(clearClipboard()));
}

void MainWindow::clearClipboard() {
    if(QClipboard* c = QApplication::clipboard()) {
        c->clear();
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}

void MainWindow::showMessage()
{
    trayIcon->showMessage(tr("Title"), tr("test message"), windowIcon(), 5000);
}

void MainWindow::createUi()
{
    timerLabel = new QLabel;
    timerLabel->setText("Timeout:");

    timeoutComboBox = new QComboBox;
    timeoutComboBox->addItem("5 sec");
    timeoutComboBox->addItem("30 sec");
    timeoutComboBox->addItem("5 minutes");

    connect(timeoutComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(timeoutChanged(int)));
    timeoutComboBox->setCurrentIndex(timeoutComboBox->findData("30 sec", Qt::DisplayRole));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(timerLabel);
    layout->addWidget(timeoutComboBox);
    layout->addStretch(0);
    this->centralWidget()->setLayout(layout);
}

void MainWindow::timeoutChanged(int index){
    switch (index) {
    case 0:
        timeout = 5 * 1000;
        break;
    case 1:
        timeout = 30 * 1000;
        break;
    case 2:
        timeout = 60 * 1000 * 5;
        break;
    }
    qInfo("Timeout: %d", timeout);
}

void MainWindow::createActions()
{
    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/assets/trash.png"));
    trayIcon->setVisible(true);
}
