#ifndef VIEWBUF_H
#define VIEWBUF_H

#include <QtGui>
#include "ui/ui_view_buffer.h"

class ViewBuffer : public QDialog {
	Q_OBJECT
	
	public:
		ViewBuffer(QWidget *parent = 0);
		~ViewBuffer();
	
	private:
		Ui::view_buffer ui;
};

#endif // VIEWBUG_H

