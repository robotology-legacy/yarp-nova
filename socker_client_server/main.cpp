/*
 * A socket client-server example.
 *
 *
 */

/**
 * \file main.cpp
 * A socket client-server example using the game library.
 */

#include <iostream>
using namespace std;

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include "NovaSemaphore.h"
#include "NovaThread.h"
#include "NovaTime.h"
#include "NovaInit.h"
#include "NovaClient.h"
#include "NovaServer.h"

//const int N = 10;

/*
 *
 */
class Server : public NovaThread 
{
protected:
	NovaServer server;
	NovaClient stream;
	char data[256];

public:
	virtual void main() 
	{
		while (!isEnding())
		{
			memset (data, 0, 256);

			int result;
			do
			{
				result = server.begin(10001);
			}
			while (result != 0);

			server.accept (stream);
			
			while (!isEnding() && !(data[0]=='q' && data[1]=='!'))
			{
				result = stream.receive (data, 2, -1);
				if (result != 2)
					break;

				cout << "received : " << data[0] << data[1] << endl;
				//NovaTime::sleep(1);
			}

			if ((data[0] == 'q' && data[1] == '!') || isEnding())
				break;
		}

		cout << "server completed!" << endl;
	}
};

/*
 *
 */
class Client : public NovaThread 
{
protected:
	NovaClient client;
	char data[256];

public:
	virtual void main() 
	{
		memset (data, 0, 256);
		client.connect ("localhost", 10001);
		data[0] = 'A';
		data[1] = 'a';
		while (!isEnding()) 
		{
			data[0] ++; if (data[0] > 'Z') data[0] = 'A';
			data[1] ++; if (data[1] > 'z') data[1] = 'a';
			cout << "about to send " << data[0] << data[1] << endl;
			client.send (data, 2);

			NovaTime::sleep(0.5);
		}

		data[0] = 'q';
		data[1] = '!';
		client.send (data, 2);

		NovaTime::sleep(2);

		cout << "client completed!" << endl;
	}
};


/*
 *
 */
int main (int argc, char *argv[])
{
	NovaInit::init ();
	/// 
	Server s;
	Client c;

	cout << "Starting server (listener)" << endl;
	s.begin();

	NovaTime::sleep (1);
	cout << "Starting one client (sender)" << endl;

	c.begin();

	cout << "Communicating..." << endl;

	NovaTime::sleep (20);

	c.end ();

	NovaTime::sleep (1);
	s.end ();

	NovaInit::fini ();

	cout << "Test completed!" << endl;
	NovaTime::sleep (1);

	return 0;
}

