//
//  main.cpp
//  WebServerTest
//
//  Created by Danny Thibaudeau on 2013-04-20.
//  Copyright (c) 2013 Danny Thibaudeau. All rights reserved.
//

#include <iostream>

#include <Nyx.hpp>
#include <NyxNet.hpp>

#include "ConnectionHandler.hpp"

int main(int argc, const char * argv[])
{
    Nyx::CModuleRef             refModule = Nyx::CModule::Alloc();
    NyxNet::CModuleRef          refNetModule = NyxNet::CModule::Alloc();
    Nyx::CTraceCompositorRef    refTraceCompositor = Nyx::CTraceCompositor::Alloc();
    
    refTraceCompositor->SetOutput( NyxNet::CPipeTraceOutput::Alloc("WebServerTest") );
    
    NYXTRACE(0x0, L"starting" );

    
    NyxNet::CTcpIpServerRef     refServer = NyxNet::CTcpIpServer::Alloc();
    CConnectionHandlerRef       refConnectionHandler = new CConnectionHandler();
    
    refServer->Create(8000, 100, refConnectionHandler);
    refServer->Start();
    
    getchar();
    
    refServer->Stop();
    
    NYXTRACE(0x0, L"ending" );
    
    return 0;
}

