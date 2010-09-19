#include "commandbuffer.h"

CommandBuffer::CommandBuffer(QString cid) : 
	QObject(),
	command_id(cid),
	property(0)
{

}

CommandBuffer::~CommandBuffer() {

}

void
CommandBuffer::run(QString current_dir, QStringList files) {

}

void 
CommandBuffer::set_command_id(QString id) {
	command_id = id;
}

void 
CommandBuffer::add_file(QString file) {
	files << file;
}

void 
CommandBuffer::clear_buffer() {
	files.clear();
}

QString 
CommandBuffer::get_command_id() const {
	return command_id;
}

bool
CommandBuffer::show() {
	return false;
}
