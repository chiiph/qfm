#include "view_buffer.h"

ViewBuffer::ViewBuffer(QWidget *parent) :
	QDialog(parent)
{
	ui.setupUi(this);

	QClipboard *clip = QApplication::clipboard();
	QStringList buf = clip->text().split(";");
	QString cmd = buf.takeAt(0);
	QString files = "";
	while(!buf.empty())
		files += buf.takeAt(0) + "\n";
	ui.cmd_label->setText(tr("cmd=")+cmd);
	ui.buffer_text->setText(files);
}

ViewBuffer::~ViewBuffer() {

}
