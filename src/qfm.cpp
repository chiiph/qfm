#include "qfm.h"
#include "core/qfmcore.h"
#include "listitem.h"

Qfm::Qfm() : 
	QMainWindow() {
	core = new QfmCore(this);

	this->connect(core, SIGNAL(refresh_ui()), SLOT(update_layouts()));

	ui.setupUi(this);
	
	spacer_selected = new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding);
	spacer_directory = new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding);
	update_layouts();

	ui.directory_list->installEventFilter(this);
	ui.selected_list->installEventFilter(this);

	mode = Qfm::Normal;
}

Qfm::~Qfm() {

}

void
Qfm::update_layouts() {
	qDebug() << "Updating layouts";
	// First clear everything
	while(ui.directory_list->count() > 0)
		ui.directory_list->takeItem(0);
	while(ui.selected_list->count() > 0)
		ui.selected_list->takeItem(0);

	// Then add everything
	foreach(ListItem *it, *(core->get_items(QfmCore::Directory))) {
		ui.directory_list->addItem(it);
	}

	foreach(ListItem *it, *(core->get_items(QfmCore::Selected))) {
		ui.selected_list->addItem(it);
	}
}

void
Qfm::scroll_everything() {
	foreach(ListItem *it, *(core->get_items(QfmCore::Directory))) {
		if(it->is_selected())
			ui.directory_list->scrollToItem(it);
	}
	foreach(ListItem *it, *(core->get_items(QfmCore::Selected))) {
		if(it->is_selected())
			ui.selected_list->scrollToItem(it);
	}
}

void
Qfm::keyPressEvent(QKeyEvent *ev) {
	QStringList l;
	bool handled = true;
	switch(ev->key()) {
		case Qt::Key_K:
			up(QfmCore::Directory);
		break;

		case Qt::Key_J:
			down(QfmCore::Directory);
		break;

		// Select in directory
		case Qt::Key_L:
			move_selected(QfmCore::Directory, QfmCore::Selected);
		break;

		// Up in selected
		case Qt::Key_I:
			up(QfmCore::Selected);
		break;

		// Down in selected
		case Qt::Key_U:
			down(QfmCore::Selected);
		break;

		// Unselect in selected
		case Qt::Key_O:
			move_selected(QfmCore::Selected, QfmCore::Directory);
		break;

		case Qt::Key_P:
			core->flush_buffer();
		break;

		case Qt::Key_Return:
			core->set_filter("");
			ui.search_line->setText("");
			core->navigate();
			update_layouts();
		break;

		case Qt::Key_Slash:
			if(mode == Qfm::Normal) {
				mode = Qfm::Search;
				connect(ui.search_line, SIGNAL(textChanged(const QString &)),
						this, SLOT(set_filter(const QString &)));
				ui.search_line->setEnabled(true);
				ui.search_line->setFocus();
			}
		break;

		case Qt::Key_Escape:
			if(mode == Qfm::Search) {
				mode = Qfm::Normal;
				disconnect(ui.search_line, 0, this, 0);
				ui.search_line->clearFocus();
				ui.search_line->setEnabled(false);
			}
		break;
		
		case Qt::Key_Space:
			set_filter("");
			ui.search_line->setText("");
		break;

		default:
			handled = false;
		break;
	}

	if(handled) return;

	qDebug() << "Custom handling";

	qDebug() << ev->key();
	qDebug() << Qt::Key_C;
	QString id = core->get_key_map()->value((Qt::Key)ev->key(), QString(""));
	qDebug() << id;
	if(id.size() != 0) {
		foreach(ListItem *file, *(core->get_items(QfmCore::Selected))) {
			l << file->get_full_path();
		}

		core->run(id, l);
	}
}

void
Qfm::up(QfmCore::Buffer b) {
	if(core->get_items(b)->empty())
		return;
	if(core->get_selected_item(b) <= 0)
		return;

	core->update_selected_item(b, -1);
	core->get_items(b)->at(core->get_selected_item(b)+1)->toggle_selected();
	core->get_items(b)->at(core->get_selected_item(b))->toggle_selected();
	scroll_everything();
}

void
Qfm::down(QfmCore::Buffer b) {
	if(core->get_items(b)->empty())
		return;
	if(core->get_selected_item(b) == core->get_items(b)->count() - 1) {
		return;
	}

	core->update_selected_item(b, 1);
	if(core->get_selected_item(b) != 0)
		core->get_items(b)->at(core->get_selected_item(b) - 1)->toggle_selected();
	core->get_items(b)->at(core->get_selected_item(b))->toggle_selected();
	scroll_everything();
}

void 
Qfm::select(int pos, QfmCore::Buffer src, QfmCore::Buffer dest) {
	if(core->get_items(src)->empty())
		return;
	if(pos >= core->get_items(src)->count() or pos < 0)
		return;
	
	ListItem *it = core->get_items(src)->takeAt(pos);
	if(it->is_selected())
		it->toggle_selected();
	
	*(core->get_items(dest)) << it;

	update_layouts();
}

void
Qfm::move_selected(QfmCore::Buffer src, QfmCore::Buffer dest) {
	if(core->get_items(src)->empty() or core->get_selected_item(src) < 0)
		return;
	core->get_items(src)->at(core->get_selected_item(src))->toggle_selected();
	select(core->get_selected_item(src), src, dest);
	if(core->get_selected_item(src) >= core->get_items(src)->count())
		core->update_selected_item(src, -1);
	if((core->get_selected_item(src) >= 0) and (core->get_selected_item(src) < core->get_items(src)->count()))
		core->get_items(src)->at(core->get_selected_item(src))->toggle_selected();
}

bool 
Qfm::eventFilter(QObject *obj, QEvent *event) {
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
		keyPressEvent(keyEvent);
		return true;
	} else {
		return false;
	}
}

void
Qfm::set_filter(const QString &str) {
	core->set_filter(str);
	core->refresh();
}
