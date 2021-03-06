#ifndef LISTITEM_H
#define LISTITEM_H

#include <QtGui>

class ListItem : public QListWidgetItem {
	public:
		ListItem(QString file, QString full_path, QListWidget *parent = 0);
		virtual ~ListItem();

		void toggle_selected();
		QString get_full_path() { return full_path; }
		bool is_selected() { return selected; }
	
	private:
		QString full_path;
		QString display_text;
		bool selected;
};

#endif // LISTITEM_H
