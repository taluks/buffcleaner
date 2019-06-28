#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QCoreApplication>
#include <QCloseEvent>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QMenu>
#include <QMessageBox>
#include <QSpinBox>

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
    setFixedSize(250, 300);

    timer = new QTimer(this);

    const QClipboard *clipboard =  QApplication::clipboard();
    connect(clipboard, &QClipboard::dataChanged, this, &MainWindow::onClipboardChanged);

    currentTimeUnit = 0; // Default value seconds
    updateTimeout();
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
    if(timer->isActive())
        timer->stop();
    QClipboard *clipboard =  QApplication::clipboard();
    if(!clipboard->text().isEmpty())
        timer->singleShot(timeout, this, &MainWindow::clearClipboard);
}

void MainWindow::clearClipboard() {
    QClipboard *clipboard =  QApplication::clipboard();
    clipboard->clear();
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::DoubleClick){
        show();
    }

}

void MainWindow::createUi()
{
    delayGroupBox = new QGroupBox(tr("Set Delay"));

    delayLabel = new QLabel("Clean every:");

    durationSpinBox = new QSpinBox;
    durationSpinBox->setRange(1, 999);
    durationSpinBox->setValue(15);
    connect(durationSpinBox, qOverload<int>(&QSpinBox::valueChanged), [&](){
        updateTimeout();
    });

    delayComboBox = new QComboBox;
    delayComboBox->addItem("Seconds");
    delayComboBox->addItem("Minutes");
    delayComboBox->addItem("Hours");
    connect(delayComboBox, &QComboBox::currentIndexChanged, [&](int index){
        currentTimeUnit = index;
        updateTimeout();
    });

    QHBoxLayout *delayLayout = new QHBoxLayout;
    delayLayout->addWidget(delayLabel);
    delayLayout->addWidget(durationSpinBox);
    delayLayout->addWidget(delayComboBox);
    delayGroupBox->setLayout(delayLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(delayGroupBox);
    centralWidget()->setLayout(mainLayout);
}

void MainWindow::updateTimeout(){
    switch (currentTimeUnit) {
    case 0:
        timeout = durationSpinBox->value() * 1000;
        break;
    case 1:
        timeout = durationSpinBox->value() * 1000 * 60;
        break;
    case 2:
        timeout = durationSpinBox->value() * 1000 * 60 * 60;
        break;
    }
    qInfo("Timeout: %d   time unit: %d", timeout, currentTimeUnit);
}

void MainWindow::createActions()
{
    restoreAction = new QAction(tr("&Open"), this);
    connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

    clearClipboardAction = new QAction(tr("&Clear"), this);
    connect(clearClipboardAction, &QAction::triggered, this, &MainWindow::clearClipboard);

    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

void MainWindow::createTrayIcon()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(clearClipboardAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/assets/trash.png"));
    trayIcon->setVisible(true);
}
