#include "listitem.h"

ListItem::ListItem(QString file, QString fpath, QListWidget *parent) :
	QListWidgetItem(parent),
	full_path(fpath),
	display_text(file),
	selected(true) {
	toggle_selected();
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
