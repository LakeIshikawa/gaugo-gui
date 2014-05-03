#include "gaugotalker.h"
#include <QtDebug>

GauGoTalker::GauGoTalker(QObject *parent) : QObject(parent)
{
    noLogCommands.append("board");
}

bool GauGoTalker::start(QPlainTextEdit* output)
{
    this->output = output;

    // Register for read signals
    connect(&gauGo, SIGNAL(readyRead()), this, SLOT(onReadOutput()));

    // Start up gauGo
    gauGo.start("gauGo", QStringList());
    bool res = gauGo.waitForStarted();

    return res;
}

void GauGoTalker::sendGTPCommand(QString &command)
{
    this->commandList.append(command);

    // Log
    if( !noLogCommands.contains(command) ){
        output->appendPlainText(QString("<< ").append(command));
    }

    gauGo.write(qPrintable(command.append("\n")));
}

void GauGoTalker::onReadOutput()
{
    // Read all available things
    QString readData = QString(gauGo.readAll());

    // Parse
    QStringList lines = readData.split("\n");
    lines.removeLast();

    QMutableListIterator<QString> i(lines);
    while(i.hasNext()){
        QString line = i.next();

        // Log
        if( !noLogCommands.contains(commandList.first()) ){
            output->appendPlainText(line);
        }

        // Comments notified one by one
        if( line.startsWith('#') ){
            emit GTPComment(commandList.first(), line);
        }

        // Command result
        else if( line.startsWith("=") | line.startsWith("?") ){
            QStringList result;
            result.append(line.remove(0,2));

            // Append all lines to \n\n and go to next command
            while(i.hasNext()){
                QString resLine = i.next();
                if( resLine.isEmpty() ){
                    // \n\n found! end of command result
                    emit GTPResult(commandList.first(), result);
                    commandList.removeFirst();
                    break;
                } else {
                    result.append(resLine);
                }
            }
        }
    }
}
