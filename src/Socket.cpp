/*
 * Socket.cpp
 *
 *  Created on: 07 янв. 2015 г.
 *      Author: tarasov
 */

#include "Socket.h"

#include <unistd.h>
#include <cstring>
#include <iostream>

using namespace std;

#define INVALID_SOCKET  -1
#define ENOERR 0

enum
{
    MAX_ADDRESS_LENGTH = sizeof(struct sockaddr_in)
};

SocketAddr::SocketAddr(Port port)
{
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY;
    m_addr.sin_port = htons(port);
}

Socket::Socket() :
        m_sockfd(INVALID_SOCKET)
{
}

Socket::Socket(SocketFD sockfd) :
        m_sockfd(sockfd)
{
}

Socket::~Socket()
{
    close();
}

void Socket::init(const SocketAddr & address)
{
    int proto(0);
    m_sockfd = ::socket(address.m_addr.sin_family, SOCK_STREAM, proto);
    if (m_sockfd == INVALID_SOCKET)
    {
        doThrow(errno);
    }

}

void Socket::close()
{
    if (m_sockfd != INVALID_SOCKET)
    {
        ::close(m_sockfd);
        m_sockfd = INVALID_SOCKET;
    }
}

void Socket::bind(const SocketAddr& address)
{
    if (m_sockfd == INVALID_SOCKET)
    {
        init(address.m_addr.sin_family);
    }

    SocketLen addrLen = sizeof(address.m_addr);

    int rc = ::bind(m_sockfd, reinterpret_cast<const struct sockaddr*>(&address.m_addr), addrLen);

    if (rc != 0)
    {
        doThrow(errno);
    }
}

void Socket::listen(int backlog)
{
    if (m_sockfd == INVALID_SOCKET)
    {
        throw SocketException("Invalid socket");
    }

    int rc = ::listen(m_sockfd, backlog);

    if (rc != 0)
    {
        doThrow(errno);
    }
}

Socket* Socket::acceptConnection()
{
    if (m_sockfd == INVALID_SOCKET)
    {
        throw SocketException("Invalid socket");
    }

    char buffer[MAX_ADDRESS_LENGTH];
    struct sockaddr* pSA = reinterpret_cast<struct sockaddr*>(buffer);
    SocketLen saLen = sizeof(buffer);
    SocketFD sd;
    do
    {
        sd = ::accept(m_sockfd, pSA, &saLen);
    } while (sd == INVALID_SOCKET && errno == EINTR);
    if (sd != INVALID_SOCKET)
    {
        return new Socket(sd);
    }
    doThrow(errno);
    return 0;
}

void Socket::setOption(int level, int option, int value)
{
    setRawOption(level, option, &value, sizeof(value));
}

void Socket::setRawOption(int level, int option, const void* value, SocketLen length)
{
    if (m_sockfd == INVALID_SOCKET)
    {
        throw SocketException("Invalid socket");
    }
    int rc = ::setsockopt(m_sockfd, level, option, reinterpret_cast<const char*>(value), length);
    if (rc == -1)
    {
        doThrow(errno);
    }
}

int Socket::sendBytes(const void* buffer, int length, int flags)
{
    int rc;
    do
    {
        if (m_sockfd == INVALID_SOCKET)
        {
            throw SocketException("Invalid socket");
        }
        rc = ::send(m_sockfd, reinterpret_cast<const char*>(buffer), length, flags);
    } while (rc < 0 && errno == EINTR);

    if (rc < 0)
    {
        doThrow(errno);
    }
    return rc;
}

int Socket::receiveBytes(void* buffer, int length, int flags)
{
    int rc;
    do
    {
        if (m_sockfd == INVALID_SOCKET)
        {
            throw SocketException("Invalid socket");
        }

        rc = ::recv(m_sockfd, reinterpret_cast<char*>(buffer), length, flags);
    } while (rc < 0 && errno == EINTR);

    if (rc < 0)
    {
        doThrow(errno);
    }
    return rc;
}



map<int, std::string> Socket::createSocketErrorMap()
{
    map<int, std::string> m;

    /*copy from errno-base.h*/
    m[EPERM] = "Operation not permitted";
    m[ENOENT] = "No such file or directory";
    m[ESRCH] = "No such process";
    m[EINTR] = "Interrupted system call";
    m[EIO] = "I/O error";
    m[ENXIO] = "No such device or address";
    m[E2BIG] = "Argument list too long";
    m[ENOEXEC] = "Exec format error";
    m[EBADF] = "Bad file number";
    m[ECHILD] = "No child processes";
    m[EAGAIN] = "Try again";
    m[ENOMEM] = "Out of memory";
    m[EACCES] = "Permission denied";
    m[EFAULT] = "Bad address";
    m[ENOTBLK] = "Block device required";
    m[EBUSY] = "Device or resource busy";
    m[EEXIST] = "File exists";
    m[EXDEV] = "Cross-device link";
    m[ENODEV] = "No such device";
    m[ENOTDIR] = "Not a directory";
    m[EISDIR] = "Is a directory";
    m[EINVAL] = "Invalid argument";
    m[ENFILE] = "File table overflow";
    m[EMFILE] = "Too many open files";
    m[ENOTTY] = "Not a typewriter";
    m[ETXTBSY] = "Text file busy";
    m[EFBIG] = "File too large";
    m[ENOSPC] = "No space left on device";
    m[ESPIPE] = "Illegal seek";
    m[EROFS] = "Read-only file system";
    m[EMLINK] = "Too many links";
    m[EPIPE] = "Broken pipe";
    m[EDOM] = "Math argument out of domain of func";
    m[ERANGE] = "Math result not representable";

    /*copy from  errno.h*/
    m[EDEADLK] = "Resource deadlock would occur";
    m[ENAMETOOLONG] = "File name too long";
    m[ENOLCK] = "No record locks available";
    m[ENOSYS] = "Function not implemented";
    m[ENOTEMPTY] = "Directory not empty";
    m[ELOOP] = "Too many symbolic links encountered";
    m[EWOULDBLOCK] = "Operation would block";
    m[ENOMSG] = "No message of desired type";
    m[EIDRM] = "Identifier removed";
    m[ECHRNG] = "Channel number out of range";
    m[EL2NSYNC] = "Level 2 not synchronized";
    m[EL3HLT] = "Level 3 halted";
    m[EL3RST] = "Level 3 reset";
    m[ELNRNG] = "Link number out of range";
    m[EUNATCH] = "Protocol driver not attached";
    m[ENOCSI] = "No CSI structure available";
    m[EL2HLT] = "Level 2 halted";
    m[EBADE] = "Invalid exchange";
    m[EBADR] = "Invalid request descriptor";
    m[EXFULL] = "Exchange full";
    m[ENOANO] = "No anode";
    m[EBADRQC] = "Invalid request code";
    m[EBADSLT] = "Invalid slot";
    m[EDEADLOCK] = "Resource deadlock would occur";
    m[EBFONT] = "Bad font file format";
    m[ENOSTR] = "Device not a stream";
    m[ENODATA] = "No data available";
    m[ETIME] = "Timer expired";
    m[ENOSR] = "Out of streams resources";
    m[ENONET] = "Machine is not on the network";
    m[ENOPKG] = "Package not installed";
    m[EREMOTE] = "Object is remote";
    m[ENOLINK] = "Link has been severed";
    m[EADV] = "Advertise error";
    m[ESRMNT] = "Srmount error";
    m[ECOMM] = "Communication error on send";
    m[EPROTO] = "Protocol error";
    m[EMULTIHOP] = "Multihop attempted";
    m[EDOTDOT] = "RFS specific error";
    m[EBADMSG] = "Not a data message";
    m[EOVERFLOW] = "Value too large for defined data type";
    m[ENOTUNIQ] = "Name not unique on network";
    m[EBADFD] = "File descriptor in bad state";
    m[EREMCHG] = "Remote address changed";
    m[ELIBACC] = "Can not access a needed shared library";
    m[ELIBBAD] = "Accessing a corrupted shared library";
    m[ELIBSCN] = ".lib section in a.out corrupted";
    m[ELIBMAX] = "Attempting to link in too many shared libraries";
    m[ELIBEXEC] = "Cannot exec a shared library directly";
    m[EILSEQ] = "Illegal byte sequence";
    m[ERESTART] = "Interrupted system call should be restarted";
    m[ESTRPIPE] = "Streams pipe error";
    m[EUSERS] = "Too many users";
    m[ENOTSOCK] = "Socket operation on non-socket";
    m[EDESTADDRREQ] = "Destination address required";
    m[EMSGSIZE] = "Message too long";
    m[EPROTOTYPE] = "Protocol wrong type for socket";
    m[ENOPROTOOPT] = "Protocol not available";
    m[EPROTONOSUPPORT] = "Protocol not supported";
    m[ESOCKTNOSUPPORT] = "Socket type not supported";
    m[EOPNOTSUPP] = "Operation not supported on transport endpoint";
    m[EPFNOSUPPORT] = "Protocol family not supported";
    m[EAFNOSUPPORT] = "Address family not supported by protocol";
    m[EADDRINUSE] = "Address already in use";
    m[EADDRNOTAVAIL] = "Cannot assign requested address";
    m[ENETDOWN] = "Network is down";
    m[ENETUNREACH] = "Network is unreachable";
    m[ENETRESET] = "Network dropped connection because of reset";
    m[ECONNABORTED] = "Software caused connection abort";
    m[ECONNRESET] = "Connection reset by peer";
    m[ENOBUFS] = "No buffer space available";
    m[EISCONN] = "Transport endpoint is already connected";
    m[ENOTCONN] = "Transport endpoint is not connected";
    m[ESHUTDOWN] = "Cannot send after transport endpoint shutdown";
    m[ETOOMANYREFS] = "Too many references: cannot splice";
    m[ETIMEDOUT] = "Connection timed out";
    m[ECONNREFUSED] = "Connection refused";
    m[EHOSTDOWN] = "Host is down";
    m[EHOSTUNREACH] = "No route to host";
    m[EALREADY] = "Operation already in progress";
    m[EINPROGRESS] = "Operation now in progress";
    m[ESTALE] = "Stale NFS file handle";
    m[EUCLEAN] = "Structure needs cleaning";
    m[ENOTNAM] = "Not a XENIX named type file";
    m[ENAVAIL] = "No XENIX semaphores available";
    m[EISNAM] = "Is a named type file";
    m[EREMOTEIO] = "Remote I/O error";
    m[EDQUOT] = "Quota exceeded";
    m[ENOMEDIUM] = "No medium found";
    m[EMEDIUMTYPE] = "Wrong medium type";
    m[ECANCELED] = "Operation Canceled";
    m[ENOKEY] = "Required key not available";
    m[EKEYEXPIRED] = "Key has expired";
    m[EKEYREVOKED] = "Key has been revoked";
    m[EKEYREJECTED] = "Key was rejected by service";
    m[EOWNERDEAD] = "Owner died";
    m[ENOTRECOVERABLE] = "State not recoverable";
    m[ERFKILL] = "Operation not possible due to RF-kill";
    m[EHWPOISON] = "Memory page has hardware error";

    return m;
}

const map<int, std::string> Socket::socketErrorMap = Socket::createSocketErrorMap();

void Socket::doThrow(int code)
{

    map<int, std::string>::const_iterator findIt = Socket::socketErrorMap.find(code);
    if (findIt != Socket::socketErrorMap.end())
    {
        throw SocketException(findIt->second);
    }
    else if (code == ENOERR)
    {
        //OK
        return ;
    }
    else
    {
        throw SocketException("Unknown error");
    }

}

