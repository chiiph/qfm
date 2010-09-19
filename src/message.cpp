#include "message.h"

struct msgbuf {
	long mtype;
	char text[MSGSZ];
};

Message::Message(key_t k, QObject *parent = 0) :
	QObject(parent),
	mtype(1),
	key(k),
	buf_length(0) {

}

Message::~Message() {

}

bool 
Message::create() {
	if((msgid = msgget(key, IPC_CREAT | 0666)) < 0)
		return false;
	return true;
}

bool 
Message::send(const char *msg) {
	msgbuf sbuf;
	sbuf.type = 1;
	strncpy(sbuf.text, msg, MSGSZ);

}

bool 
Message::recieve() {

}
