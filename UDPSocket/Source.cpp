#include "UDP.h"

int main ( )
{
	try
	{
		while ( true )
		{
			string str;
			getline ( cin , str );
			ParseRequest ( str );
		}
	}
	catch ( runtime_error &err )
	{
		cout << err.what ( );
	}
	system ( "pause" );
}