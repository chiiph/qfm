#ifndef MESSAGE_H
#define MESSAGE_H

#define MSGSZ 128

#include <QtCore>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

class Message : public QObject {
	Q_OBJECT

	public:
		Message(key_t key, QObject *parent = 0);
		virtual ~Message();

		void set_key(key_t k) { key = k; }
		key_t get_key() { return key }
		size_t get_size() { return buf_length; }
		char *get_text() { return text; }

		bool create();
		bool send(const char *msg);
		bool recieve();
	
	private:
		long mtype;
		char text[MSGSZ];
		key_t key;
		size_t buf_length;
		int msgid;
};

#endif // MESSAGE_H
