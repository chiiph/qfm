#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include "commandbuffer.h"

#include <QtGui>

class DeleteCommand : public QObject, public CommandBuffer {
	Q_OBJECT
	Q_INTERFACES(CommandBuffer)
	public:
		DeleteCommand();
		virtual ~DeleteCommand();
	
		void force_doit() { doit = true; }

		// Pastes the files from the buffer to
		// current_dir
		virtual void run(QString current_dir, QStringList files);
		virtual bool show();
	
	private:
		bool removeDir(const QString &dirName);

		bool doit;
};


#endif // PASTECOMMAND_H
