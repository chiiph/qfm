#include "movecommand.h"

MoveCommand::MoveCommand() : 
	CommandBuffer(QString("move")) 
{
	
}

MoveCommand::~MoveCommand() {

}

void 
MoveCommand::run(QString current_dir, QStringList files) {
	qDebug() << "Running MoveCommand";
	QDir dir(current_dir);
	
	if(!dir.isReadable()) return;

	PasteCommand().run(current_dir, files);
	DeleteCommand d;
	d.force_doit();
	d.run(current_dir, files);
}
