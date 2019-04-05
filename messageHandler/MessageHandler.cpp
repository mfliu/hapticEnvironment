#include "MessageHandler.h"
#include <typeinfo>

MessageHandler::MessageHandler(const string& address, uint16_t port)
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

int main()
{
  MessageHandler* mh = new MessageHandler("127.0.0.1", 8080);
  mh->getServer()->bind("getMsgNum", [&mh](){return mh->getMsgNum();});
  mh->getServer()->bind("getTimestamp", [&mh](){return mh->getTimestamp();});
  mh->getServer()->run();
}
