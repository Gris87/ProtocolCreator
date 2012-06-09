#include "dumpcrashes.h"

QtMsgHandler secondHandler=0;
QStringList logs;
int logsMaximum=INT_MAX;

#ifdef Q_OS_WIN
bool bSymInitialized = false;
HMODULE hModule_Imagehlp = NULL;

HMODULE hModule_StackTracer;

typedef QString (*PFNGETSTACKTRACE)(HANDLE);
PFNGETSTACKTRACE pfnGetStackTrace = NULL;

typedef bool (*PFNGETBACKTRACE)(PCONTEXT, QString &);
PFNGETBACKTRACE pfnGetBackTrace = NULL;

HMODULE loadStackTracerLibrary()
{
    hModule_StackTracer = LoadLibraryA("StackTracer.dll");
    return hModule_StackTracer;
}

DWORD GetModuleBase(DWORD dwAddress)
{
    MEMORY_BASIC_INFORMATION Buffer;

    return VirtualQuery((LPCVOID) dwAddress, &Buffer, sizeof(Buffer)) ? (DWORD) Buffer.AllocationBase : 0;
}

QString addressToHex(DWORD address, int minLength=8)
{
    QString res=QString::number(address, 16).toUpper();

    while (res.length()<minLength)
    {
        res.insert(0, "0");
    }

    return "0x"+res;
}

bool dumpHeader(QString &aReport)
{
    aReport.append("\r\n");
    aReport.append("Program: "+QDir::toNativeSeparators(QCoreApplication::applicationFilePath())+"\r\n");
    aReport.append("Command line: ");

    QStringList arguments=QCoreApplication::arguments();

    for (int i=1; i<arguments.length(); i++)
    {
        if (i<1)
        {
            aReport.append(", ");
        }

        aReport.append("\""+arguments.at(i)+"\"");
    }

    aReport.append("\r\n");



    QString version=QCoreApplication::applicationVersion();

    if (version=="")
    {
        version="UNKNOWN";
    }

    aReport.append("Version: "+version+"\r\n");



    QString organizationName=QCoreApplication::organizationName();

    if (organizationName=="")
    {
        organizationName="UNKNOWN";
    }

    aReport.append("Organization name: "+organizationName+"\r\n");



    QString organizationDomain=QCoreApplication::organizationDomain();

    if (organizationDomain=="")
    {
        organizationDomain="UNKNOWN";
    }

    aReport.append("Organization domain: "+organizationDomain+"\r\n");

    aReport.append("Compiled on: "+QString::fromAscii(__DATE__)+" "+QString::fromAscii(__TIME__)+"\r\n");
    aReport.append("Compiled by: ");

#if defined(__GCC__)
    aReport.append("GCC "+QString::fromAscii(__VERSION__));
#elif defined(__GNUC__)
    aReport.append("GNUC "+QString::fromAscii(__VERSION__));
#elif defined(__GLIBC__)
    aReport.append("GLIBC "+QString::fromAscii(__VERSION__));
#elif defined(__GNUC__)
    aReport.append("UNKNOWN");
#endif

    aReport.append("\r\n");

    aReport.append("Compiled mode: ");

#ifdef QT_DEBUG
    aReport.append("Debug");
#else
    aReport.append("Release");
#endif

    aReport.append("\r\n");

    return true;
}

bool dumpCrashInfo(PEXCEPTION_RECORD pExceptionRecord, QString &aReport)
{
    aReport.append("\r\n");
    aReport.append("Application caused ");

    switch(pExceptionRecord->ExceptionCode)
    {
        case EXCEPTION_ACCESS_VIOLATION:         aReport.append("an Access Violation");        break;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    aReport.append("an Array Bound Exceeded");    break;
        case EXCEPTION_BREAKPOINT:               aReport.append("a Breakpoint");               break;
        case EXCEPTION_DATATYPE_MISALIGNMENT:    aReport.append("a Datatype Misalignment");    break;
        case EXCEPTION_FLT_DENORMAL_OPERAND:     aReport.append("a Float Denormal Operand");   break;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:       aReport.append("a Float Divide By Zero");     break;
        case EXCEPTION_FLT_INEXACT_RESULT:       aReport.append("a Float Inexact Result");     break;
        case EXCEPTION_FLT_INVALID_OPERATION:    aReport.append("a Float Invalid Operation");  break;
        case EXCEPTION_FLT_OVERFLOW:             aReport.append("a Float Overflow");           break;
        case EXCEPTION_FLT_STACK_CHECK:          aReport.append("a Float Stack Check");        break;
        case EXCEPTION_FLT_UNDERFLOW:            aReport.append("a Float Underflow");          break;
        case EXCEPTION_GUARD_PAGE:               aReport.append("a Guard Page");               break;
        case EXCEPTION_ILLEGAL_INSTRUCTION:      aReport.append("an Illegal Instruction");     break;
        case EXCEPTION_IN_PAGE_ERROR:            aReport.append("an In Page Error");           break;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:       aReport.append("an Integer Divide By Zero");  break;
        case EXCEPTION_INT_OVERFLOW:             aReport.append("an Integer Overflow");        break;
        case EXCEPTION_INVALID_DISPOSITION:      aReport.append("an Invalid Disposition");     break;
        case EXCEPTION_INVALID_HANDLE:           aReport.append("an Invalid Handle");          break;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION: aReport.append("a Noncontinuable Exception"); break;
        case EXCEPTION_PRIV_INSTRUCTION:         aReport.append("a Privileged Instruction");   break;
        case EXCEPTION_SINGLE_STEP:              aReport.append("a Single Step");              break;
        case EXCEPTION_STACK_OVERFLOW:           aReport.append("a Stack Overflow");           break;
        case DBG_CONTROL_C:                      aReport.append("a Control+C");                break;
        case DBG_CONTROL_BREAK:                  aReport.append("a Control+Break");            break;
        case DBG_TERMINATE_THREAD:               aReport.append("a Terminate Thread");         break;
        case DBG_TERMINATE_PROCESS:              aReport.append("a Terminate Process");        break;
        case RPC_S_UNKNOWN_IF:                   aReport.append("an Unknown Interface");       break;
        case RPC_S_SERVER_UNAVAILABLE:           aReport.append("a Server Unavailable");       break;
        default:                                 aReport.append("an Unknown exception code: "+QString::number(pExceptionRecord->ExceptionCode)); break;
    }

    // Now print information about where the fault occured
    aReport.append(" at location "+addressToHex((DWORD)pExceptionRecord->ExceptionAddress));

    HMODULE hModule=(HMODULE)GetModuleBase((DWORD) pExceptionRecord->ExceptionAddress);

    if (hModule)
    {
        TCHAR szModule[MAX_PATH];

        if (GetModuleFileName(hModule, szModule, sizeof(szModule)))
        {
            aReport.append(" in module "+QString::fromWCharArray(szModule));
        }
    }

    aReport.append("\r\n");

    // If the exception was an access violation, print out some additional information, to the error log and the debugger.
    if (
        pExceptionRecord->ExceptionCode==EXCEPTION_ACCESS_VIOLATION
        &&
        pExceptionRecord->NumberParameters>1
       )
    {
        if (pExceptionRecord->ExceptionInformation[0])
        {
            aReport.append("Writing to");
        }
        else
        {
            aReport.append("Reading from");
        }

        aReport.append(" location "+addressToHex(pExceptionRecord->ExceptionInformation[1])+" \r\n");
    }

    return true;
}

bool dumpRegisters(PCONTEXT pContext, QString &aReport)
{
    aReport.append("\r\n");
    aReport.append("Registers:\r\n");

#if defined(_X86_)
    if (pContext->ContextFlags & CONTEXT_INTEGER)
    {
        aReport.append("eax="+addressToHex(pContext->Eax));
        aReport.append(" ebx="+addressToHex(pContext->Ebx));
        aReport.append(" ecx="+addressToHex(pContext->Ecx));
        aReport.append(" edx="+addressToHex(pContext->Edx));
        aReport.append(" esi="+addressToHex(pContext->Esi));
        aReport.append(" edi="+addressToHex(pContext->Edi));
        aReport.append("\r\n");
    }

    if (pContext->ContextFlags & CONTEXT_CONTROL)
    {
        aReport.append("eip="+addressToHex(pContext->Eip));
        aReport.append(" esp="+addressToHex(pContext->Esp));
        aReport.append(" ebp="+addressToHex(pContext->Ebp));

        QString iopl=addressToHex((pContext->EFlags >> 12) & 3, 0);

        while (iopl.length()<7)
        {
            iopl.append(" ");
        }

        aReport.append(" iopl="+iopl);                                   //  IOPL level value
        aReport.append(pContext->EFlags & 0x00100000 ? " vip" : "    "); //  VIP (virtual interrupt pending)
        aReport.append(pContext->EFlags & 0x00080000 ? " vif" : "    "); //  VIF (virtual interrupt flag)
        aReport.append(pContext->EFlags & 0x00000800 ? " ov"  : " nv");  //  VIF (virtual interrupt flag)
        aReport.append(pContext->EFlags & 0x00000400 ? " dn"  : " up");  //  OF (overflow flag)
        aReport.append(pContext->EFlags & 0x00000200 ? " ei"  : " di");  //  IF (interrupt enable flag)
        aReport.append(pContext->EFlags & 0x00000080 ? " ng"  : " pl");  //  SF (sign flag)
        aReport.append(pContext->EFlags & 0x00000040 ? " zr"  : " nz");  //  ZF (zero flag)
        aReport.append(pContext->EFlags & 0x00000010 ? " ac"  : " na");  //  AF (aux carry flag)
        aReport.append(pContext->EFlags & 0x00000004 ? " po"  : " pe");  //  PF (parity flag)
        aReport.append(pContext->EFlags & 0x00000001 ? " cy"  : " nc");  //  CF (carry flag)
        aReport.append("\r\n");
    }

    if (pContext->ContextFlags & CONTEXT_SEGMENTS)
    {
        aReport.append("cs="+addressToHex(pContext->SegCs, 4));
        aReport.append(" ss="+addressToHex(pContext->SegSs, 4));
        aReport.append(" ds="+addressToHex(pContext->SegDs, 4));
        aReport.append(" es="+addressToHex(pContext->SegEs, 4));
        aReport.append(" fs="+addressToHex(pContext->SegFs, 4));
        aReport.append(" gs="+addressToHex(pContext->SegGs, 4));

        if (pContext->ContextFlags & CONTEXT_CONTROL)
        {
            aReport.append("                efl="+addressToHex(pContext->EFlags));
        }

        aReport.append("\r\n");
    }
    else
    {
        if (pContext->ContextFlags & CONTEXT_CONTROL)
        {
            aReport.append("                                                                           efl="+addressToHex(pContext->EFlags)+"\r\n");
        }
    }
#else
    aReport.append("Impossible to get information about registers\r\n");
#endif

    return true;
}

bool dumpLogs(QString &aReport)
{
    aReport.append("\r\n");
    aReport.append("Logs:\r\n");

    if (logs.length()>0)
    {
        for (int i=0; i<logs.length(); i++)
        {
            aReport.append(logs.at(i));
            aReport.append("\r\n");
        }
    }
    else
    {
        aReport.append("Logs not found. Try to use installMsgHandler() instead of qInstallMsgHandler()?");
    }

    return true;
}

LONG WINAPI exceptionFilter(struct _EXCEPTION_POINTERS *pExceptionInfo)
{
    qCritical("===== APPLICATION CRASHES =====");

    QDateTime aErrorTime=QDateTime::currentDateTime();
    QString aReport="";

    // Start out with a banner
    aReport.append("--------------------------------------\r\n");
    aReport.append("Error occured on "+aErrorTime.toString("dd-MM-yyyy hh:mm:ss")+"\r\n");
    aReport.append("--------------------------------------\r\n");

    // Dump a generic info header
    if (!dumpHeader(aReport))
    {
        qCritical()<<"Impossible to create report. Sorry...";
        return EXCEPTION_CONTINUE_SEARCH;
    }

    // Dump information about the type of fault
    if (!dumpCrashInfo(pExceptionInfo->ExceptionRecord, aReport))
    {
        qCritical()<<"Impossible to create report. Sorry...";
        return EXCEPTION_CONTINUE_SEARCH;
    }

    // Dump information about registers
    if (!dumpRegisters(pExceptionInfo->ContextRecord, aReport))
    {
        qCritical()<<"Impossible to create report. Sorry...";
        return EXCEPTION_CONTINUE_SEARCH;
    }

    // Dump backtrace
    aReport.append("\r\n");
    aReport.append("Call stack:\r\n");

    if (hModule_StackTracer || loadStackTracerLibrary())
    {
        if (pfnGetBackTrace || (pfnGetBackTrace = (PFNGETBACKTRACE) GetProcAddress(hModule_StackTracer, "getBackTrace")))
        {
            QString aStackTrace;

            if (pfnGetBackTrace(pExceptionInfo->ContextRecord, aStackTrace))
            {
                aReport.append(aStackTrace);
            }
            else
            {
                aReport.append("Impossible to open library \"StackTracer\"\r\n");
            }
        }
        else
        {
            aReport.append("Incorrect version of library \"StackTracer\"\r\n");
        }
    }
    else
    {
        aReport.append("Impossible to open library \"StackTracer\"\r\n");
    }

    QString aLogsString;

    // Dump logs
    dumpLogs(aLogsString);


    qCritical()<<aReport;


    aReport.append(aLogsString);

    ExceptionObject reportObject(aReport);
    QTimer::singleShot(0, &reportObject, SLOT(showDialog()));

    QEventLoop loop;
    loop.exec();

    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

void unexpectedException()
{
    int *a=0;
    *a=0;
}

void allowDumpCrashes()
{
    installMsgHandler();

#ifdef Q_OS_WIN
    SetUnhandledExceptionFilter(exceptionFilter);
#endif

    std::set_terminate(unexpectedException);
}

void logMessageHandler(QtMsgType type, const char *buf)
{
    if (logs.length()>0)
    {
        if (secondHandler)
        {
            secondHandler(type, buf);
        }
        else
        {
            // From qt_message_output in QT 4.8
#if defined(Q_CC_MWERKS) && defined(Q_OS_MACX)
            mac_default_handler(buf);
#elif defined(Q_OS_WINCE)
            QString fstr = QString::fromLatin1(buf);
            fstr += QLatin1Char('\n');
            OutputDebugString(reinterpret_cast<const wchar_t *> (fstr.utf16()));
#elif defined(Q_OS_SYMBIAN)
            // RDebug::Print has a cap of 256 characters so break it up
            char format[] = "[Qt Message] %S";
            const int maxBlockSize = 256 - sizeof(format);
            const TPtrC8 ptr(reinterpret_cast<const TUint8*>(buf));
            for (int i = 0; i < ptr.Length(); i += maxBlockSize) {
                TPtrC8 part(ptr.Mid(i, qMin(maxBlockSize, ptr.Length()-i)));
                RDebug::Printf(format, &part);
            }
#else
            fprintf(stderr, "%s\n", buf);
            fflush(stderr);
#endif
        }
    }

    QString aRow=QDateTime::currentDateTime().toString("MM-dd hh:mm:ss.zzz");

    aRow.append(": ");

    switch (type)
    {
        case QtDebugMsg:
            aRow.append("DEBUG:    ");
        break;
        case QtWarningMsg:
            aRow.append("WARNING:  ");
        break;
        case QtCriticalMsg:
            aRow.append("CRITICAL: ");
        break;
        case QtFatalMsg:
            aRow.append("FATAL:    ");
        break;
        default:
            aRow.append("UNKNOWN:  ");
        break;
    }

    QStringList logBuffer=QString::fromUtf8(buf).split("\n");

    for (int i=0; i<logBuffer.length(); i++)
    {
        QString aOneLine=logBuffer.at(i);

        aOneLine.remove("\r");

        logs.append(aRow+aOneLine);
    }

    while (logs.length()>logsMaximum)
    {
        logs.removeFirst();
    }

    if (type==QtFatalMsg)
    {
        int *a=0;
        *a=0;
    }
}

QtMsgHandler installMsgHandler(QtMsgHandler h, int maxLogCount)
{
    if (logs.length()==0)
    {
        logMessageHandler(QtDebugMsg, "Init dump crashes");
    }



    QtMsgHandler oldSecondHandler = secondHandler;
    QtMsgHandler old = qInstallMsgHandler(logMessageHandler);

    if (h==0 && old && old!=logMessageHandler)
    {
        secondHandler=old;
    }
    else
    {
        secondHandler=h;
    }



    if (maxLogCount>0)
    {
        logsMaximum=maxLogCount;

        while (logs.length()>logsMaximum)
        {
            logs.removeFirst();
        }
    }

    if (old==0)
    {
        return 0;
    }

    if (old!=logMessageHandler)
    {
        return old;
    }
    else
    {
        return oldSecondHandler;
    }
}

QString getStackTrace()
{
    QString res="Call stack:\r\n";

    if (hModule_StackTracer || loadStackTracerLibrary())
    {
        if (pfnGetStackTrace || (pfnGetStackTrace = (PFNGETSTACKTRACE) GetProcAddress(hModule_StackTracer, "getStackTrace")))
        {
            res.append(pfnGetStackTrace(0));
        }
        else
        {
            res.append("Incorrect version of library \"StackTracer\"");
        }
    }
    else
    {
        res.append("Impossible to open library \"StackTracer\"");
    }

    return res;
}

void printStackTrace()
{
    qDebug()<<getStackTrace();
}
