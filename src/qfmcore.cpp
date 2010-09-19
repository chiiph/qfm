#include "qfmcore.h"
#include "qfm.h"
#include "commandbuffer.h"
#include "pastecommand.h"
#include "deletecommand.h"
#include "movecommand.h"
#include "listitem.h"

QfmCore::QfmCore(Qfm *q) : 
	QObject(),
	shared_memory("qfm_mem"),
	directory(QDir::home()),
	qfm(q) {
	load_commands();

	// "After crash" shared memory handler
	shared_memory.attach();
	if(shared_memory.isAttached())
		shared_memory.detach();

	connect(&timer, SIGNAL(timeout()), this, SLOT(clearmem()));

	// Init buffer items
	for(int i = 0; i < QfmCore::Last; i++)
		items.append(new QList<ListItem *>);

	// Init buffer selection
	for(int i = 0; i < QfmCore::Last; i++)
		selected_item.append(0);
	filldir();
}

QfmCore::~QfmCore() {
	if(shared_memory.isAttached()) {
		qDebug() << "INFO: Detaching from shared_memory.";
		shared_memory.detach();
	}
}

void 
QfmCore::filldir() {
	qDebug() << items.size();
	for(int i = 0; i < QfmCore::Last; i++) {
		while(!get_items((Buffer)i)->empty())
			delete (get_items((Buffer)i))->takeFirst();
	}

	foreach(QString file, directory.entryList()) {
		qDebug() << file;
		*(get_items(QfmCore::Directory)) << new ListItem(file, directory.absolutePath()+"/"+file, qfm);
	}

//    qDebug() << directory_items.count();
	for(int i = 0; i < QfmCore::Last; i++) {
		if(!get_items((Buffer)i)->empty())
			get_items((Buffer)i)->at(0)->toggle_selected();
		else
			selected_item[i] = -1;
	}
}

void 
QfmCore::load_buffer(QString command, QStringList files) {
	qDebug() << command;
	qDebug() << files;

	QString strbuf = "";
	strbuf += QString::number(1) + ";";
	strbuf += command + ";";

	foreach(QString file, files) {
		strbuf += file + ";";
	}

	write(strbuf);

	timer.start(10);
}

void 
QfmCore::flush_buffer() {
	QString current_dir = directory.absolutePath();

	QString text = read();
	text = text.replace(0,1,"0");
	qDebug() << "TEXT NOW>" << text;
	write(text);
	qDebug() << "A VER" << read();

	QStringList files = text.split(";", QString::SkipEmptyParts);
	QString pid = files.takeAt(0);
	QString cmd = files.takeAt(0);
	qDebug() << "Command to run:";
	qDebug() << cmd;
	qDebug() << files;
	command_map[cmd]->run(current_dir, files);
}

void 
QfmCore::load_commands() {
	PasteCommand *p = new PasteCommand();
	command_map[p->get_command_id()] = p;
	DeleteCommand *d = new DeleteCommand();
	command_map[d->get_command_id()] = d;
	MoveCommand *m = new MoveCommand();
	command_map[m->get_command_id()] = m;
}

void 
QfmCore::navigate() {
	QFileInfo finfo(get_items(QfmCore::Directory)->at(selected_item.at(QfmCore::Directory))->get_full_path());
	if(finfo.isDir()) {
		directory.cd(finfo.filePath());
		filldir();
	}
	selected_item[QfmCore::Directory] = 0;
}

void 
QfmCore::clearmem() {
	if(shared_memory.lock()) {
		QBuffer buffer;
		QDataStream in(&buffer);
		QString text;

		buffer.setData((char *)shared_memory.data(), shared_memory.size());
		buffer.open(QBuffer::ReadWrite);

		in >> text;
		if(text.startsWith("0")) {
			qDebug() << "INFO: Cleaning shared memory";
			shared_memory.detach();
			timer.stop();
		}
		shared_memory.unlock();
	}
}

bool
QfmCore::write(QString data) {
	QByteArray ba;
	QBuffer buffer(&ba);

	if(shared_memory.isAttached())
		shared_memory.detach();

	buffer.open(QBuffer::ReadWrite);
	QDataStream out(&buffer);

	out << data;

	int size = buffer.size();

	if(!shared_memory.create(size)) {
		if(!shared_memory.attach()) {
			qDebug() << "ERROR: Can't create shared_memory.";
			return false;
		}
	}

	shared_memory.lock();
	char *to = (char *)shared_memory.data();
	const char *from = buffer.data().data();
	memcpy(to, from, qMin(shared_memory.size(), size));
	shared_memory.unlock();

	return true;
}

QString
QfmCore::read() {
	if(!shared_memory.attach() and !shared_memory.isAttached()) {
		qDebug() << "ERROR: Can't attach to shared_memory.";
		return QString();
	}

	if(shared_memory.size() == 0) {
		qDebug() << "INFO: There's nothing in the buffer to do.";
		return QString();
	}

	QBuffer buffer;
	QDataStream in(&buffer);
	QString text;

	shared_memory.lock();
	buffer.setData((char *)shared_memory.data(), shared_memory.size());
	buffer.open(QBuffer::ReadWrite);

	in >> text;
	shared_memory.unlock();

	qDebug() << "saraaaaaaaannnnn";
	qDebug() << text;

	shared_memory.detach();

	return text;
}

void
QfmCore::run(QString cmd, QStringList files) {
	if(!command_map[cmd]->get_properties()) {
		load_buffer(cmd, files);
		return;
	}
	
	if(command_map[cmd]->get_properties() & CommandBuffer::Display) {
		qDebug() << "Display command";
		command_map[cmd]->show();
	}

	if(command_map[cmd]->get_properties() & CommandBuffer::InPlace) {
		qDebug() << "InPlace command";
		QString current_dir = directory.absolutePath();
		command_map[cmd]->run(current_dir, files);
	}
}
