#include "listitem.h"

ListItem::ListItem(QString file, QString fpath, QListWidget *parent) :
	QListWidgetItem(parent),
	full_path(fpath),
	display_text(file),
	selected(true) 
{
	toggle_selected();
	QFileInfo finfo(full_path);
	if(finfo.isDir())
		setIcon(QIcon(":folder"));
	else if(finfo.isFile()) {
		setIcon(QIcon(":generic-file"));
		QString s = full_path;
		QString ext = "";
		int pos = s.indexOf('.');
		if(pos >= 0)
			ext = s.right(s.length() - pos - 1);
		qDebug() << "Extension" << ext;
	}
}

ListItem::~ListItem() {

}

void 
ListItem::toggle_selected() {
	selected = !selected;

//    if(selected) {
//        setText("<b>"+display_text+"</b>");
//    } else {
		setText(display_text);
//    }

	setSelected(selected);
}
