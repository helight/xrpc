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
 * file name: commnet.h
 * modify date: 2012-03-11
 * Author: Sebastien Vincent,  Helight Xu
 *
 * Program definitions: brief net api.
 */

#ifndef XCOMMNET_H
#define XCOMMNET_H
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
#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include "ev.h"
#include "jsonrpc.h"
#include "json.h"

using namespace std;

struct client {
	int fd;
	ev_io ev_write;
	ev_io ev_read;
	struct sockaddr_in client_addr;
	struct in_addr	ipaddr;			/*client ip*/
	char rbuff[1024];
	json::Value response;
	json::Value recv;
	json::rpc::Handler *handler;
};

void InitDaemon();
int setnonblock(int fd);
void SigHandler(int signum);

/**
 * \brief Accept a new client socket.
 */
void Accept(struct ev_loop *loop, struct ev_io *w, int revents);
void udp_recv(struct ev_loop *loop, struct ev_io *w, int revents);

#endif /* XCOMMNET_H */

