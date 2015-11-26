#include "InputHandler.h"

typedef void (*input_message_callback)(void);
static void input_message_cb() {

}

extern int g_flag;
//return count in queue
int InputHandler::scanQueueMessage() {
	if (g_flag)
		return 10;
	else
		return 0;
	//return 10;
}