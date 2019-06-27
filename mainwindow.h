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
    void showMessage();
    void onClipboardChanged();
    void clearClipboard();
    void timeoutChanged(int index);


private:
    void createUi();
    void createActions();
    void createTrayIcon();

    Ui::MainWindow *ui;

    QGroupBox *delayGroupBox;
    QLabel *delayLabel;
    QComboBox *delayComboBox;
    QSpinBox *durationSpinBox;

    QGroupBox *timerGroupBox;
    QLabel *timerLabel;
    QTimer *timer;
    int timeout;

    QComboBox *timeoutComboBox;

    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif // MAINWINDOW_H
