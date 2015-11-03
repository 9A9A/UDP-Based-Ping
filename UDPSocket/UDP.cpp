#include "UDP.h"

bool isNumber ( const string &str )
{
	return str.find_first_not_of ( "0123456789" ) == std::string::npos;
}
void ParseRequest ( string &str )
{
	if ( str.size ( ) )
	{
		vector<string> stringList;
		char *p = strtok ( const_cast< char* >( str.c_str ( ) ) , " :" );
		while ( p )
		{
			stringList.push_back ( p );
			p = strtok ( nullptr , " :" );
		}
		if ( stringList.size ( ) < 1 )
		{
			cout << "Emtpy parameter list\n";
		}
		else
		{
			short nPort = 7; // default Echo protocol port RFC 862
			size_t attempts = 4;
			size_t packetsize = 32;
			IPv4 ip;
			auto ping = [ & ] ( )->void
			{
				if ( !ip.empty ( ) )
				{
					UDPClient clnt ( nPort );
					clnt.Ping ( ip , attempts , packetsize );
				}
				else
				{
					cout << "Empty ip\n";
				}
			};
			if ( stringList.size ( ) == 1 )
			{
				if ( stringList [ 0 ] == "exit" || stringList [ 0 ] == "quit" || stringList [ 0 ] == "q" )
				{
					exit ( 0 );
				}
				if ( stringList [ 0 ] != "-h" )
				{
					ip = IPv4 ( stringList [ 0 ] );
#ifdef _DEBUG
					cout << "Destination ip : " << ip.ToStr ( ) << endl;
#endif
				}
				else
				{
					cout << "UDP based ping utility. Supports only IPv4 based addresses.\n";
					cout << "xxx.xxx.xxx.xxx - send 4 packets 32 bytes each\n";
					cout << "xxx.xxx.xxx.xxx:yyyyy - send 4 packets 32 bytes each to specified port\n";
					cout << "-s [PACKET SIZE]\tsetup packet size [0 - " << MaxBufferSize << "]\n";
					cout << "-n [PACKET AMOUNT]\tsetup packet amount\n";
				}
				ping ( );
			}
			else if ( stringList.size ( ) == 2 )
			{
				ip = IPv4 ( stringList [ 0 ] );
#ifdef _DEBUG
				cout << "Destination ip : " << ip.ToStr ( ) << endl;
#endif
				if ( isNumber ( stringList [ 1 ] ) )
				{
					if ( stoi ( stringList [ 1 ] ) <= 65535 )
					{
						nPort = stoi ( stringList [ 1 ] );
					}
					else
					{
						cout << "Port value overflow\n";
					}
#ifdef _DEBUG
					cout << "Destination port : " << nPort << endl;
#endif
				}
				ping ( );
			}
			else
			{
				ip = IPv4 ( stringList [ 0 ] );
#ifdef _DEBUG
				cout << "Destination ip : " << ip.ToStr ( ) << endl;
#endif
				if ( isNumber ( stringList [ 1 ] ) )
				{
					if ( stoi ( stringList [ 1 ] ) < 65535 )
					{
						nPort = stoi ( stringList [ 1 ] );
					}
					else
					{
						cout << "Port value overflow\n";
					}
#ifdef _DEBUG
					cout << "Destination port : " << nPort << endl;
#endif
				}
				for ( size_t i = 1; i < stringList.size ( ) - 1; i++ )
				{
					if ( isNumber ( stringList [ i + 1 ] ) )
					{
						if ( stringList [ i ] == "-n" )
						{
							attempts = stoi ( stringList [ i + 1 ] );
#ifdef _DEBUG
							cout << "Packets to send : " << attempts << endl;
#endif
						}
						else if ( stringList [ i ] == "-s" )
						{
							if ( stoi ( stringList [ i + 1 ] ) <= MaxBufferSize )
							{
								packetsize = stoi ( stringList [ i + 1 ] );
							}
							else
							{
								cout << "Packet size must be lower than " << MaxBufferSize << endl;
							}
#ifdef _DEBUG
							cout << "Packet size : " << packetsize << endl;
#endif
						}
					}
				}
				ping ( );
			}
		}
	}
	else
	{
		cout << "Empty request string\n";
	}
}
void swap ( IPv4 &lhs , IPv4 &rhs )
{
	using std::swap;
	swap ( lhs.m_IpAddress [ 0 ] , rhs.m_IpAddress [ 0 ] );
	swap ( lhs.m_IpAddress [ 1 ] , rhs.m_IpAddress [ 1 ] );
	swap ( lhs.m_IpAddress [ 2 ] , rhs.m_IpAddress [ 2 ] );
	swap ( lhs.m_IpAddress [ 3 ] , rhs.m_IpAddress [ 3 ] );
}
IPv4& IPv4::operator = ( const IPv4 &obj )
{
	IPv4 copy ( obj );
	swap ( *this , copy );
	return *this;
}
IPv4::IPv4 ( )
{
	m_IpAddress [ 0 ] = 0;
	m_IpAddress [ 1 ] = 0;
	m_IpAddress [ 2 ] = 0;
	m_IpAddress [ 3 ] = 0;
}
IPv4::IPv4 ( byte a , byte b , byte c , byte d )
{
	m_IpAddress [ 0 ] = a;
	m_IpAddress [ 1 ] = b;
	m_IpAddress [ 2 ] = c;
	m_IpAddress [ 3 ] = d;
}
IPv4::IPv4 ( const IPv4 &obj )
{
	if ( &obj != this )
	{
		m_IpAddress [ 0 ] = obj.m_IpAddress [ 0 ];
		m_IpAddress [ 1 ] = obj.m_IpAddress [ 1 ];
		m_IpAddress [ 2 ] = obj.m_IpAddress [ 2 ];
		m_IpAddress [ 3 ] = obj.m_IpAddress [ 3 ];
	}
}
IPv4::IPv4 ( string str )
{
	try
	{
		while ( str.back ( ) == '\0' )
		{
			str.pop_back ( );
		}
		if ( str.size ( ) <= 15 && str.size ( ) >= 7 )
		{
			string x;
			size_t p = 0;
			for ( auto &i : str )
			{
				if ( ( i != '.' ) && ( i != ',' ) )
				{
					x.push_back ( i );
				}
				else
				{
					if ( isNumber ( x ) )
					{
						int temp = stoi ( x );
						x.clear ( );
						if ( temp > 255 )
						{
							throw runtime_error ( "Construction IPv4 failed : value overflow\n" );
						}
						else
						{
							m_IpAddress [ p++ ] = temp;
						}
					}
					else
					{
						throw runtime_error ( "Construction IPv4 failed : part of ip is not number\n" );
					}
				}
			}
			if ( isNumber ( x ) )
			{
				int temp = stoi ( x );
				x.clear ( );
				if ( temp > 255 )
				{
					throw runtime_error ( "Construction IPv4 failed : value overflow\n" );
				}
				else
				{
					m_IpAddress [ p++ ] = temp;
				}
			}
			else
			{
				throw runtime_error ( "Construction IPv4 failed : part of ip is not number\n" );
			}
		}
		else
		{
			throw runtime_error ( "Construction IPv4 failed : size mismatch\n" );
		}
	}
	catch ( runtime_error &err )
	{
		cout << err.what ( );
		*this = IPv4 ( );
	}
}
bool IPv4::empty ( )
{
	return ( !m_IpAddress [ 0 ] && !m_IpAddress [ 1 ] && !m_IpAddress [ 2 ] && !m_IpAddress [ 3 ] ) ? true : false;
}
string IPv4::ToStr ( )
{
	return string ( to_string ( m_IpAddress [ 0 ] ) + '.' + to_string ( m_IpAddress [ 1 ] ) + '.' + to_string ( m_IpAddress [ 2 ] ) + '.' + to_string ( m_IpAddress [ 3 ] ) );
}
void UDPClient::StartThread ( )
{
	if ( !threadActive )
	{
		threadActive = true;
		m_thread = unique_ptr<thread> ( new thread ( &UDPClient::Update , this ) );
	}
}
void UDPClient::StopThread ( )
{
	if ( threadActive )
	{
		threadActive = false;
		shutdown ( m_socket , SD_BOTH );
		closesocket ( m_socket );
		m_thread->join ( );
	}
}
void UDPClient::Update ( )
{
	size_t i = 1;
	char pBuf [ MaxBufferSize ];
	while ( threadActive )
	{
		int nSize = sizeof ( m_destination );
		int nResult = recvfrom ( m_socket , pBuf , MaxBufferSize , 0 , NULL , NULL );
		if ( m_bWrited && threadActive )
		{
			interval = duration_cast< microseconds >( system_clock::now ( ).time_since_epoch ( ) - m_SendPoint.time_since_epoch ( ) ).count ( );
			m_Stats.push_back ( interval );
			cout << i++ << ". > " << m_ip.ToStr ( ) << " : " << interval / 1000.0f << " ms\n";
		}
		if ( nResult == SOCKET_ERROR )
		{
			m_bWrited = false;
			continue;
		}
		else
		{
			m_bWrited = false;
			cout << pBuf << endl;
		}
	}
#ifdef _DEBUG
	cout << "Thread " << this_thread::get_id ( ) << " terminated" << endl;
#endif
}
UDPClient::UDPClient ( unsigned short port )
{
	WSADATA wsaDATA = { 0 };
	m_socket = INVALID_SOCKET;
	m_port = port;
	int nResult = WSAStartup ( MAKEWORD ( 2 , 2 ) , &wsaDATA );
	if ( nResult != 0 )
	{
		throw runtime_error ( "WSA Startup failed\n" );
	}
	m_socket = socket ( AF_INET , SOCK_DGRAM , IPPROTO_UDP );
	if ( m_socket == INVALID_SOCKET )
	{
		nResult = closesocket ( m_socket );
		if ( nResult == SOCKET_ERROR )
		{
			WSACleanup ( );
			throw runtime_error ( "close socket failed\n" );
		}
		throw runtime_error ( "unable to create socket\n" );
	}
	m_destination.sin_addr.s_addr = inet_addr ( "127.0.0.1" );
	m_destination.sin_port = htons ( m_port );
	m_destination.sin_family = AF_INET;
	StartThread ( );
}
UDPClient::~UDPClient ( )
{
	StopThread ( );
	WSACleanup ( );
#ifdef _DEBUG
	cout << "Object has been destroyed\n";
#endif
}
void UDPClient::Ping ( IPv4 &ip , size_t attemps , size_t packet_size )
{
	cout << "Destination : [" << ip.ToStr ( ) << ":" << m_port << "] - sending " << attemps << " packets " << packet_size << " bytes\n";
	m_ip = ip;
	vector<byte> packet ( packet_size );
	for ( size_t i = 0; i < packet_size; i++ )
	{
		packet [ i ] = i % 255;
	}
	for ( size_t i = 0; i < attemps; i++ )
	{
		Write ( ip , packet );
		Sleep ( MaxTimeout );
		if ( m_bWrited )
		{
			m_bWrited = false;
			cout << "Request timed out.\n";
		}
	}
	if ( m_Stats.size ( ) )
	{
		cout << "Statistics for [" << m_ip.ToStr ( ) << ":" << m_port << "] :\n";
		cout << "Sended : " << attemps << "\t\tReceived : " << m_Stats.size ( ) << "\t\tPacket Loss : " << 100 - ( size_t ) ( ( double ) m_Stats.size ( ) * 100 / ( double ) attemps ) << "%\n";
		cout << "Minimum : " << Min ( m_Stats ) / 1000.0f << "ms\tAverage : " << Average ( m_Stats ) / 1000.0f << "ms\tMaximum : " << Max ( m_Stats ) / 1000.0f << "ms\n";
	}
	else
	{
		cout << "Statistics for [" << m_ip.ToStr ( ) << ":" << m_port << "] :\n";
		cout << "Sended : " << attemps << "\t\tReceived : " << m_Stats.size ( ) << "\t\tPacket Loss : " << 100 - ( size_t ) ( ( double ) m_Stats.size ( ) * 100 / ( double ) attemps ) << "%\n";
		cout << "Minimum : n\\a\t\tAverage : n\\a\t\tMaximum : n\\a\n";
	}
}
void UDPClient::Write ( IPv4 &addr , vector<byte> &vec )
{
	m_ip = addr;
	m_destination.sin_addr.s_addr = inet_addr ( addr.ToStr ( ).data ( ) );
	int nResult = sendto ( m_socket , reinterpret_cast< char* >( vec.data ( ) ) , vec.size ( ) , NULL , ( SOCKADDR * ) &m_destination , sizeof ( m_destination ) );
	m_SendPoint = system_clock::now ( );
	if ( nResult == SOCKET_ERROR )
	{
		cout << "transmission failed\n";
	}
	else
	{
		m_bWrited = true;
	}
}