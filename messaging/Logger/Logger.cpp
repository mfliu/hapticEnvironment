#include "Logger.h"

Logger::Logger(const char* address, int logPort, const char* logDirectory, const char* logFilePrefix)
{
  ipAddr = address;
  port = logPort;
  saveDir = logDirectory;
  saveFile = logFilePrefix;
  time_t now;
  time(&now);
  struct tm* timeinfo;
  timeinfo = localtime(&now);
  char buffer[80];
  strftime(buffer, 80, "%F-%H-%M-%S", timeinfo);
  string divider = "/";
  string dash = "-";
  string fullFile = saveDir + divider + saveFile + dash + buffer + ".log";
  logFile.open(fullFile, fstream::binary | fstream::out | fstream::app);
  if (logFile.is_open())
  {
    cout << fullFile << " opened" << endl;
  }
}

void Logger::startLogger()
{
  cout << "Opening listening socket..." << endl;
  listener_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (listener_socket < 0)
  {
    cout << "Failed to open listening socket" << endl;
    exit(1);
  }

  memset((char *) &listenerStruct, 0, listenerLen);
  listenerStruct.sin_family = AF_INET;
  listenerStruct.sin_port = htons(port);
  if (inet_aton(ipAddr, &listenerStruct.sin_addr) == 0)
  {
    cout << "inet_aton failed" << endl;
    exit(1);
  }
  
  int opt = 1;
  int broadcast = setsockopt(listener_socket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
  int reuseAddr = setsockopt(listener_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  int reusePort = setsockopt(listener_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
  if (broadcast < 0 || reuseAddr < 0 || reusePort < 0)
  {
    cout << "Failed to set socket options" << endl;
    exit(1);
  }

  int bind_logger = bind(listener_socket, (struct sockaddr *) &listenerStruct, listenerLen);
  if (bind_logger < 0)
  {
    cout << "Error binding listener socket" << endl;
  }
}

void Logger::updateLogger()
{
  char rawPacket[MAX_PACKET_LENGTH];
  char* packetPointer = rawPacket;
  bool running = true;

  while (running == true)
  {
    memset(rawPacket, 0, MAX_PACKET_LENGTH);
    int bytesRead = readPacket(packetPointer);
    if (bytesRead > 0)
    {
      MSG_HEADER header;
      memcpy(&header, packetPointer, sizeof(header));
      int msgType = header.msg_type;
      char writePacket[bytesRead];
      char* writePtr = writePacket;
      memcpy(writePtr, packetPointer, sizeof(writePacket));
      if (logFile.is_open()) 
      {
        logFile.write(writePacket, bytesRead);
      }
      else {
        cout << "File closed" << endl;
      }
      if (msgType == SESSION_END)
      {
        running = false;
      }
    }
  }
  stopLogger();
}

int Logger::readPacket(char* packetPointer)
{
  int value = 0, bytesRead = 0;
  ioctl(listener_socket, FIONREAD, &value);
  if (value > 0) {
    bytesRead = recvfrom(listener_socket, packetPointer, MAX_PACKET_LENGTH, 0, (struct sockaddr*) &listenerStruct, (socklen_t*) &listenerLen);
  }
  return bytesRead;
}

void Logger::stopLogger()
{
  close(listener_socket);
  logFile.close();
}

int main()
{
  Logger* logger = new Logger("127.0.0.1", 7000, "/home/mfl24/data/RnelShare/users/mfl24/Test/Data/Logger/", "LogFile");
  logger->startLogger();
  logger->updateLogger();
}
