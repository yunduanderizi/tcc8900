/********************************************************
*文件名：MyProcess.cpp
*描   述：处理进程交互的基类将，进程输出切成行
*作   者:   胡丰凯
*时   间:   2011-8-16
*
************************************************************/
#include "myprocess.h"
#include "trace.h"
namespace MediaControl2
{

MyProcess::MyProcess(QObject * parent)
	: QProcess(parent) {

	//QProcess::MergedChannels
	//QProcess merges the output of the running process into the standard
	//output channel (stdout). The standard error channel (stderr) will not
	//receive any data. The standard output and standard error data of the
	//running process are interleaved.
	setProcessChannelMode(QProcess::MergedChannels);

	//QProcess::readyReadStandardOutput()
	//This signal is emitted when the process has made new data available through
	//its standard output channel (stdout). It is emitted regardless of the
	//current read channel.
	connect(this, SIGNAL(readyReadStandardOutput()), SLOT(readStdout()));

	connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(finished(int, QProcess::ExitStatus)));

	connect(this, SIGNAL(error(QProcess::ProcessError)), SLOT(error(QProcess::ProcessError)));
}

MyProcess::~MyProcess() {
}

bool MyProcess::isRunning() const {
	return (state() == QProcess::Running);
}

void MyProcess::start(const QString & program, const QStringList & arguments)
{


        _remainingOutput.clear();

        //trace((char *)"\nMyProcess::start: %s \n",arguments.join(" ").toLocal8Bit().data());
     QProcess::start(program, arguments);
}

void MyProcess::readStdout() {
	genericRead(readAllStandardOutput());
}

void MyProcess::genericRead(const QByteArray & output) {
	QByteArray totalOutput = _remainingOutput + output;
	int start = 0;
	int from = 0;
        int pos = canReadLine(totalOutput, from);

	while (pos > -1) {
		//Readline
		//QString line = totalOutput.mid(start, pos - start);
		//cp1252
		//Windows-1252
		//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1252"));
		QString line = QString::fromLocal8Bit(totalOutput.mid(start, pos - start));
		from = pos + 1;


		start = from;

		//QString::trimmed() is used for removing leading and trailing whitespaces
		//Some .mp3 files contain tags with starting and ending whitespaces
		//Unfortunately MPlayer gives us leading and trailing whitespaces,
		//Winamp for example doesn't show them
		line = line.trimmed();
		if (!line.isEmpty()) {
			emit lineAvailable(line);
		}

                pos = canReadLine(totalOutput, from);
	}

	_remainingOutput = totalOutput.mid(from);
}

int MyProcess::canReadLine(const QByteArray & output, int from) {
	int pos1 = output.indexOf('\n', from);
	int pos2 = output.indexOf('\r', from);

	if ((pos1 == -1) && (pos2 == -1)) {
		return -1;
	}

	int pos = pos1;
	if ((pos1 != -1) && (pos2 != -1)) {
		if (pos1 < pos2) {
			pos = pos1;
		} else {
			pos = pos2;
		}
	} else {
		if (pos1 == -1) {
			pos = pos2;
		} else if (pos2 == -1) {
			pos = pos1;
		}
	}

	return pos;
}

void MyProcess::finished(int exitCode, QProcess::ExitStatus exitStatus) {

//	if (bytesAvailable() > 0) {
//		readStdout();
//	}

}

void MyProcess::error(QProcess::ProcessError error) {
    QString l_error= QString("QProcess Crashed");

	switch (error) {
	case QProcess::FailedToStart:
                trace((char *)"\nQProcess::FailedToStart\n");
		break;
	case QProcess::Crashed:
                emit lineAvailable(l_error);
                trace((char *)"\n QProcess::Crashed \n");

		break;
	case QProcess::Timedout:
                trace((char *)"\n QProcess::Timedout \n");

		break;
	case QProcess::WriteError:
                emit lineAvailable(l_error);
                trace((char *)"\n QProcess::WriteError \n");

		break;
	case QProcess::ReadError:
                trace((char *)"\n QProcess::ReadError \n");

		break;
	case QProcess::UnknownError:
                trace((char *)"\n QProcess::UnknownError \n");

		break;
	default:
                trace((char *)"\n Unknown QProcess::ProcessError \n");

	}
}


}

