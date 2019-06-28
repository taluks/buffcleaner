#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QClipboard>
#include <QMimeData>
#include <signal.h>
#include <QTimer>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QGroupBox;
class QLabel;
class QComboBox;
class QHBoxLayout;
class QSpinBox;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void setVisible(bool visible) override;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void onClipboardChanged();
    void clearClipboard();

private:
    void createUi();
    void createActions();
    void createTrayIcon();
    void updateTimeout();

    Ui::MainWindow *ui;

    QGroupBox *delayGroupBox;
    QLabel *delayLabel;
    QComboBox *delayComboBox;
    QSpinBox *durationSpinBox;

    QTimer *timer;
    int currentTimeUnit;
    int timeout;

    QAction *restoreAction;
    QAction *clearClipboardAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // MAINWINDOW_H
