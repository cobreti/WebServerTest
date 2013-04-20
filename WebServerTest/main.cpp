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

int main(int argc, const char * argv[])
{
    Nyx::CModuleRef             refModule = Nyx::CModule::Alloc();
    NyxNet::CModuleRef          refNetModule = NyxNet::CModule::Alloc();
    Nyx::CTraceCompositorRef    refTraceCompositor = Nyx::CTraceCompositor::Alloc();
    
    refTraceCompositor->SetOutput( NyxNet::CPipeTraceOutput::Alloc("WebServerTest") );
    
    NYXTRACE(0x0, L"starting" );

    // insert code here...
    std::cout << "Hello, World!\n";
    
    
    getchar();
    
    NYXTRACE(0x0, L"ending" );
    
    return 0;
}

