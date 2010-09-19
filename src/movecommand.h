#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include "commandbuffer.h"
#include "deletecommand.h"
#include "pastecommand.h"

#include <QtGui>

class MoveCommand : public CommandBuffer {
	public:
		MoveCommand();
		virtual ~MoveCommand();
	
	public slots:
		// Pastes the files from the buffer to
		// current_dir
		virtual void run(QString current_dir, QStringList files);
	
	private:
		friend void DeleteCommand::run(QString current_dir, QStringList files);
		friend void PasteCommand::run(QString current_dir, QStringList files);
};


#endif // MOVECOMMAND_H
