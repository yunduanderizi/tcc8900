#ifndef TRACE_H_20110630
#define TRACE_H_20110630
inline void trace(char* msg,...)
{
    QByteArray  l_evn = qgetenv("DEBUG");
    if(0== qstrcmp(l_evn,"ON"))
    {
        va_list ap;
        QByteArray buf;
        va_start(ap, msg);
        buf = QString().vsprintf(msg, ap).toLocal8Bit();
        qt_message_output(QtWarningMsg, buf.constData());
        va_end(ap);
    }
}
#endif // TRACE_H_20110630
