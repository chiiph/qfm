#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include "commandbuffer.h"

class MoveCommand : public QObject, public CommandBuffer {
	Q_OBJECT
	Q_INTERFACES(CommandBuffer)
	public:
		MoveCommand();
		virtual ~MoveCommand();
	
	public slots:
		// Pastes the files from the buffer to
		// current_dir
		virtual void run(QString current_dir, QStringList files);
	
	private:
		void runPaste(QString current_dir, QStringList files);
};


#endif // MOVECOMMAND_H
