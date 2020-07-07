#include "scenetreeviewstyleditemdelegate.h"
#include <qtreeview.h>
#include "iraspastructure.h"
#include "scenetreeview.h"
#include "scenetreeviewmodel.h"

SceneTreeViewStyledItemDelegate::SceneTreeViewStyledItemDelegate(QWidget* parent) : QStyledItemDelegate(parent)
{

}


void SceneTreeViewStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  if(!index.isValid())
    return;

  QStyledItemDelegate::paint(painter, option, index);

  DisplayableProtocol* item = static_cast<DisplayableProtocol*>(index.internalPointer());

  if(Movie *movie = dynamic_cast<Movie*>(item))
  {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QStyleOptionViewItemV4 opt = option;
    QStyledItemDelegate::initStyleOption(&opt, index);
    QRect rect = opt.rect;
    rect.setX(0);

    // handle selection
    if(option.state & QStyle::State_Selected)
    {
      if(SceneTreeView *item = qobject_cast<SceneTreeView*>(this->parent()))
      {
        if(SceneTreeViewModel *model = qobject_cast<SceneTreeViewModel*>(item->model()))
        {
          if(model->isMainSelectedItem(movie->shared_from_this()))
          {
            painter->save();

            painter->setPen(Qt::white);
            painter->drawRoundedRect(rect.adjusted(1,1,-1,-1), 5, 5);

            painter->restore();
          }
        }
      }
    }

    painter->restore();
  }
}
