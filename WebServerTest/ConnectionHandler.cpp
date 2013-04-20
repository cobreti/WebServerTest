//
//  ConnectionHandler.cpp
//  WebServerTest
//
//  Created by Danny Thibaudeau on 2013-04-20.
//  Copyright (c) 2013 Danny Thibaudeau. All rights reserved.
//

#include "ConnectionHandler.hpp"
#include <NyxStreamRW.hpp>


CConnectionHandler::CConnectionHandler()
{
    
}


CConnectionHandler::~CConnectionHandler()
{
    
}

void CConnectionHandler::HandleStream( Nyx::IStreamRW& rStream )
{
    Nyx::CAString content;
    Nyx::CAString header;
    char buffer[4096];
    Nyx::NyxResult res;
    Nyx::NyxSize readSize = 0;
    Nyx::NyxSize writtenSize = 0;
    
    header =    "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html; charset=utf-8\r\n"
                "Content-Length: 82\r\n\r\n";
//                "Server: NyxWebServer\n";
    
    content =   "<html>\r\n"
                "<title>WebTraceSvr</title>\r\n"
                "<body>"
                "<h1>Web Trace Server</h1>\r\n"
                "</body>\r\n"
                "</html>\r\n";
    
    ::memset(buffer, 0, sizeof(buffer));
    
    res = rStream.Read(buffer, 4095, readSize);
    
    NYXTRACE(0x0, L"data received");
    
    if ( readSize > 0 )
    {
        NYXTRACE(0x0, Nyx::CTF_AnsiText(buffer) );
    }
    
    rStream.Write(header.BufferPtr(), header.length(), writtenSize);
    rStream.Write(content.BufferPtr(), content.length(), writtenSize );
}


Nyx::NyxResult CConnectionHandler::OnNewConnection( NyxNet::IConnection* pConnection, NyxNet::IConnectionHandler*& pCloneHandler )
{
    NYXTRACE(0x0, L"new connection");
    
    pCloneHandler = static_cast<NyxNet::IConnectionHandler*>(this);
    
    return Nyx::kNyxRes_Success;
}


void CConnectionHandler::OnConnectionTerminated( NyxNet::IConnection* pConnection )
{
    NYXTRACE(0x0, L"connection terminated");
}


