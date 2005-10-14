
#ifndef NOVA_CLIENT_H
#define NOVA_CLIENT_H

class NovaServer;
class NovaStream;


/**
 * A class for managing the connection between a client and
 * a server.
 */

class NovaClient {
public:

  /**
   * Constructor.
   *  Creates an empty connection.  Before sending or receiving, you
   *  need to connect().
   */
  NovaClient();

  /**
   * Destructor.
   *  Shuts the connection down.
   */
  virtual ~NovaClient();

  /**
   * Establish a connection with a port on a particular machine.
   * @param hostname the machine with which to connect
   * @param the port to connect to on the remote machine
   */
  void connect(const char *hostname, int port);

  /**
   * Establish a connection.
   * @param server the object visible to remote clients
   */
  void connect(NovaServer& server);

  /**
   * Send data out on the connection
   * @param data the buffer to send
   * @param len the size of the buffer in bytes
   * @return number of bytes sent, or a negative number upon error
   */
  int send(const char *data, int len);

  /**
   * Receive data send from the remote machine through the connection
   * @param data a place to store the date
   * @param len the size of the buffer in bytes
   * @param timeout how long to wait (in seconds - negative means forever)
   * @return number of bytes received, or a negative number upon error
   */
  int receive(char *data, int len, double timeout = -1);

private:
  /**
   * Internal helper object.
   */
  void *system_resource;

  /**
   * Internal stream object
   */
  NovaStream& getStream();

  /**
   * Needs to cooperate with the server class
   */
  friend class NovaServer;
};

#endif
