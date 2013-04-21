//
//  ConnectionHandler.cpp
//  WebServerTest
//
//  Created by Danny Thibaudeau on 2013-04-20.
//  Copyright (c) 2013 Danny Thibaudeau. All rights reserved.
//

#include <NyxNetConnection.hpp>

#include "SSLConnectionHandler.hpp"
#include <NyxStreamRW.hpp>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>


int password_callback(char *buf, int size, int rwflag, void *password)
{
    strncpy(buf, "NyxWeb", size);
    buf[size - 1] = '\0';
    return(strlen(buf));
}


CSSLConnectionHandler::CSSLConnectionHandler(SSL_CTX* ctx, SSL* ssl, BIO* bio, NyxNet::IConnection* pConnection) :
    m_ctx(ctx),
    m_ssl(ssl),
    m_bio(bio),
    m_pConnection(pConnection),
    m_bRunning(true)
{
    int res = 0;
    
    if ( ctx == NULL && ssl == NULL )
    {
        m_ctx = SSL_CTX_new(SSLv23_server_method());
        
        // Sets the default certificate password callback function. Read more under the Certificate Verification section.
        SSL_CTX_set_default_passwd_cb(m_ctx, password_callback);
        // Sets the certificate file to be used.
        res = SSL_CTX_use_certificate_file(m_ctx, "/Users/dannyt/dev/WebServerTest/cert.pem", SSL_FILETYPE_PEM);
        // Sets the private key file to be used.
        res = SSL_CTX_use_PrivateKey_file(m_ctx, "/Users/dannyt/dev/WebServerTest/privkey.pem", SSL_FILETYPE_PEM);
        
        // Load trusted root authorities
//        res = SSL_CTX_load_verify_locations(m_ctx, "/Users/dannyt/dev/WebServerTest/rootCA.pem", 0);
        // Set the maximum depth to be used verifying certificates
        // Due to a bug, this is not enforced. The verify callback must enforce it.
//        SSL_CTX_set_verify_depth(m_ctx, 1);
        // Set the certificate verification callback.
        //    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER | SSL_VERIFY_FAIL_IF_NO_PEER_CERT, verify_callback);
        
        // Load the random file, read 1024 << 10 bytes, add to PRNG for entropy
        //    RAND_load_file("random.pem", 1024 << 10);
        
        
        // We need to load the Diffie-Hellman key exchange parameters.
        // First load dh1024.pem (you DID create it, didn't you?)
        BIO* bio = BIO_new_file("/Users/dannyt/dev/WebServerTest/dh1024.pem", "r");
        // Did we get a handle to the file?
        //    if (bio == NULL) {
        //        printf("Couldn't open DH param file!\n");
        //        break;
        //    }
        
        // Read in the DH params.
        DH* ret = PEM_read_bio_DHparams(bio, NULL, NULL, NULL);
        // Free up the BIO object.
        BIO_free(bio);
        // Set up our SSL_CTX to use the DH parameters.
        if (SSL_CTX_set_tmp_dh(m_ctx, ret) < 0) {
            printf("Couldn't set DH parameters!\n");
            //        break;
        }
        
        // Now we need to generate a RSA key for use.
        // 1024-bit key. If you want to use something stronger, go ahead but it must be a power of 2. Upper limit should be 4096.
        RSA* rsa = RSA_generate_key(1024, RSA_F4, NULL, NULL);
        
        // Set up our SSL_CTX to use the generated RSA key.
        if (!SSL_CTX_set_tmp_rsa(m_ctx, rsa)) {
            printf("Couldn't set RSA key!\n");
            // We don't break out here because it's not a requirement for the RSA key to be set. It does help to have it.
        }
        // Free up the RSA structure.
        RSA_free(rsa);
        
        SSL_CTX_set_cipher_list(m_ctx, "ALL");
        // Set up our SSL object as before
        m_ssl = SSL_new(m_ctx);
        // Set up our BIO object to use the client socket
        
        NyxNet::CSocketRef refSocket = pConnection->Socket();
        NyxNet::CTcpIpSocket*   pTcpIpSocket = refSocket->TcpIpSocket();
        int socketid = pTcpIpSocket->TcpIpSocketId();
        
        m_bio = BIO_new_socket(socketid, BIO_NOCLOSE);
        BIO_set_nbio(m_bio, 0);
        SSL_set_bio(m_ssl, m_bio, m_bio);
        
        int r = SSL_accept(m_ssl);
        if ( r != 1 )
        {
            NYXTRACE(0x0, L"SSL accept failure");
        }
        else
        {
            NYXTRACE(0x0, L"SSL accept success");
        }
        
        if ( SSL_get_verify_result(m_ssl) != X509_V_OK)
        {
            NYXTRACE(0x0, L"ssl verify result failure");
        }
    }
}


CSSLConnectionHandler::~CSSLConnectionHandler()
{
    
}

void CSSLConnectionHandler::HandleStream( Nyx::IStreamRW& rStream )
{
//    NyxNet::CSocketRef refSocket = m_pConnection->Socket();
//    int socketid = refSocket->tcpsocket();
   
    while (m_bRunning)
    {
        Nyx::CAString content;
        Nyx::CAString header;
        char buffer[4096];
    //    Nyx::NyxResult res;
        int readSize = 0;
    //    Nyx::NyxSize writtenSize = 0;
        
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
        
        //    res = rStream.Read(buffer, 4095, readSize);
        readSize = SSL_read(m_ssl, buffer, 4095);
        if ( readSize > 0 )
        {
    //            readSize = BIO_read(m_bio, buffer, 4095);
            
            
            NYXTRACE(0x0, L"data received - readsize : " << readSize );
            
        //    if ( readSize == 0 )
        //    {
        //        int errorCode = SSL_get_error(m_ssl, readSize);
        //        
        //        NYXTRACE(0x0, L"ssl error code: " << ERR_error_string(errorCode, NULL) );
        //    }
            
        //    if ( readSize > 0 )
            {
                NYXTRACE(0x0, Nyx::CTF_AnsiText(buffer) );
            }
            
            SSL_write(m_ssl, header.BufferPtr(), header.length());
            SSL_write(m_ssl, content.BufferPtr(), content.length());
        //    rStream.Write(header.BufferPtr(), header.length(), writtenSize);
        //    rStream.Write(content.BufferPtr(), content.length(), writtenSize );
        }
        else
            sleep(1);
    }
    
    NYXTRACE(0x0, L"ending SSLConnection stream handler");
}


Nyx::NyxResult CSSLConnectionHandler::OnNewConnection( NyxNet::IConnection* pConnection, NyxNet::IConnectionHandler*& pCloneHandler )
{
//    NYXTRACE(0x0, L"new ssl connection");
    
    NyxNet::CSocketRef refSocket = pConnection->Socket();
    NyxNet::CTcpIpSocket*   pTcpIpSocket = refSocket->TcpIpSocket();
    int socketid = pTcpIpSocket->TcpIpSocketId();
    
    NYXTRACE(0x0, L"new ssl connection from "
             << Nyx::CTF_AnsiText(pTcpIpSocket->ClientAddress().Ip().c_str())
             << L" port "
             << Nyx::CTF_Int(pTcpIpSocket->ClientAddress().Port()) );
    
    BIO* sslclient = BIO_new_socket(socketid, BIO_NOCLOSE);
    BIO_set_nbio(sslclient, 0);
    SSL_set_bio(m_ssl, sslclient, sslclient);
    
    int r = SSL_accept(m_ssl);
    if ( r != 1 )
    {
        NYXTRACE(0x0, L"SSL accept failure");
    }
    else
    {
        NYXTRACE(0x0, L"SSL accept success");
    }
    
    if ( SSL_get_verify_result(m_ssl) != X509_V_OK)
    {
        NYXTRACE(0x0, L"ssl verify result failure");
    }

    CSSLConnectionHandler* pNewConnection = new CSSLConnectionHandler(m_ctx, m_ssl, m_bio, pConnection);
    
    pCloneHandler = static_cast<NyxNet::IConnectionHandler*>(pNewConnection);
        
    return Nyx::kNyxRes_Success;
}


void CSSLConnectionHandler::OnConnectionTerminated( NyxNet::IConnection* pConnection )
{
    NYXTRACE(0x0, L"ssl connection terminated");
    delete this;
}


void CSSLConnectionHandler::CloseConnection( NyxNet::IConnection* pConnection )
{
    NYXTRACE(0x0, L"Closing connection");
    m_bRunning = false;
}
