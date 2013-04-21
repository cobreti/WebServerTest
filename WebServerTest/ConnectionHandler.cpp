//
//  ConnectionHandler.cpp
//  WebServerTest
//
//  Created by Danny Thibaudeau on 2013-04-20.
//  Copyright (c) 2013 Danny Thibaudeau. All rights reserved.
//

#include <NyxNetConnection.hpp>

#include "ConnectionHandler.hpp"
#include "SSLConnectionHandler.hpp"
#include <NyxStreamRW.hpp>



CConnectionHandler::CConnectionHandler()
{
}


CConnectionHandler::~CConnectionHandler()
{
    
}

void CConnectionHandler::HandleStream( Nyx::IStreamRW& rStream )
{
}


Nyx::NyxResult CConnectionHandler::OnNewConnection( NyxNet::IConnection* pConnection, NyxNet::IConnectionHandler*& pCloneHandler )
{
    NYXTRACE(0x0, L"new connection");
        
    CSSLConnectionHandler* pNewConnection = new CSSLConnectionHandler(NULL, NULL, NULL, pConnection);
    
    pCloneHandler = static_cast<NyxNet::IConnectionHandler*>(pNewConnection);
    
    return Nyx::kNyxRes_Success;
}


void CConnectionHandler::OnConnectionTerminated( NyxNet::IConnection* pConnection )
{
    NYXTRACE(0x0, L"connection terminated");
//    delete this;
}


