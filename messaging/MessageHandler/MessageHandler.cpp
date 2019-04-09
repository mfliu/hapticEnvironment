#include "MessageHandler.h"
#include <typeinfo>
#include <fcntl.h>

MessageHandler::MessageHandler(const char* address, int iPort, int oPort, int diPort, int doPort)
{
  ipAddr = address;
  inputPort = iPort;
  outputPort = oPort;
  dataInPort = diPort;
  dataOutPort = doPort;
  startTime = high_resolution_clock::now();
  running = true;
}

int MessageHandler::getMsgNum()
{
  return msgNum++;
}

double MessageHandler::getTimestamp()
{
  high_resolution_clock::time_point currTime = high_resolution_clock::now();
  duration<double> timeNow = duration_cast<duration<double>> (currTime-startTime);
  return timeNow.count();
}

void MessageHandler::openInputSocket()
{
  cout << "Opening listening socket..." << endl;
  input_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (input_socket < 0)
  {
    cout << "Open listening socket failed." << endl;
    exit(1);
  }
  fcntl(input_socket, F_SETFL, O_NONBLOCK);

  memset((char *) &inputStruct, 0, inputLen);
  inputStruct.sin_family = AF_INET;
  inputStruct.sin_port = htons(inputPort);
  if (inet_aton(ipAddr, &inputStruct.sin_addr) == 0)
  {
    cout << "inet_aton failed" << endl;
    exit(1);
  }

  int opt = 1;
  int broadcast = setsockopt(input_socket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
  int reuseAddr = setsockopt(input_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  int reusePort = setsockopt(input_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
  if (broadcast < 0 || reuseAddr < 0 || reusePort < 0)
  {
    cout << "Failed to set socket option" << endl;
    exit(1);
  }
  
  int bind_sock_in = bind(input_socket, (struct sockaddr*) &inputStruct, inputLen);
  if (bind_sock_in < 0)  {
    cout << "Error binding listener socket" << endl;
  }
}

void MessageHandler::openOutputSocket()
{
  cout << "Opening sending socket..." << endl;
  output_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (output_socket < 0)
  {
    cout << "Open sending socket failed." << endl;
    exit(1);
  }
  fcntl(output_socket, F_SETFL, O_NONBLOCK);

  memset((char *) &outputStruct, 0, outputLen);
  outputStruct.sin_family = AF_INET;
  outputStruct.sin_port = htons(outputPort);
  if (inet_aton(ipAddr, &outputStruct.sin_addr) == 0)
  {
    cout << "inet_aton failed" << endl;
    exit(1);
  }

  int opt = 1;
  int broadcast = setsockopt(output_socket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
  int reuseAddr = setsockopt(output_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  int reusePort = setsockopt(output_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
  if (broadcast < 0 || reuseAddr < 0 || reusePort < 0) {
    cout << "Failed to set socket options" << endl;
    exit(1);
  }
}

void MessageHandler::openDataSockets()
{
  cout << "Opening data in socket..." << endl;
  dataIn_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (dataIn_socket < 0)
  {
    cout << "Open listening socket failed." << endl;
    exit(1);
  }
  memset((char *) &dataInStruct, 0, dataInLen);
  dataInStruct.sin_family = AF_INET;
  dataInStruct.sin_port = htons(dataInPort);
  if (inet_aton(ipAddr, &dataInStruct.sin_addr) == 0)
  {
    cout << "inet_aton failed" << endl;
    exit(1);
  }

  int opt = 1;
  int broadcast = setsockopt(dataIn_socket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
  int reuseAddr = setsockopt(dataIn_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  int reusePort = setsockopt(dataIn_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
  if (broadcast < 0 || reuseAddr < 0 || reusePort < 0)
  {
    cout << "Failed to set socket option" << endl;
    exit(1);
  }
  
  int bind_sock_in = bind(dataIn_socket, (struct sockaddr*) &dataInStruct, dataInLen);
  if (bind_sock_in < 0)  {
    cout << "Error binding listener socket" << endl;
  }

  cout << "Opening data out socket..." << endl;
  dataOut_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (dataOut_socket < 0)
  {
    cout << "Open data out socket failed." << endl;
    exit(1);
  }
  memset((char *) &dataOutStruct, 0, dataOutLen);
  dataOutStruct.sin_family = AF_INET;
  dataOutStruct.sin_port = htons(dataOutPort);
  if (inet_aton(ipAddr, &dataOutStruct.sin_addr) == 0)
  {
    cout << "inet_aton failed" << endl;
    exit(1);
  }

  //int opt = 1;
  broadcast = setsockopt(dataOut_socket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
  reuseAddr = setsockopt(dataOut_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  reusePort = setsockopt(dataOut_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
  if (broadcast < 0 || reuseAddr < 0 || reusePort < 0)
  {
    cout << "Failed to set socket option" << endl;
    exit(1);
  }
}
void MessageHandler::closeInputSocket()
{
  close(input_socket); 
}

void MessageHandler::closeOutputSocket()
{
  close(output_socket);
}

void MessageHandler::closeDataSockets()
{
  close(dataIn_socket);
  close(dataOut_socket);
}

int MessageHandler::readPacket(char* packetPointer)
{
  int value = 0, bytesRead = 0;
  ioctl(input_socket, FIONREAD, &value);
  if (value > 0) {
    bytesRead = recvfrom(input_socket, packetPointer, MAX_PACKET_LENGTH, 0, (struct sockaddr*) &inputStruct, (socklen_t*) &inputLen);
  }
  return bytesRead;
}

int MessageHandler::sendPacket(char* packet, uint16_t lengthPacket)
{
  if (sendto(output_socket, packet, lengthPacket, 0, (struct sockaddr*) &outputStruct, outputLen) < 0)
  {
    cout << "Data sending error" << endl;
    return 0;
  }
  return 1;
}

int MessageHandler::readData(char* packetPointer)
{
  int value = 0, bytesRead = 0;
  ioctl(dataIn_socket, FIONREAD, &value);
  if (value > 0) {
    bytesRead = recvfrom(dataIn_socket, packetPointer, MAX_PACKET_LENGTH, 0, (struct sockaddr*) &dataInStruct, (socklen_t*) &dataInLen);
  }
  return bytesRead;
}

int MessageHandler::sendData(char* packet, uint16_t lengthPacket)
{
  if (sendto(dataOut_socket, packet, lengthPacket, 0, (struct sockaddr*) &dataOutStruct, dataOutLen) < 0)
  {
    cout << "Data sending error" << endl;
    return 0;
  }
  return 1;
}

void MessageHandler::updateMessageHandler()
{
  char rawPacket[MAX_PACKET_LENGTH];
  char* packetPointer = rawPacket;
  while (running == true)
  {
    memset(rawPacket, 0, MAX_PACKET_LENGTH);
    int bytesRead = readPacket(packetPointer);
    if (bytesRead > 0)
    {
      MSG_HEADER header;
      memcpy(&header, packetPointer, sizeof(header));
      int msgType = header.msg_type;
      header.serial_no = getMsgNum();
      //header.msg_type = msgType;
      header.timestamp = getTimestamp();
      memcpy(packetPointer, &header, sizeof(header));
      sendPacket(packetPointer, bytesRead);
      if (msgType == SESSION_END) 
      {
        running = false;
      }
    }
  }
  closeInputSocket();
  closeOutputSocket();
}

void MessageHandler::updateDataHandler()
{
  char rawPacket[MAX_PACKET_LENGTH];
  char* packetPointer = rawPacket;
  while (running == true)
  {
    memset(rawPacket, 0, MAX_PACKET_LENGTH);
    int bytesRead = readData(packetPointer);
    if (bytesRead > 0)
    {
      MSG_HEADER header;
      memcpy(&header, packetPointer, sizeof(header));
      //int msgType = header.msg_type;
      header.serial_no = getMsgNum();
      //header.msg_type = msgType;
      header.timestamp = getTimestamp();
      memcpy(packetPointer, &header, sizeof(header));
      sendData(packetPointer, bytesRead);
    }
  }
  closeDataSockets();
}
int main()
{
  //TODO: Read Ports and IP address from config file
  MessageHandler* mh = new MessageHandler("127.0.0.1", 8080, 7000, 10000, 20000);
  mh->openInputSocket();
  mh->openOutputSocket();
  mh->openDataSockets();

  thread mhThread(&MessageHandler::updateMessageHandler, mh);
  thread dataThread(&MessageHandler::updateDataHandler, mh);

  mhThread.join();
  dataThread.join();
  
}
