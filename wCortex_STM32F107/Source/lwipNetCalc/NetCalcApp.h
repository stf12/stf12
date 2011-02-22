/** 
 * <b>File:</b> NetCalcApp.h
 *
 * <b>Project:</b> Remote Calculator Demo
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

#ifndef __NET_CALC_APP_H_
#define __NET_CALC_APP_H_

// The task priority.
#define basicwebWEBSERVER_PRIORITY      ( tskIDLE_PRIORITY + 2 )

// The port on which we listen.
#define NET_CALC_PORT		( 2000 )

// The size of the task stack.
#define lwipNET_CALC_STACK_SIZE	250

/*
 * The Remote Calculator task control loop.
 *
 * @param pvParameters not used.
 */
void NetworkCalculatorLoop( void *pvParameters );

#endif

