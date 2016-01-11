////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2014-2016 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Jan Steemann
////////////////////////////////////////////////////////////////////////////////

#ifndef LIB_REST_ENDPOINT_H
#define LIB_REST_ENDPOINT_H 1

#include "Basics/socket-utils.h"

#include "Basics/Common.h"
#include "Basics/StringUtils.h"

#ifdef TRI_HAVE_LINUX_SOCKETS
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/file.h>
#endif

#ifdef TRI_HAVE_WINSOCK2_H
#include <WinSock2.h>
#include <WS2tcpip.h>
#endif


namespace triagens {
namespace rest {

////////////////////////////////////////////////////////////////////////////////
/// @brief endpoint specification
////////////////////////////////////////////////////////////////////////////////

class Endpoint {
  
 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief endpoint types
  //////////////////////////////////////////////////////////////////////////////

  enum EndpointType { ENDPOINT_SERVER, ENDPOINT_CLIENT };

  //////////////////////////////////////////////////////////////////////////////
  /// @brief endpoint types
  //////////////////////////////////////////////////////////////////////////////

  enum DomainType { DOMAIN_UNKNOWN = 0, DOMAIN_UNIX, DOMAIN_IPV4, DOMAIN_IPV6 };

  //////////////////////////////////////////////////////////////////////////////
  /// @brief encryption used when talking to endpoint
  //////////////////////////////////////////////////////////////////////////////

  enum EncryptionType { ENCRYPTION_NONE = 0, ENCRYPTION_SSL };

  
 protected:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief creates an endpoint
  //////////////////////////////////////////////////////////////////////////////

  Endpoint(const EndpointType, const DomainType, const EncryptionType,
           std::string const&, int);

 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief destroys an endpoint
  //////////////////////////////////////////////////////////////////////////////

  virtual ~Endpoint();

  
 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief return the endpoint specification in a unified form
  //////////////////////////////////////////////////////////////////////////////

  static std::string getUnifiedForm(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief creates a server endpoint from a string value
  //////////////////////////////////////////////////////////////////////////////

  static Endpoint* serverFactory(std::string const&, int, bool reuseAddress);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief creates a client endpoint from a string value
  //////////////////////////////////////////////////////////////////////////////

  static Endpoint* clientFactory(std::string const&);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief creates an endpoint from a string value
  //////////////////////////////////////////////////////////////////////////////

  static Endpoint* factory(const EndpointType type, std::string const&, int,
                           bool);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief compare two endpoints
  //////////////////////////////////////////////////////////////////////////////

  bool operator==(Endpoint const&) const;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief return the default endpoint
  //////////////////////////////////////////////////////////////////////////////

  static std::string const getDefaultEndpoint();

  //////////////////////////////////////////////////////////////////////////////
  /// @brief connect the endpoint
  //////////////////////////////////////////////////////////////////////////////

  virtual TRI_socket_t connect(double, double) = 0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief disconnect the endpoint
  //////////////////////////////////////////////////////////////////////////////

  virtual void disconnect() = 0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief init an incoming connection
  //////////////////////////////////////////////////////////////////////////////

  virtual bool initIncoming(TRI_socket_t) = 0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief set socket timeout
  //////////////////////////////////////////////////////////////////////////////

  virtual bool setTimeout(TRI_socket_t, double);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief initialize socket flags
  //////////////////////////////////////////////////////////////////////////////

  virtual bool setSocketFlags(TRI_socket_t);

  //////////////////////////////////////////////////////////////////////////////
  /// @brief return whether the endpoint is connected
  //////////////////////////////////////////////////////////////////////////////

  bool isConnected() const { return _connected; }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get the type of an endpoint
  //////////////////////////////////////////////////////////////////////////////

  EndpointType getType() const { return _type; }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get the domain type of an endpoint
  //////////////////////////////////////////////////////////////////////////////

  DomainType getDomainType() const { return _domainType; }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get the encryption used
  //////////////////////////////////////////////////////////////////////////////

  EncryptionType getEncryption() const { return _encryption; }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get the original endpoint specification
  //////////////////////////////////////////////////////////////////////////////

  std::string getSpecification() const { return _specification; }

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get endpoint domain
  //////////////////////////////////////////////////////////////////////////////

  virtual int getDomain() const = 0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get port number
  //////////////////////////////////////////////////////////////////////////////

  virtual int getPort() const = 0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get host name
  //////////////////////////////////////////////////////////////////////////////

  virtual std::string getHost() const = 0;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief get address
  //////////////////////////////////////////////////////////////////////////////

  virtual std::string getHostString() const = 0;

  
 public:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief error message if failure occurred
  //////////////////////////////////////////////////////////////////////////////

  std::string _errorMessage;

  
 protected:
  //////////////////////////////////////////////////////////////////////////////
  /// @brief whether or not the endpoint is connected
  //////////////////////////////////////////////////////////////////////////////

  bool _connected;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief the actual socket
  //////////////////////////////////////////////////////////////////////////////

  TRI_socket_t _socket;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief endpoint type
  //////////////////////////////////////////////////////////////////////////////

  EndpointType _type;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief endpoint domain type
  //////////////////////////////////////////////////////////////////////////////

  DomainType _domainType;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief encryption used
  //////////////////////////////////////////////////////////////////////////////

  EncryptionType _encryption;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief original endpoint specification
  //////////////////////////////////////////////////////////////////////////////

  std::string _specification;

  //////////////////////////////////////////////////////////////////////////////
  /// @brief listen backlog size
  //////////////////////////////////////////////////////////////////////////////

  int _listenBacklog;
};
}
}

#endif


