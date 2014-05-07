#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uctnode.h"
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), gaugo(NULL)
{
    noLogCommands.append("board");
    noLogCommands.append("ucttree");

    ui->setupUi(this);

    // Setup board view
    ui->boardView->setup();

    // Start gauGo
    if( !gaugo.start() ){
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
    // Board
    QString board = "board";
    ui->GTPSynch->appendPlainText(QString("<< board"));
    gaugo.sendGTPCommand(board);

    // UCT tree
    QString tree = "ucttree";
    ui->GTPSynch->appendPlainText(QString("<< ucttree"));
    gaugo.sendGTPCommand(tree);
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
        ui->GTPInput->setPlainText("");

        gtpAndSynch(text);
    }
}

void MainWindow::onReceiveGTPOutput(QString &command, QStringList &lines)
{
    // Log
    QPlainTextEdit* output;
    if( !noLogCommands.contains(command) ){
        output = ui->GTPOutput;
    } else {
        output = ui->GTPSynch;
    }

    output->appendPlainText("= ");
    foreach (const QString& line, lines) {
        output->appendPlainText(line);
    }

    // Parse result
    if( command == "board" ) parseBoard(lines);
    else if( command == "ucttree") parseUCTTree(lines);
}

void MainWindow::onReceiveGTPComment(QString &command, QString &comment)
{
    ui->GTPOutput->appendPlainText(comment);
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

void MainWindow::parseUCTTree(QStringList &lines)
{    
    QList<UCTNode> treePos;
    if( lines.at(0).isEmpty() ) {
        ui->uctNodesTable->setUCTTree(treePos);
        return;
    }

    // Parse node list
    QStringList children = lines.at(0).split(",", QString::SkipEmptyParts);
    foreach( const QString &child, children ){
        QStringList move = child.split("/", QString::SkipEmptyParts);
        QString name = move.at(0);
        QPoint pos = BoardView::nameToPoint(name);
        treePos.append(UCTNode(pos, move.at(1).toInt(), move.at(2).toInt(),
                               move.at(3).toInt(), move.at(4).toInt(),
                               move.at(5).toFloat()));
    }

    // Set the tree on view
    ui->boardView->setUCTTree(treePos);
    // Set the tree on table widget
    ui->uctNodesTable->setUCTTree(treePos);
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

void MainWindow::gtpAndSynch(QString &command)
{
    // Log
    if( !noLogCommands.contains(command) ){
        ui->GTPOutput->appendPlainText(QString("<< ").append(command));
    } else {
        ui->GTPSynch->appendPlainText(QString("<< ").append(command));
    }

    gaugo.sendGTPCommand(command);
    synchronizeBoard();
}

void MainWindow::on_action_Clear_triggered()
{
    QString command = tr("clear_board");
    gtpAndSynch(command);
}

void MainWindow::on_action9x9_triggered()
{
    QString command = tr("boardsize 9");
    gtpAndSynch(command);
}

void MainWindow::on_action13x13_triggered()
{
    QString command = tr("boardsize 13");
    gtpAndSynch(command);
}

void MainWindow::on_action19x19_triggered()
{
    QString command = tr("boardsize 19");
    gtpAndSynch(command);
}

void MainWindow::on_action_Open_SGF_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open position"), "", tr("SGF Files(*.sgf)"));
    QString command = tr("load %1").arg(fileName);
    gtpAndSynch(command);
}

void MainWindow::on_action_Save_to_SGF_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save position"), "", tr("SGF Files(*.sgf)"));
    QString command = tr("save %1").arg(fileName);
    gtpAndSynch(command);
}

void MainWindow::on_action_Undo_triggered()
{
    QString command = tr("undo");
    gtpAndSynch(command);
}

void MainWindow::on_action_Redo_triggered()
{
    QString command = tr("redo");
    gtpAndSynch(command);
}
