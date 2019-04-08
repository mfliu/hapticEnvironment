#include "MessageHandler.h"
#include <typeinfo>

MessageHandler::MessageHandler(const char* address, int iPort, int oPort)
{
  ipAddr = address;
  inputPort = iPort;
  outputPort = oPort;
  startTime = high_resolution_clock::now();
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

  memset((char *) &inputStruct, 0, inputLen);
  inputStruct.sin_family = AF_INET;
  inputStruct.sin_port = htons(inputPort);
  if (inet_aton(ipAddr, &inputStruct.sin_addr) == 0)
  {
    cout << "inet_aton failed" << endl;
    exit(1);
  }

  int opt = 1;
  if (setsockopt(input_socket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)) < 0)
  {
    cout << "Failed to set broadcast option" << endl;
    exit(1);
  }
  if (setsockopt(input_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0)
  {
    cout << "Failed to set reuse address and port option" << endl;
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
  if (broadcast < 0) {
    cout << "Failed to set broadcast option" << endl;
    exit(1);
  }
  int reuseAddr = setsockopt(output_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  int reusePort = setsockopt(output_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
  if (reuseAddr < 0 || reusePort < 0) {
    cout << "Failed to set reuse address and port option" << endl;
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

int main()
{
  //TODO: Read Ports and IP address from config file
  MessageHandler* mh = new MessageHandler("127.0.0.1", 8080, 7000);
  mh->openInputSocket();
  mh->openOutputSocket();
  char rawPacket[MAX_PACKET_LENGTH];
  char* packetPointer = rawPacket;
  bool running = true;
  while (running == true)
  {
    memset(rawPacket, 0, MAX_PACKET_LENGTH);
    int bytesRead = mh->readPacket(packetPointer);
    if (bytesRead > 0)
    {
      MSG_HEADER header;
      memcpy(&header, packetPointer, sizeof(header));
      int msgType = header.msg_type;
      header.serial_no = mh->getMsgNum();
      header.timestamp = mh->getTimestamp();
      memcpy(packetPointer, &header, sizeof(header));
      mh->sendPacket(packetPointer, bytesRead);
      switch (msgType)
      {
        case SESSION_END:
        {
          running = false;
          break;
        }
      }
    }
  }
  mh->closeInputSocket();
  mh->closeOutputSocket();
}
