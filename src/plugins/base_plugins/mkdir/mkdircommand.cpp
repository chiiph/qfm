#include "mkdircommand.h"

MkdirCommand::MkdirCommand() : 
	CommandBuffer(QString("mkdir"), Qt::Key_F10) 
{
	CommandBuffer::property = CommandBuffer::InPlace;
	newname = "";
}

MkdirCommand::~MkdirCommand() {

}

void 
MkdirCommand::run(QString current_dir, QStringList files) {
	qDebug() << "Running MkdirCommand";
	QDir dir(current_dir);
	
	if(!dir.isReadable()) return;

	newname = MkdirPrompt::prompt();
	if(newname.size() == 0) return;

	dir.mkpath(newname);
}

MkdirPrompt::MkdirPrompt(QWidget *parent) : 
	QDialog(parent)
{
	ui.setupUi(this);
}

MkdirPrompt::~MkdirPrompt() {

}

QString
MkdirPrompt::prompt() {
	MkdirPrompt r;
	r.exec();
	if(r.result() == QDialog::Accepted) {
		return r.get_newname();
	}

	return QString("");
}

Q_EXPORT_PLUGIN2(plugin_mkdir, MkdirCommand)
