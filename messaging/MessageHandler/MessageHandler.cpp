#include "MessageHandler.h"
#include <typeinfo>
#include <fcntl.h>

MessageHandler::MessageHandler(const char* address, int port)
{
  srv = new rpc::server(address, port);
  startTime = high_resolution_clock::now();
}

rpc::server* MessageHandler::getServer()
{
  return srv;  
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
/*
int MessageHandler::sendMessage(char* const packet)
{
  memset(msg, 0, MAX_PACKET_LENGTH);
  memcpy(msg, packet, sizeof(&packet));
  return 1;
}

int MessageHandler::getMessage(char* result)
{
  memset(result, 0, MAX_PACKET_LENGTH);
  memcpy(result, msg, sizeof(&msg));
  return 1;
  
}
*/
int main()
{
  //TODO: Read Ports and IP address from config file
  MessageHandler* mh = new MessageHandler("127.0.0.1", 8080);
  mh->getServer()->bind("getMsgNum", [&mh](){return mh->getMsgNum();});
  mh->getServer()->bind("getTimestamp", [&mh](){return mh->getTimestamp();});
  //mh->getServer()->bind("sendMessage", [&mh]((char* const) packet){return mh->sendMessage(packet);});
  //mh->getServer()->bind("getMessage", [&mh](char* result){return mh->getMessage(result);});
  mh->getServer()->run();
}
