#include "Logger.h"

Logger::Logger(const char* address, int logPort, int dataPort, const char* logDirectory, const char* logFilePrefix)
{
  ipAddr = address;
  port = logPort;
  data_port = dataPort;
  saveDir = logDirectory;
  saveFile = logFilePrefix;
  running = true;

  time_t now;
  time(&now);
  struct tm* timeinfo;
  timeinfo = localtime(&now);
  char buffer[80];
  strftime(buffer, 80, "%F-%H-%M-%S", timeinfo);
  string divider = "/";
  string dash = "-";
  string fullFile = saveDir + divider + saveFile + dash + buffer + "_messages.log";
  logFile.open(fullFile, fstream::binary | fstream::out);
  if (logFile.is_open())
  {
    cout << fullFile << " opened" << endl;
  }

  string fullDataFile = saveDir + divider + saveFile + dash + buffer + "_hapticData.log";
  logDataFile.open(fullDataFile, fstream::binary | fstream ::out);
  if (logDataFile.is_open())
  {
    cout << fullDataFile << " opened" << endl;
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

  cout << "Opening data listening socket..." << endl;
  data_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (data_socket < 0)
  {
    cout << "Failed to open data listening socket" << endl;
    exit(1);
  }

  memset((char *) &dataStruct, 0, dataLen);
  dataStruct.sin_family = AF_INET;
  dataStruct.sin_port = htons(port);
  if (inet_aton(ipAddr, &dataStruct.sin_addr) == 0)
  {
    cout << "inet_aton failed" << endl;
    exit(1);
  }
  
  broadcast = setsockopt(data_socket, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
  reuseAddr = setsockopt(data_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  reusePort = setsockopt(data_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
  if (broadcast < 0 || reuseAddr < 0 || reusePort < 0)
  {
    cout << "Failed to set socket options" << endl;
    exit(1);
  }

  int bind_loggerData = bind(data_socket, (struct sockaddr *) &dataStruct, dataLen);
  if (bind_loggerData < 0)
  {
    cout << "Error binding data listener socket" << endl;
  }
}

void Logger::updateLogger()
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
  stopMsgLogger();
}

void Logger::updateDataLogger()
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
      int msgType = header.msg_type;
      char writePacket[bytesRead];
      char* writePtr = writePacket;
      memcpy(writePtr, packetPointer, sizeof(writePacket));
      if (logDataFile.is_open()) 
      {
        logDataFile.write(writePacket, bytesRead);
      }
      else {
        cout << "File closed" << endl;
      }
    }
  }
  stopDataLogger();

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

int Logger::readData(char* packetPointer)
{
  int value = 0, bytesRead = 0;
  ioctl(data_socket, FIONREAD, &value);
  if (value > 0) {
    bytesRead = recvfrom(data_socket, packetPointer, MAX_PACKET_LENGTH, 0, (struct sockaddr*) &dataStruct, (socklen_t*) &dataLen);
  }
  return bytesRead;
}

void Logger::stopMsgLogger()
{
  close(listener_socket);
  logFile.close();
}

void Logger::stopDataLogger()
{
  close(data_socket);
  logDataFile.close();
}
int main()
{
  Logger* logger = new Logger("127.0.0.1", 7000, 20000, "/home/mfl24/data/RnelShare/users/mfl24/Test/Data/", "LogFile");
  logger->startLogger();
  thread loggingThread(&Logger::updateLogger, logger);
  thread dataLogging(&Logger::updateDataLogger, logger);

  loggingThread.join();
  dataLogging.join();
}
