#include "qfm.h"
#include "qfmcore.h"
#include "listitem.h"

Qfm::Qfm() : 
	QMainWindow() {
	core = new QfmCore(this);
	ui.setupUi(this);
	
	spacer_selected = new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding);
	spacer_directory = new QSpacerItem(0,0,QSizePolicy::Expanding, QSizePolicy::Expanding);
	update_layouts();
}

Qfm::~Qfm() {

}

void
Qfm::update_layouts() {
	// First clear everything
	for(int i = 0; i < ui.directory_layout->count(); i++) {
		ui.directory_layout->removeItem(ui.directory_layout->itemAt(i));
	}
	for(int i = 0; i < ui.selected_layout->count(); i++) {
		ui.selected_layout->removeItem(ui.selected_layout->itemAt(i));
	}

	// Then add everything
	foreach(ListItem *it, *(core->get_items(QfmCore::Directory))) {
		ui.directory_layout->addWidget(it);
	}

	foreach(ListItem *it, *(core->get_items(QfmCore::Selected))) {
		ui.selected_layout->addWidget(it);
	}
}

void
Qfm::keyPressEvent(QKeyEvent *ev) {
	QStringList l;
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

		case Qt::Key_Y:
			qDebug() << "Yanking selected";
			foreach(ListItem *file, *(core->get_items(QfmCore::Selected))) {
				l << file->get_full_path();
			}

			core->run("copy", l);
		break;

		case Qt::Key_D:
			qDebug() << "Deleting selected";
			foreach(ListItem *file, *(core->get_items(QfmCore::Selected))) {
				l << file->get_full_path();
			}

			core->run("delete", l);
		break;

		case Qt::Key_C:
			qDebug() << "Cutting selected";
			foreach(ListItem *file, *(core->get_items(QfmCore::Selected))) {
				l << file->get_full_path();
			}

			core->run("move", l);
		break;

		case Qt::Key_P:
			core->flush_buffer();
		break;

		case Qt::Key_Return:
			core->navigate();
			update_layouts();
		break;
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
