#include <iostream>
#include "CommandReader.h"
#include "AtomicBlockQueue.h"
#include "FileOutputThread.h"
#include "async.h"
#include <thread>
#include <chrono>
#include <server.h>
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cout<<"bulk_server <port> <bulk_size>"<<std::endl;
        return -1;
    }

    int port = std::atoi(argv[1]);
    int bulk_size = std::atoi(argv[2]);
    int desc = connect_bulk(bulk_size);
    
    Server server(desc, bulk_size, port);
    server.start();
    disconnect(desc);
    return 0;
}