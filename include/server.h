#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/write.hpp>
#include <cstdio>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/asio/buffer.hpp>
#include <iostream>
#include <list>
#include "Commutator.h"

namespace ba = boost::asio;
using SocketPtr = std::shared_ptr<ba::ip::tcp::socket>;

class Client
{
	char data[512];
	SocketPtr mSocket;
	std::unique_ptr<Commutator> mCommutator;

public:
	Client(ba::io_service &io_service, int common_handler, int block_size) : mSocket{std::make_shared<ba::ip::tcp::socket>(io_service)},
                                                                           mCommutator{std::make_unique<Commutator>(common_handler, block_size)} {};
	void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred)
	{
		if (!ec)
		{
			std::string str(data, bytes_transferred);
			mCommutator->push(str);
		}
		ba::async_read(*mSocket, ba::buffer(data, 512), boost::bind(&Client::up_to_enter, this, ba::placeholders::error, ba::placeholders::bytes_transferred), boost::bind(&Client::read_handler, this, ba::placeholders::error, ba::placeholders::bytes_transferred));
	}

	SocketPtr getSocket()
	{
		return mSocket;
	}

	size_t up_to_enter(const boost::system::error_code &ec, size_t bytes)
	{
		if (!ec)
		{
			for (size_t i = 0; i < bytes; ++i)
				if ((data[i] == '\n') || (data[i] == ' '))
					return 0;
		}
		return 1;
	}
    void aread()
    {
		ba::async_read(*mSocket, ba::buffer(data, 512), boost::bind(&Client::up_to_enter, this, ba::placeholders::error, ba::placeholders::bytes_transferred), boost::bind(&Client::read_handler, this, ba::placeholders::error, ba::placeholders::bytes_transferred));
    }
};

class Server
{
	ba::io_service ioservice;
	ba::ip::tcp::endpoint ep;
	std::list<Client> mClients;
	int mBlockSize;
	int mCommonHandler;

public:
	Server(int common_handler, int block_size, int port) : mCommonHandler(common_handler), mBlockSize(block_size)
	{
		ep = ba::ip::tcp::endpoint(ba::ip::tcp::v4(), port); // listen on 2001
	}

	void start()
	{
		ba::ip::tcp::acceptor tcp_acceptor{ioservice, ep};

		tcp_acceptor.listen();
		mClients.push_back(Client(ioservice, mCommonHandler, mBlockSize));
		tcp_acceptor.async_accept(*mClients.back().getSocket(), boost::bind(&Server::accept_handler, this, std::ref(tcp_acceptor), std::ref(mClients.back()), boost::placeholders::_1));
		ioservice.run();
	}
private:
	void accept_handler(ba::ip::tcp::acceptor &tcp_acceptor, Client &client, const boost::system::error_code &ec)
	{
		if (!ec)
		{
		  client.aread();
		}
		mClients.push_back(Client(ioservice, mCommonHandler, mBlockSize));
		tcp_acceptor.async_accept(*mClients.back().getSocket(), boost::bind(&Server::accept_handler, this, std::ref(tcp_acceptor), std::ref(mClients.back()), boost::placeholders::_1));
	}
};
