#ifndef _UDP_H_
#define _UDP_H_
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include <iomanip>
#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <iostream>
#include <algorithm>
#include <memory>
#include <WinSock2.h>

typedef unsigned char byte;
using namespace std;
using namespace chrono;

const size_t MaxBufferSize = 1024;
const size_t MaxTimeout = 1000;

#define DISABLE_COPY(T) void operator = (const T &obj); \
								T(const T &);
#define DISABLE_MOVE(T) T& operator = (T&& obj); \
								T(T&& obj);


void ParseRequest ( string &str );
bool isNumber ( const string &str );



template<typename _T> double Average ( vector<_T> &vec )
{
	double avg = 0;
	for ( auto &i : vec )
	{
		avg += i.count();
	}
	return avg / vec.size ( );
}
template<typename _T> double Max ( vector<_T> &vec )
{
	double max = DBL_MIN;
	for ( auto &i : vec )
	{
		max = ( i.count() > max ) ? i.count() : max;
	}
	return max;
}
template<typename _T> double Min ( vector<_T> &vec )
{
	double min = DBL_MAX;
	for ( auto &i : vec )
	{
		min = ( i.count() < min ) ? i.count() : min;
	}
	return min;
}

class IPv4
{
public:
	IPv4 ( );
	IPv4 ( byte a , byte b , byte c , byte d );
	IPv4 ( const IPv4 &obj );

	IPv4 ( string str );
	IPv4& operator = ( const IPv4 &obj );
	string ToStr ( );
	bool empty ( );
	friend void swap ( IPv4 &lhs , IPv4 &rhs );
private:
	byte m_IpAddress [ 4 ];
};
class UDPClient
{
public:
	UDPClient ( unsigned short port );
	virtual ~UDPClient ( );
	virtual void Ping ( IPv4 &ip , size_t attemps , size_t packet_size );
	virtual void Write ( IPv4 &addr , vector<byte> &vec );
protected:
	virtual void Update ( );
private:
	void StartThread ( );
	void StopThread ( );


	vector<microseconds> m_Stats;
	microseconds interval;
	atomic<bool> m_bWrited;
	high_resolution_clock::time_point m_SendPoint;
	SOCKET m_socket;
	struct sockaddr_in m_destination;
	atomic<bool> threadActive;
	unique_ptr<thread> m_thread;
	uint16_t m_port;
	IPv4 m_ip;
	DISABLE_COPY ( UDPClient );
	DISABLE_MOVE ( UDPClient );
};


#endif