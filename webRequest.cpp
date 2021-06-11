#include "webRequest.h"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>

std::string getHTML(const std::string& server, const std::string& path) {
    boost::system::error_code ec;
    using namespace boost::asio;

    io_service svc;
    ssl::context ctx( ssl::context::method::sslv23_client);
    ssl::stream<ip::tcp::socket> ssock(svc, ctx);

    ip::tcp::resolver resolver(svc);
    auto endpoint = resolver.resolve(server, "https");
    connect(ssock.lowest_layer(), endpoint);

    ssock.handshake(ssl::stream_base::handshake_type::client);

    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream << "GET " << path << " HTTP/1.1\r\n";
    request_stream << "Host: " << server << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    boost::asio::write(ssock, request);

    std::string response;
    do {
        char buf[1024];
        size_t bytes_transferred = ssock.read_some(buffer(buf), ec);
        if (!ec) response.append(buf, buf + bytes_transferred);
    } while (!ec);

    return response;
}
