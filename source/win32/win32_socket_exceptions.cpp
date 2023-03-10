#include "win32_socket_exceptions.hpp"

#include <map>
#include <string>

/*
namespace net {

	// Windows socket exceprions
	enum SocketExceptions {
		ERROR_WSA_INVALID_HANDLE = WSA_INVALID_HANDLE,
		ERROR_WSA_NOT_ENOUGH_MEMORY = WSA_NOT_ENOUGH_MEMORY,
		ERROR_WSA_INVALID_PARAMETER = WSA_INVALID_PARAMETER,
		ERROR_WSA_OPERATION_ABORTED = WSA_OPERATION_ABORTED,
		ERROR_WSA_IO_INCOMPLETE = WSA_IO_INCOMPLETE,
		ERROR_WSA_IO_PENDING = WSA_IO_PENDING,
		ERROR_WSAEINTR = WSAEINTR,
		ERROR_WSAEBADF = WSAEBADF,
		ERROR_WSAEACCES = WSAEACCES,
		ERROR_WSAEFAULT = WSAEFAULT,
		ERROR_WSAEINVAL = WSAEINVAL,
		ERROR_WSAEMFILE = WSAEMFILE,
		ERROR_WSAEWOULDBLOCK = WSAEWOULDBLOCK,
		ERROR_WSAEINPROGRESS = WSAEINPROGRESS,
		ERROR_WSAEALREADY = WSAEALREADY,
		ERROR_WSAENOTSOCK = WSAENOTSOCK,
		ERROR_WSAEDESTADDRREQ = WSAEDESTADDRREQ,
		ERROR_WSAEMSGSIZE = WSAEMSGSIZE,
		ERROR_WSAEPROTOTYPE = WSAEPROTOTYPE,
		ERROR_WSAENOPROTOOPT = WSAENOPROTOOPT,
		ERROR_WSAEPROTONOSUPPORT = WSAEPROTONOSUPPORT,
		ERROR_WSAESOCKTNOSUPPORT = WSAESOCKTNOSUPPORT,
		ERROR_WSAEOPNOTSUPP = WSAEOPNOTSUPP,
		ERROR_WSAEPFNOSUPPORT = WSAEPFNOSUPPORT,
		ERROR_WSAEAFNOSUPPORT = WSAEAFNOSUPPORT,
		ERROR_WSAEADDRINUSE = WSAEADDRINUSE,
		ERROR_WSAEADDRNOTAVAIL = WSAEADDRNOTAVAIL,
		ERROR_WSAENETDOWN = WSAENETDOWN,
		ERROR_WSAENETUNREACH = WSAENETUNREACH,
		ERROR_WSAENETRESET = WSAENETRESET,
		ERROR_WSAECONNABORTED = WSAECONNABORTED,
		ERROR_WSAECONNRESET = WSAECONNRESET,
		ERROR_WSAENOBUFS = WSAENOBUFS,
		ERROR_WSAEISCONN = WSAEISCONN,
		ERROR_WSAENOTCONN = WSAENOTCONN,
		ERROR_WSAESHUTDOWN = WSAESHUTDOWN,
		ERROR_WSAETOOMANYREFS = WSAETOOMANYREFS,
		ERROR_WSAETIMEDOUT = WSAETIMEDOUT,
		ERROR_WSAECONNREFUSED = WSAECONNREFUSED,
		ERROR_WSAELOOP = WSAELOOP,
		ERROR_WSAENAMETOOLONG = WSAENAMETOOLONG,
		ERROR_WSAEHOSTDOWN = WSAEHOSTDOWN,
		ERROR_WSAEHOSTUNREACH = WSAEHOSTUNREACH,
		ERROR_WSAENOTEMPTY = WSAENOTEMPTY,
		ERROR_WSAEPROCLIM = WSAEPROCLIM,
		ERROR_WSAEUSERS = WSAEUSERS,
		ERROR_WSAEDQUOT = WSAEDQUOT,
		ERROR_WSAESTALE = WSAESTALE,
		ERROR_WSAEREMOTE = WSAEREMOTE,
		ERROR_WSASYSNOTREADY = WSASYSNOTREADY,
		ERROR_WSAVERNOTSUPPORTED = WSAVERNOTSUPPORTED,
		ERROR_WSANOTINITIALISED = WSANOTINITIALISED,
		ERROR_WSAEDISCON = WSAEDISCON,
		ERROR_WSAENOMORE = WSAENOMORE,
		ERROR_WSAECANCELLED = WSAECANCELLED,
		ERROR_WSAEINVALIDPROCTABLE = WSAEINVALIDPROCTABLE,
		ERROR_WSAEINVALIDPROVIDER = WSAEINVALIDPROVIDER,
		ERROR_WSAEPROVIDERFAILEDINIT = WSAEPROVIDERFAILEDINIT,
		ERROR_WSASYSCALLFAILURE = WSASYSCALLFAILURE,
		ERROR_WSASERVICE_NOT_FOUND = WSASERVICE_NOT_FOUND,
		ERROR_WSATYPE_NOT_FOUND = WSATYPE_NOT_FOUND,
		ERROR_WSA_E_NO_MORE = WSA_E_NO_MORE,
		ERROR_WSA_E_CANCELLED = WSA_E_CANCELLED,
		ERROR_WSAEREFUSED = WSAEREFUSED,
		ERROR_WSAHOST_NOT_FOUND = WSAHOST_NOT_FOUND,
		ERROR_WSATRY_AGAIN = WSATRY_AGAIN,
		ERROR_WSANO_RECOVERY = WSANO_RECOVERY,
		ERROR_WSANO_DATA = WSANO_DATA,
		ERROR_WSA_QOS_RECEIVERS = WSA_QOS_RECEIVERS,
		ERROR_WSA_QOS_SENDERS = WSA_QOS_SENDERS,
		ERROR_WSA_QOS_NO_SENDERS = WSA_QOS_NO_SENDERS,
		ERROR_WSA_QOS_NO_RECEIVERS = WSA_QOS_NO_RECEIVERS,
		ERROR_WSA_QOS_REQUEST_CONFIRMED = WSA_QOS_REQUEST_CONFIRMED,
		ERROR_WSA_QOS_ADMISSION_FAILURE = WSA_QOS_ADMISSION_FAILURE,
		ERROR_WSA_QOS_POLICY_FAILURE = WSA_QOS_POLICY_FAILURE,
		ERROR_WSA_QOS_BAD_STYLE = WSA_QOS_BAD_STYLE,
		ERROR_WSA_QOS_BAD_OBJECT = WSA_QOS_BAD_OBJECT,
		ERROR_WSA_QOS_TRAFFIC_CTRL_ERROR = WSA_QOS_TRAFFIC_CTRL_ERROR,
		ERROR_WSA_QOS_GENERIC_ERROR = WSA_QOS_GENERIC_ERROR,
		ERROR_WSA_QOS_ESERVICETYPE = WSA_QOS_ESERVICETYPE,
		ERROR_WSA_QOS_EFLOWSPEC = WSA_QOS_EFLOWSPEC,
		ERROR_WSA_QOS_EPROVSPECBUF = WSA_QOS_EPROVSPECBUF,
		ERROR_WSA_QOS_EFILTERSTYLE = WSA_QOS_EFILTERSTYLE,
		ERROR_WSA_QOS_EFILTERTYPE = WSA_QOS_EFILTERTYPE,
		ERROR_WSA_QOS_EFILTERCOUNT = WSA_QOS_EFILTERCOUNT,
		ERROR_WSA_QOS_EOBJLENGTH = WSA_QOS_EOBJLENGTH,
		ERROR_WSA_QOS_EFLOWCOUNT = WSA_QOS_EFLOWCOUNT,
		ERROR_WSA_QOS_EUNKOWNPSOBJ = WSA_QOS_EUNKOWNPSOBJ,
		ERROR_WSA_QOS_EPOLICYOBJ = WSA_QOS_EPOLICYOBJ,
		ERROR_WSA_QOS_EFLOWDESC = WSA_QOS_EFLOWDESC,
		ERROR_WSA_QOS_EPSFLOWSPEC = WSA_QOS_EPSFLOWSPEC,
		ERROR_WSA_QOS_EPSFILTERSPEC = WSA_QOS_EPSFILTERSPEC,
		ERROR_WSA_QOS_ESDMODEOBJ = WSA_QOS_ESDMODEOBJ,
		ERROR_WSA_QOS_ESHAPERATEOBJ = WSA_QOS_ESHAPERATEOBJ,
		ERROR_WSA_QOS_RESERVED_PETYPE = WSA_QOS_RESERVED_PETYPE
	};

	std::map<SocketExceptions, const char*> ExceptionsMap
	{
		{ SocketExceptions::ERROR_WSA_INVALID_HANDLE, "WSA_INVALID_HANDLE" },
		{ SocketExceptions::ERROR_WSA_NOT_ENOUGH_MEMORY , "WSA_NOT_ENOUGH_MEMORY" },
		{ SocketExceptions::ERROR_WSA_INVALID_PARAMETER , "WSA_INVALID_PARAMETER" },
		{ SocketExceptions::ERROR_WSA_OPERATION_ABORTED , "WSA_OPERATION_ABORTED" },
		{ SocketExceptions::ERROR_WSA_IO_INCOMPLETE , "WSA_IO_INCOMPLETE" },
		{ SocketExceptions::ERROR_WSA_IO_PENDING , "WSA_IO_PENDING" },
		{ SocketExceptions::ERROR_WSAEINTR , "WSAEINTR" },
		{ SocketExceptions::ERROR_WSAEBADF , "WSAEBADF" },
		{ SocketExceptions::ERROR_WSAEACCES, "WSAEACCES" },
		{ SocketExceptions::ERROR_WSAEFAULT, "WSAEFAULT" },
		{ SocketExceptions::ERROR_WSAEINVAL, "WSAEINVAL" },
		{ SocketExceptions::ERROR_WSAEMFILE, "WSAEMFILE" },
		{ SocketExceptions::ERROR_WSAEWOULDBLOCK, "WSAEWOULDBLOCK" },
		{ SocketExceptions::ERROR_WSAEINPROGRESS, "WSAEINPROGRESS" },
		{ SocketExceptions::ERROR_WSAEALREADY, "WSAEALREADY" },
		{ SocketExceptions::ERROR_WSAENOTSOCK, "WSAENOTSOCK" },
		{ SocketExceptions::ERROR_WSAEDESTADDRREQ, "WSAEDESTADDRREQ" },
		{ SocketExceptions::ERROR_WSAEMSGSIZE, "WSAEMSGSIZE" },
		{ SocketExceptions::ERROR_WSAEPROTOTYPE, "WSAEPROTOTYPE" },
		{ SocketExceptions::ERROR_WSAENOPROTOOPT, "WSAENOPROTOOPT" },
		{ SocketExceptions::ERROR_WSAEPROTONOSUPPORT, "WSAEPROTONOSUPPORT " },
		{ SocketExceptions::ERROR_WSAESOCKTNOSUPPORT, "WSAESOCKTNOSUPPORT" },
		{ SocketExceptions::ERROR_WSAEOPNOTSUPP, "WSAEOPNOTSUPP" },
		{ SocketExceptions::ERROR_WSAEPFNOSUPPORT, "WSAEPFNOSUPPORT" },
		{ SocketExceptions::ERROR_WSAEAFNOSUPPORT, "WSAEAFNOSUPPORT" },
		{ SocketExceptions::ERROR_WSAEADDRINUSE, "WSAEADDRINUSE" },
		{ SocketExceptions::ERROR_WSAEADDRNOTAVAIL, "WSAEADDRNOTAVAIL" },
		{ SocketExceptions::ERROR_WSAENETDOWN, "WSAENETDOWN" },
		{ SocketExceptions::ERROR_WSAENETUNREACH, "WSAENETUNREACH" },
		{ SocketExceptions::ERROR_WSAENETRESET, "WSAENETRESET" },
		{ SocketExceptions::ERROR_WSAECONNABORTED, "WSAECONNABORTED" },
		{ SocketExceptions::ERROR_WSAECONNRESET, "WSAECONNRESET" },
		{ SocketExceptions::ERROR_WSAENOBUFS, "WSAENOBUFS " },
		{ SocketExceptions::ERROR_WSAEISCONN, "WSAEISCONN" },
		{ SocketExceptions::ERROR_WSAENOTCONN, "WSAENOTCONN" },
		{ SocketExceptions::ERROR_WSAESHUTDOWN, "WSAESHUTDOWN" },
		{ SocketExceptions::ERROR_WSAETOOMANYREFS, "WSAETOOMANYREFS" },
		{ SocketExceptions::ERROR_WSAETIMEDOUT, "WSAETIMEDOUT" },
		{ SocketExceptions::ERROR_WSAECONNREFUSED, "WSAECONNREFUSED" },
		{ SocketExceptions::ERROR_WSAELOOP, "WSAELOOP" },
		{ SocketExceptions::ERROR_WSAENAMETOOLONG, "WSAENAMETOOLONG" },
		{ SocketExceptions::ERROR_WSAEHOSTDOWN, "WSAEHOSTDOWN" },
		{ SocketExceptions::ERROR_WSAEHOSTUNREACH, "WSAEHOSTUNREACH" },
		{ SocketExceptions::ERROR_WSAENOTEMPTY, "WSAENOTEMPTY" },
		{ SocketExceptions::ERROR_WSAEPROCLIM, "WSAEPROCLIM" },
		{ SocketExceptions::ERROR_WSAEUSERS, "WSAEUSERS" },
		{ SocketExceptions::ERROR_WSAEDQUOT, "WSAEDQUOT" },
		{ SocketExceptions::ERROR_WSAESTALE, "WSAESTALE" },
		{ SocketExceptions::ERROR_WSAEREMOTE, "WSAEREMOTE" },
		{ SocketExceptions::ERROR_WSASYSNOTREADY, "WSASYSNOTREADY" },
		{ SocketExceptions::ERROR_WSAVERNOTSUPPORTED, "WSAVERNOTSUPPORTED" },
		{ SocketExceptions::ERROR_WSANOTINITIALISED, "WSANOTINITIALISED" },
		{ SocketExceptions::ERROR_WSAEDISCON, "WSAEDISCON" },
		{ SocketExceptions::ERROR_WSAENOMORE, "WSAENOMORE" },
		{ SocketExceptions::ERROR_WSAECANCELLED, "WSAECANCELLED" },
		{ SocketExceptions::ERROR_WSAEINVALIDPROCTABLE, "WSAEINVALIDPROCTABLE" },
		{ SocketExceptions::ERROR_WSAEINVALIDPROVIDER, "WSAEINVALIDPROVIDER" },
		{ SocketExceptions::ERROR_WSAEPROVIDERFAILEDINIT, "WSAEPROVIDERFAILEDINIT" },
		{ SocketExceptions::ERROR_WSASYSCALLFAILURE, "WSASYSCALLFAILURE" },
		{ SocketExceptions::ERROR_WSASERVICE_NOT_FOUND, "WSASERVICE_NOT_FOUND" },
		{ SocketExceptions::ERROR_WSATYPE_NOT_FOUND, "WSATYPE_NOT_FOUND" },
		{ SocketExceptions::ERROR_WSA_E_NO_MORE, "WSA_E_NO_MORE" },
		{ SocketExceptions::ERROR_WSA_E_CANCELLED, "WSA_E_CANCELLED" },
		{ SocketExceptions::ERROR_WSAEREFUSED, "WSAEREFUSED" },
		{ SocketExceptions::ERROR_WSAHOST_NOT_FOUND, "WSAHOST_NOT_FOUND" },
		{ SocketExceptions::ERROR_WSATRY_AGAIN, "WSATRY_AGAIN" },
		{ SocketExceptions::ERROR_WSANO_RECOVERY, "WSANO_RECOVERY" },
		{ SocketExceptions::ERROR_WSANO_DATA, "WSANO_DATA" },
		{ SocketExceptions::ERROR_WSA_QOS_RECEIVERS, "WSA_QOS_RECEIVERS" },
		{ SocketExceptions::ERROR_WSA_QOS_SENDERS, "WSA_QOS_SENDERS" },
		{ SocketExceptions::ERROR_WSA_QOS_NO_SENDERS, "WSA_QOS_NO_SENDERS" },
		{ SocketExceptions::ERROR_WSA_QOS_NO_RECEIVERS, "WSA_QOS_NO_RECEIVERS" },
		{ SocketExceptions::ERROR_WSA_QOS_REQUEST_CONFIRMED, "WSA_QOS_REQUEST_CONFIRMED" },
		{ SocketExceptions::ERROR_WSA_QOS_ADMISSION_FAILURE, "WSA_QOS_ADMISSION_FAILURE" },
		{ SocketExceptions::ERROR_WSA_QOS_POLICY_FAILURE, "WSA_QOS_POLICY_FAILURE" },
		{ SocketExceptions::ERROR_WSA_QOS_BAD_STYLE, "WSA_QOS_BAD_STYLE" },
		{ SocketExceptions::ERROR_WSA_QOS_BAD_OBJECT, "WSA_QOS_BAD_OBJECT" },
		{ SocketExceptions::ERROR_WSA_QOS_TRAFFIC_CTRL_ERROR, "WSA_QOS_TRAFFIC_CTRL_{ ERROR" },
		{ SocketExceptions::ERROR_WSA_QOS_GENERIC_ERROR, "WSA_QOS_GENERIC_{ ERROR" },
		{ SocketExceptions::ERROR_WSA_QOS_ESERVICETYPE, "WSA_QOS_ESERVICETYPE" },
		{ SocketExceptions::ERROR_WSA_QOS_EFLOWSPEC, "WSA_QOS_EFLOWSPEC" },
		{ SocketExceptions::ERROR_WSA_QOS_EPROVSPECBUF, "WSA_QOS_EPROVSPECBUF" },
		{ SocketExceptions::ERROR_WSA_QOS_EFILTERSTYLE, "WSA_QOS_EFILTERSTYLE" },
		{ SocketExceptions::ERROR_WSA_QOS_EFILTERTYPE, "WSA_QOS_EFILTERTYPE" },
		{ SocketExceptions::ERROR_WSA_QOS_EFILTERCOUNT, "WSA_QOS_EFILTERCOUNT" },
		{ SocketExceptions::ERROR_WSA_QOS_EOBJLENGTH, "WSA_QOS_EOBJLENGTH" },
		{ SocketExceptions::ERROR_WSA_QOS_EFLOWCOUNT, "WSA_QOS_EFLOWCOUNT" },
		{ SocketExceptions::ERROR_WSA_QOS_EUNKOWNPSOBJ, "WSA_QOS_EUNKOWNPSOBJ" },
		{ SocketExceptions::ERROR_WSA_QOS_EPOLICYOBJ, "WSA_QOS_EPOLICYOBJ" },
		{ SocketExceptions::ERROR_WSA_QOS_EFLOWDESC, "WSA_QOS_EFLOWDESC" },
		{ SocketExceptions::ERROR_WSA_QOS_EPSFLOWSPEC, "WSA_QOS_EPSFLOWSPEC" },
		{ SocketExceptions::ERROR_WSA_QOS_EPSFILTERSPEC, "WSA_QOS_EPSFILTERSPEC" },
		{ SocketExceptions::ERROR_WSA_QOS_ESDMODEOBJ, "WSA_QOS_ESDMODEOBJ" },
		{ SocketExceptions::ERROR_WSA_QOS_ESHAPERATEOBJ, "WSA_QOS_ESHAPERATEOBJ" },
		{ SocketExceptions::ERROR_WSA_QOS_RESERVED_PETYPE, "QOS_RESERVED_PETYPE" }
	};

} // namespace Net
*/