/*
 * A telnet client example.
 *
 *
 */

/**
 * \file main.cpp
 * A telnet client example using the game library.
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
#include "NovaTelnetClient.h"

/*
 *
 */
class Client : public NovaThread 
{
protected:
	NovaTelnetClient client;
	NovaSemaphore mutex;

public:
	Client () : mutex(0) {}

	void send (const char *str)
	{
		mutex.wait();
		client.sendText (str);
		mutex.post();
	}

	virtual void main() 
	{
		client.connect ("localhost", 9999);
		mutex.post();
		cout << "client connected" << endl;

		while (!isEnding()) 
		{
			const char *str = client.receiveText ();
			mutex.wait();
			if (str != NULL)
				cout << str << endl;
			mutex.post();
		}

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
	Client c;
	cout << "Starting one telnet client" << endl;

	c.begin();

	cout << "Communicating..." << endl;

	char text[512];
	memset (text, 0, 512);
	while (text[0] != 'q')
	{
		memset (text, 0, 512);
		cout << "-> ";
		cin >> text;
		int wh = strlen(text);
		if (!strcmp(text, "connect"))
		{
			text[wh] = ' ';
			cin >> text+wh+1;
			wh = strlen(text);
			text[wh] = ' ';
			cin >> text+wh+1;
		}
		else
		if (!strcmp(text, "go"))
		{
			text[wh] = ' ';
			cin >> text+wh+1;
		}

		cout << "got: " << text << endl;
		if (text[0] == 'q')
		{
			cout << "terminating threads..." << endl;
			c.end(0);
		}
		else
		{
			c.send (text);
			NovaTime::sleep (1);
		}
	}

	///
	NovaInit::fini ();

	cout << "Test completed!" << endl;
	NovaTime::sleep (1);

	return 0;
}

