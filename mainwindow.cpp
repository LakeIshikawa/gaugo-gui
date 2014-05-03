#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPlainTextEdit>

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
    QString command = QString("play %1 %2").arg(turn,move);
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
            BoardItem* item = ui->boardView->setBoard(keyVal[1].toInt());
            QObject::connect(item, SIGNAL(movePlayed(QString&,QChar)), this, SLOT(play(QString&,QChar)));
        }

        // Position
        if( keyVal[0] == "BD" ) ui->boardView->setPosition(keyVal[1]);

        // Turn
        if( keyVal[0] == "TN" ) ui->boardView->setTurn(keyVal[1].toInt());
    }
}
