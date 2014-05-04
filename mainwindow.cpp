#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), gaugo(NULL)
{
    ui->setupUi(this);

    // Setup board view
    ui->boardView->setup();

    // Start gauGo
    if( !gaugo.start(ui->GTPOutput) ){
        QMessageBox e;
        e.critical(this, "Error", "Could not start gauGo. You need gauGo engine.");

        exit(1);
    }
    // Register for GTO output events
    connect(&gaugo, SIGNAL(GTPResult(QString&,QStringList&)), this, SLOT(onReceiveGTPOutput(QString&,QStringList&)));
    connect(&gaugo, SIGNAL(GTPComment(QString&,QString&)), this, SLOT(onReceiveGTPComment(QString&,QString&)));

    // First synch
    synchronizeBoard();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::synchronizeBoard()
{
    QString board = "board";
    gaugo.sendGTPCommand(board);
}

void MainWindow::play(QString &move, QChar turn)
{
    QString command = tr("play %1 %2").arg(turn,move);
    gaugo.sendGTPCommand(command);
    synchronizeBoard();
}

void MainWindow::onReceiveGTPInput()
{
    QString text = ui->GTPInput->toPlainText();
    if( text.endsWith("\n") && !text.isEmpty() ){
        text.truncate(text.length()-1);
        gaugo.sendGTPCommand(text);
        ui->GTPInput->setPlainText("");

        // Re-synchronize
        synchronizeBoard();
    }
}

void MainWindow::onReceiveGTPOutput(QString &command, QStringList &lines)
{
    if( command == "board" ) parseBoard(lines);
}

void MainWindow::onReceiveGTPComment(QString &command, QString &comment)
{
}

void MainWindow::parseBoard(QStringList &lines)
{
    foreach (const QString& line, lines) {
        QStringList keyVal = line.split("=");

        // Board size
        if( keyVal[0] == "BS" ) {
            int size = keyVal[1].toInt();
            BoardItem* item = ui->boardView->setBoard(size);
            QObject::connect(item, SIGNAL(movePlayed(QString&,QChar)), this, SLOT(play(QString&,QChar)));

            // Update check
            updateSizeMenu(size);
        }

        // Position
        if( keyVal[0] == "BD" ) ui->boardView->setPosition(keyVal[1]);

        // Turn
        if( keyVal[0] == "TN" ) ui->boardView->setTurn(keyVal[1].toInt());
    }
}

void MainWindow::updateSizeMenu(int size)
{
    ui->action9x9->setChecked(false);
    ui->action13x13->setChecked(false);
    ui->action19x19->setChecked(false);

    switch( size ){
    case 9: ui->action9x9->setChecked(true); break;
    case 13:ui->action13x13->setChecked(true); break;
    case 19:ui->action19x19->setChecked(true); break;
    }
}

void MainWindow::on_action_Clear_triggered()
{
    QString command = tr("clear_board");
    gaugo.sendGTPCommand(command);
    synchronizeBoard();
}

void MainWindow::on_action9x9_triggered()
{
    QString command = tr("boardsize 9");
    gaugo.sendGTPCommand(command);
    synchronizeBoard();
}

void MainWindow::on_action13x13_triggered()
{
    QString command = tr("boardsize 13");
    gaugo.sendGTPCommand(command);
    synchronizeBoard();
}


void MainWindow::on_action19x19_triggered()
{
    QString command = tr("boardsize 19");
    gaugo.sendGTPCommand(command);
    synchronizeBoard();
}

void MainWindow::on_action_Open_SGF_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open position"), "", tr("SGF Files(*.sgf)"));
    QString command = tr("load %1").arg(fileName);
    gaugo.sendGTPCommand(command);
    synchronizeBoard();
}

void MainWindow::on_action_Save_to_SGF_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save position"), "", tr("SGF Files(*.sgf)"));
    QString command = tr("save %1").arg(fileName);
    gaugo.sendGTPCommand(command);
    synchronizeBoard();
}
