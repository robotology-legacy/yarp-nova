/*
 * A producer-consumer example.
 *
 *
 */

/**
 * \file main.cpp
 * A producer-consumer example using the game library.
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include "NovaSemaphore.h"
#include "NovaThread.h"
#include "NovaTime.h"
#include "NovaInit.h"

const int N = 10;

/*
 * the shared buffer between the producer and the consumer.
 */
class MyBuffer
{
protected:
	int shared_buffer[N];
	int write_ptr;
	int read_ptr;

public:
	MyBuffer() { write_ptr = read_ptr = 0; }

	void inert_item (int item_value);
	int remove_item (void);
};

MyBuffer buffer;				// the instance of the buffer.

NovaSemaphore mutex(1);			// mutual exclusion.
NovaSemaphore empty(N);			// how many slots are empty.
NovaSemaphore full(0);			// how many slots are full.

/*
 *
 */
class Producer : public NovaThread 
{
public:
	virtual void main() 
	{
		while (!isEnding()) 
		{
			//count++;
			//printf("Count is %d\n", count);
			NovaTime::sleep(1);
		}
	}
};

/*
 *
 */
class Consumer : public NovaThread 
{
public:
	virtual void main() 
	{
		while (!isEnding()) 
		{
			//count++;
			//printf("Count is %d\n", count);
			NovaTime::sleep(1);
		}
	}
};


/*
 *
 */
int main (int argc, char *argv[])
{
	NovaInit::init ();
	/// 
	Producer p;
	Consumer c;

	printf ("Main: starting producer\n");
	p.begin ();
	NovaTime::sleep (1);
	
	printf ("Main: starting consumer\n");
	c.begin ();

	printf ("Type a key and ret to quit\n");
	char d;
	scanf ("%c", &d);

	p.end ();
	c.end ();

	NovaInit::fini ();
	return 0;
}

