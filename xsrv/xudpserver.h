/*
 * xsrv
 *
 * CopyLeft (c) 2008-~ sebastien.vincent@cppextrem.com Helight.Xu@gmail.com
 *
 * This source code is released for free distribution under the terms of the
 * the Free Software Foundation, version 3 of the License
 * GNU General Public License.
 * You should have received a copy of the GNU Lesser General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

 /*
 * file name: xudpserver.h
 * modify date: 2012-03-11
 * Author: Sebastien Vincent,  Helight Xu
 *
 * Program definitions: brief udp server.
 */

#ifndef XUDPSERVER_H
#define XUDPSERVER_H
#include <cstdio>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <string>
#include <list>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <jsonrpc/jsonrpc.h>
#include <jsonrpc/json.h>
#include <ev.h>
#include "commnet.h"

using namespace std;

/**
 * \class xudpserver
 * \brief Abstract JSON-RPC xudpserver.
 */
class xudpserver
{
public:
	/**
	 * \brief Constructor.
	 * \param address network address or FQDN to bind
	 * \param port local port to bind
	 */
	xudpserver(const std::string& address, uint16_t port);

	/**
	 * \brief Destructor.
	 */
	~xudpserver();

	bool Init();

	bool Start(json::rpc::Handler &handler);

	/**
	 * \brief Close listen socket and all client sockets.
	 */
	void Close();

	/**
	 * \brief Get socket descriptor.
	 * \return socket descriptor.
	 */
	int GetSocket() const;

	/**
	 * \brief Get the address.
	 * \return address or FQDN
	 */
	std::string GetAddress() const;

	/**
	 * \brief Get the port.
	 * \return local port
	 */
	uint16_t GetPort() const;

protected:
	/**
	 * \brief Socket descriptor.
	 */
	int m_sock;

	/**
	 * \brief JSON-RPC handler.
	 */
	//       Handler m_jsonHandler;

private:
	/**
	 * \brief Network address or FQDN.
	 */
	std::string m_address;

	/**
	 * \brief Local port.
	 */
	uint16_t m_port;

	struct client client;

	/**
	 * \brief List of client sockets.
	 */
	std::list<int> m_clients;

};

#endif /* XUDPSERVER_H */

