/**
 * <b>File:</b> NetCalcApp.c
 *
 * <b>Project:</b> Remote Calculator Demo
 *
 * <b>Description:</b> Implement the server side of a simple network application.
 * A client connects itself to the server over a TCP connection on the port 2000.
 * After the connection is established, the client can send a request to the
 * server.<p/>
 * A request is encoded as a string with the following syntax:<br/>
 * <code>operator[,operand1,operand2]</code><br/>
 * operand1 and operand2 are integer, and operator is one of<br/>
 * <dl>
 * <dt><b>.</b> - close the connection.</dt>
 * <dt><b>+,-,*,/</b> - are the standard arithmetic operators.</dt>
 * The application parses the request and send back the result to the client.
 *
 * <b>Cereated:</b> 27/11/2007
 *
 * <dl>
 * <dt><b>Autor</b>:</dt>
 * <dd>Stefano Oliveri</dd>
 * <dt><b>E-mail:</b></dt>
 * <dd>stefano.oliveri@st.com</dd>
 * </dl>
 */

// Standard includes.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Scheduler includes.
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Demo includes.
#include "lwipNetCalc/NetCalcApp.h"

// lwIP includes.
#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "lwip/memp.h"

#define RC_PORT 2000
#define RC_ESC	"."
#define RC_OUT_BUFF_SIZE 255

#define RC_STATUS_CLOSE 0x1
#define RC_STATUS_ERROR 0x2

#define RC_SET_STATUS_FLAG(status, flag) ((status) |= (flag))
#define RC_RESET_STATUS_FLAG(status, flag) ((status) &= (~(flag)))
#define RC_IS_STATUS_FLAG_ASSERTED(status, flag) (((status) & (flag)) != 0)

/**
 * This buffer is used to store  and parse the incoming request, and than to
 * build the message to send back to the client.
 */
static char s_out[RC_OUT_BUFF_SIZE];


static void ProcessConnection(struct netconn *conn);

/**
 * The task control loop.
 *
 * @param pvParameters not used.
 */
void NetworkCalculatorLoop( void *pvParameters )
{
	struct netconn *pListener, *pNewConnection;

	// Create a new tcp connection handle
	pListener = netconn_new( NETCONN_TCP );
	netconn_bind(pListener, NULL, RC_PORT );
	netconn_listen( pListener );

	// Loop forever
	for( ;; )
	{
		// Wait for a first connection.
		err_t res = netconn_accept(pListener, &pNewConnection);

		if(res == pNewConnection)
		{

#ifdef DEBUG
			printf("RC:new conn.\n");
#endif
			// Process the incoming connection.
			ProcessConnection(pNewConnection);

			netconn_close( pNewConnection );
			netconn_delete( pNewConnection );
		}
	}
}

/**
 * Process the incoming connection.
 *
 * @param conn Input. The netconn to use to send and receive data.
 */
void ProcessConnection(struct netconn *conn)
{
	struct netbuf *in = NULL;
	unsigned char cStatus = 0;
	err_t res;

	while ( !RC_IS_STATUS_FLAG_ASSERTED(cStatus, RC_STATUS_CLOSE|RC_STATUS_ERROR) ) {
		// Read data from the connection into the netbuf in
		res = netconn_recv(conn, &in);
		if ( res == ERR_OK ) { // in will be NULL when connection on other side is close

			char *pBuff = s_out;
			int nSize = netbuf_len(in);

			// Copy the data from the netbuf chain to the application buffer.
			netbuf_copy(in, pBuff, RC_OUT_BUFF_SIZE);

			// Parse the command: <operator>[,<operand1>,<operand2>]
				int i=0;
				for (i=0; i<nSize; ++i) {
				if ( pBuff[i] == ',' ||
						 pBuff[i] == '\r' ||
						 pBuff[i] == '\n' ) {
					pBuff[i] = '\0';
				}
			}

			if( !strncmp(pBuff, RC_ESC, sizeof(RC_ESC)) ) {
				RC_SET_STATUS_FLAG(cStatus, RC_STATUS_CLOSE);
				netbuf_delete(in);

#ifdef DEBUG
				printf("RC:close.\n");
#endif

				continue;
			}

			if ( (nSize < 6 + strlen(RC_ESC)) || (nSize > RC_OUT_BUFF_SIZE) ) {
				// Invalid command. Set error status and exit.
				RC_SET_STATUS_FLAG(cStatus, RC_STATUS_ERROR);
				continue;
			}

			// Extract both operands
			int a = atoi(&pBuff[2]);
			int b = atoi(&pBuff[3 + (strlen(&pBuff[2]))]);

			int res = 0;
			switch (pBuff[0]) { // check the operator
			case '/':
				res = b ? a / b : 0;
				break;
			case '*':
				res = a * b;
				break;
			case '-':
				res = a - b;
				break;
			case '+':
				res = a + b;
				break;
			}

			// Send the result back.
			int nResSize = sprintf(pBuff, "%d\n", res);

#ifdef DEBUG
			printf(pBuff);
#endif

			netconn_write(conn, pBuff, (u16_t)nResSize, NETCONN_COPY);
			netbuf_delete(in);
		}
		else
			RC_SET_STATUS_FLAG(cStatus, RC_STATUS_CLOSE);
	}

	if ( RC_IS_STATUS_FLAG_ASSERTED(cStatus, RC_STATUS_ERROR) ) {
		// Notify error

#ifdef DEBUG
			printf("RC:error.\n");
#endif

			netbuf_delete(in);
	}
}

