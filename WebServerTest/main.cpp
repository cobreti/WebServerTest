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

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "ConnectionHandler.hpp"

int main(int argc, const char * argv[])
{
    Nyx::CModuleRef             refModule = Nyx::CModule::Alloc();
    NyxNet::CModuleRef          refNetModule = NyxNet::CModule::Alloc();
    Nyx::CTraceCompositorRef    refTraceCompositor = Nyx::CTraceCompositor::Alloc();
    
    
    refTraceCompositor->SetOutput( NyxNet::CPipeTraceOutput::Alloc("WebServerTest") );
    
    NYXTRACE(0x0, L"starting" );

    CRYPTO_malloc_init(); // Initialize malloc, free, etc for OpenSSL's use
    SSL_library_init(); // Initialize OpenSSL's SSL libraries
    SSL_load_error_strings(); // Load SSL error strings
    ERR_load_BIO_strings(); // Load BIO error strings
    OpenSSL_add_all_algorithms(); // Load all available encryption algorithms
    
    


    
    NyxNet::CTcpIpServerRef     refServer = NyxNet::CTcpIpServer::Alloc();
    CConnectionHandlerRef       refConnectionHandler = new CConnectionHandler();
    
    refServer->Create(8000, 100, refConnectionHandler);
    refServer->Start();
    
    getchar();
    
    refServer->Stop();
    
    NYXTRACE(0x0, L"ending" );
    
    return 0;
}

