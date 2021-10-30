#include <jm/client/client.h>
#include "crypto/functions.h"
#include <boost/asio.hpp>
#include "app.h"
#include <iostream>
#include <thread>

int main(int argc, char** argv)
{
    try
    {
        std::shared_ptr<jm::clientapp::App> app = std::make_shared<jm::clientapp::App>();
        
        std::thread networkCommunication{
        [app{app->shared_from_this()}] ()
            {
                boost::asio::io_context ioContext;
                jm::client::Client me{ ioContext, app };
                me.connect("127.0.0.1", "7");
                ioContext.run();
            }
        };
        networkCommunication.detach();
        app->setMessageForServer(jm::crypto::convertTextStringToBytes("Hello"));
        std::string response{ jm::crypto::convertBytesToTextString(app->getResponseFromServer()) };
        
        std::cout << response << std::endl;
        
        //boost::asio::io_context ioContext;
        //jm::client::Client me{ ioContext, app };
        //me.connect("127.0.0.1", "7");
        //ioContext.run();
    }
    catch (std::exception exception)
    {
        std::cerr << exception.what() << std::endl;
    }

    system("pause");

    return 0;
}