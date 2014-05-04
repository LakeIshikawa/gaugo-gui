#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "gaugotalker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /**
     * @brief Synchronize board state with gauGo
     */
    void synchronizeBoard();

public slots:
    /**
     * @brief Play a move
     * @param move
     */
    void play(QString &move, QChar turn);

    /**
     * @brief Executes an arbitrary GTP command
     * @param gtpCommand The command
     */
    void onReceiveGTPInput();

    /**
     * @brief Receives the result of a GTP command
     * @param command The command
     * @param lines All lines of result string
     */
    void onReceiveGTPOutput(QString &command, QStringList &lines);

    /**
     * @brief Receives comments during a long-run GTP command
     * @param command The command
     * @param comment The received comment
     */
    void onReceiveGTPComment(QString &command, QString &comment);


private slots:
    void on_action_Clear_triggered();
    void on_action9x9_triggered();
    void on_action13x13_triggered();
    void on_action19x19_triggered();

    void on_action_Open_SGF_triggered();

    void on_action_Save_to_SGF_triggered();

private:
    Ui::MainWindow *ui;

    // Gaugo talker
    GauGoTalker gaugo;

    // GTP result parsing
    void parseBoard(QStringList &lines);

    // Update size menu checkboxes
    void updateSizeMenu(int size);
};

#endif // MAINWINDOW_H
