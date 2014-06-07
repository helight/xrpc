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
 * file name: xudpserver.cc  
 * modify date: 2012-03-28
 * Author: Helight Xu
 *
 * Program definitions: brief udp server.
 */


#include "xudpserver.h"
extern json::rpc::Handler *xhandler;

xudpserver::xudpserver(const std::string& address, uint16_t port)
{
	m_sock = -1;
	m_address = address;
	m_port = port;
	// SetEncapsulatedFormat(Json::Rpc::RAW);
}

xudpserver::~xudpserver()
{
	if (m_sock != -1) {
		Close();
	}
}

bool xudpserver::Init()
{
	if (m_sock > 0)
		return true;
	int sock;
	struct sockaddr_in listen_addr; 
	int reuseaddr_on = 1;

	sock = socket(AF_INET, SOCK_DGRAM, 0); 
	if (sock < 0)
		err(1, "listen failed");
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_on,
				sizeof(reuseaddr_on)) == -1)
		err(1, "setsockopt failed");
	memset(&listen_addr, 0, sizeof(listen_addr));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_addr.s_addr = inet_addr(m_address.c_str());
	listen_addr.sin_port = htons(m_port);

	if (::bind(sock, (struct sockaddr *)&listen_addr, sizeof(listen_addr)) < 0)
		err(1, "bind failed");

	if (sock < -1) {
		return false;
	}

	m_sock = sock;

	return (m_sock != -1) ? true : false;
}


bool xudpserver::Start(json::rpc::Handler &handler)
{
	ev_io ev_accept;
	struct ev_loop *loop;

	if (!Init())
		err(1, "init failed");
#ifndef DEBUG
	InitDaemon();
#endif
	xhandler = &handler;

	loop = ev_default_loop (0);
	ev_io_init(&ev_accept, udp_recv, m_sock, EV_READ);
	ev_io_start(loop,&ev_accept);
	ev_loop (loop, 0);
}

int xudpserver::GetSocket() const
{
	return m_sock;
}

std::string xudpserver::GetAddress() const
{
	return m_address;
}

uint16_t xudpserver::GetPort() const
{
	return m_port;
}

void xudpserver::Close()
{
	::close(m_sock);
	m_sock = -1;
} 

