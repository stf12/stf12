/**
 * <b>File:</b> net_launc_srv.h
 *
 * <b>Project:</b> Remote Calculator Demo
 *
 * <b>Description:</b> This module declares the public vStartEthernetTasks
 * function that starts all taks needed by the demo.<br/>
 *
 * <b>Created:</b> 27/11/2007
 *
 * <dl>
 * <dt><b>Author</b>:</dt>
 * <dd>Stefano Oliveri</dd>
 * <dt><b>E-mail:</b></dt>
 * <dd>stefano.oliveri@st.com</dd>
 * </dl>
 */

#ifndef ETHERNET_H
#define ETHERNET_H

// The IP address being used.
#define emacIPADDR0			configIP_ADDR0
#define emacIPADDR1			configIP_ADDR1
#define emacIPADDR2			configIP_ADDR2
#define emacIPADDR3			configIP_ADDR3

// The gateway address being used.
#define emacGATEWAY_ADDR0   10
#define emacGATEWAY_ADDR1   52
#define emacGATEWAY_ADDR2   156
#define emacGATEWAY_ADDR3   254

// The network mask being used.
#define emacNET_MASK0 		configNET_MASK0
#define emacNET_MASK1 		configNET_MASK1
#define emacNET_MASK2 		configNET_MASK2
#define emacNET_MASK3 		configNET_MASK3

// When the NET_USE_DHCP macro is defined the board use dhcp protocol to obtains its network parameters, otherwise
// a static configuration (IP address, Subnet Mask, Gateway) is used.
//#define NET_USE_DHCP

void vStartEthernetTasks( unsigned portBASE_TYPE uxPriority, void *pvParameters );

#endif
