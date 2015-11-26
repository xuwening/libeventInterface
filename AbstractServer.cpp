#include "AbstractServer.h"


AbstractServer::AbstractServer(void) {
	msgHandler = NULL;
}


AbstractServer::~AbstractServer(void) {
	delete msgHandler;
}
