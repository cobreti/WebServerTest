//
//  ConnectionHandler.hpp
//  WebServerTest
//
//  Created by Danny Thibaudeau on 2013-04-20.
//  Copyright (c) 2013 Danny Thibaudeau. All rights reserved.
//

#ifndef WebServerTest_ConnectionHandler_hpp
#define WebServerTest_ConnectionHandler_hpp

#include <NyxNet.hpp>

DECLARE_OBJECTREF(CConnectionHandler)

class CConnectionHandler : public Nyx::CRefCount_Impl<NyxNet::IConnectionHandler>
{
public:
    CConnectionHandler();
    virtual ~CConnectionHandler();
    
public: // IConnectionHandler methods
    
    virtual void HandleStream( Nyx::IStreamRW& rStream );
    virtual Nyx::NyxResult OnNewConnection( NyxNet::IConnection* pConnection, NyxNet::IConnectionHandler*& pCloneHandler );
    virtual void OnConnectionTerminated( NyxNet::IConnection* pConnection );
};


#endif
