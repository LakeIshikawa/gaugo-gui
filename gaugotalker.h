#ifndef GAUGOTALKER_H
#define GAUGOTALKER_H

#include <QProcess>
#include <QPlainTextEdit>

/**
 * @brief Utility class to communicate with gauGo process
 */
class GauGoTalker : public QObject
{
    Q_OBJECT

public:
    GauGoTalker(QObject* parent);

    /**
     * @brief Fire up gauGo
     * @return true-success false-failure
     */
    bool start(QPlainTextEdit* output);

    /**
     * @brief Execute a GTP command to gauGo
     * @param command The command (NOT \n terminated)
     * @return Lines in result of the command
     */
    void sendGTPCommand(QString &command);

signals:
    void GTPResult(QString &command, QStringList &resultLines);
    void GTPComment(QString &command, QString &comment);

public slots:

    /**
     * @brief Reads output from gauGo
     */
    void onReadOutput();


private:
    // The process
    QProcess gauGo;

    // Currently processing command
    QStringList commandList;

    // Textedit to write output to
    QPlainTextEdit* output;

    // Synch command that are not to be logged
    QStringList noLogCommands;
};

#endif // GAUGOTALKER_H
