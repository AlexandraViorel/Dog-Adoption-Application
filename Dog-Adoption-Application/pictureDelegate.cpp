#include "pictureDelegate.h"

PictureDelegate::PictureDelegate(QWidget* parent) : QStyledItemDelegate{ parent } {}

void PictureDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QString dog = index.model()->data(index, Qt::EditRole).toString();

	if (index.column() != 4)
	{
		QStyledItemDelegate::paint(painter, option, index);
		return;
	}

	if (dog.contains("Husky"))
	{
		QPixmap pixMap("husky.jpg");
		painter->drawPixmap(option.rect, pixMap);
	}
	else if (dog.contains("Labrador Retriever"))
	{
		QPixmap pixMap("labrador.jpg");
		painter->drawPixmap(option.rect, pixMap);
	}
	else if (dog.contains("Chow chow"))
	{
		QPixmap pixMap("chow.jpg");
		painter->drawPixmap(option.rect, pixMap);
	}
	else if (dog.contains("Golden Retriever"))
	{
		QPixmap pixMap("golden.jpg");
		painter->drawPixmap(option.rect, pixMap);
	}
	else
	{
		QPixmap pixMap("dog.jpg");
		painter->drawPixmap(option.rect, pixMap);
	}
}

QSize PictureDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.column() == 1)
	{
		return QSize(32, 64);
	}
	return QStyledItemDelegate::sizeHint(option, index);
}


