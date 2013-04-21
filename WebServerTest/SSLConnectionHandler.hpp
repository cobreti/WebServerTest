//
//  ConnectionHandler.hpp
//  WebServerTest
//
//  Created by Danny Thibaudeau on 2013-04-20.
//  Copyright (c) 2013 Danny Thibaudeau. All rights reserved.
//

#ifndef WebServerTest_SSLConnectionHandler_hpp
#define WebServerTest_SSLConnectionHandler_hpp

#include <NyxNet.hpp>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <map>

DECLARE_OBJECTREF(CSSLConnectionHandler)

class CSSLConnectionHandler : public Nyx::CRefCount_Impl<NyxNet::IConnectionHandler>
{
public:
    CSSLConnectionHandler(SSL_CTX* ctx, SSL* ssl, BIO* bio, NyxNet::IConnection* pConnection);
    virtual ~CSSLConnectionHandler();
    
public: // IConnectionHandler methods
    
    virtual void HandleStream( Nyx::IStreamRW& rStream );
    virtual Nyx::NyxResult OnNewConnection( NyxNet::IConnection* pConnection, NyxNet::IConnectionHandler*& pCloneHandler );
    virtual void OnConnectionTerminated( NyxNet::IConnection* pConnection );
    virtual void CloseConnection( NyxNet::IConnection* pConnection );
    
private:
    
//    typedef     std::map<int, CConnectionHandler*>      TOpenedConnectionsTable;
    
private:
    
    SSL_CTX*                    m_ctx;
    SSL*                        m_ssl;
    BIO*                        m_bio;
    NyxNet::IConnection*        m_pConnection;
    bool                        m_bRunning;
    
//    TOpenedConnectionsTable     m_OpenedConnectionsTable;
};


#endif
