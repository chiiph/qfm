#include "renamecommand.h"

RenameCommand::RenameCommand() : 
	CommandBuffer(QString("rename"), Qt::Key_R) 
{
	CommandBuffer::property = CommandBuffer::InPlace;
	newname = "";
}

RenameCommand::~RenameCommand() {

}

void 
RenameCommand::run(QString current_dir, QStringList files) {
	qDebug() << "Running RenameCommand";
	QDir dir(current_dir);
	
	if(!dir.isReadable()) return;

	QFileInfo *finfo;
	foreach(QString file, files) {
		finfo = new QFileInfo(file);
		
		if(finfo->fileName()=="." || finfo->fileName()=="..") continue;

		newname = RenamePrompt::prompt(finfo->fileName());
		if(newname.size() == 0) continue;

		if(finfo->isFile()) {
			if(!QFile(finfo->filePath()).rename(finfo->path()+"/"+newname))
				qDebug() << "ERROR: Trying to rename" << finfo->fileName() << "to" << dir.path();
		} else if(finfo->isDir()) {
			if(!QDir().rename(finfo->filePath(), finfo->path()+"/"+newname))
				qDebug() << "ERROR: Trying to copy dir" << finfo->filePath();
		}

		delete finfo;
	}
}

RenamePrompt::RenamePrompt(QWidget *parent) : 
	QDialog(parent)
{
	ui.setupUi(this);
}

RenamePrompt::~RenamePrompt() {

}

QString
RenamePrompt::prompt(QString file) {
	RenamePrompt r;
	r.set_file(file);
	r.exec();
	if(r.result() == QDialog::Accepted) {
		return r.get_newname();
	}

	return QString("");
}

Q_EXPORT_PLUGIN2(plugin_rename, RenameCommand)
