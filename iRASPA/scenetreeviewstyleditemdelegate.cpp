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

    QStyleOptionViewItem opt = option;
    QStyledItemDelegate::initStyleOption(&opt, index);
    QRect rect = opt.rect;

    #if defined(Q_OS_MAC)
      rect.setX(0);
    #endif

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

            #if defined(Q_OS_WIN)
              painter->setPen(Qt::blue);
            #elif defined(Q_OS_MAC)
              painter->setPen(Qt::white);
            #elif defined(Q_OS_LINUX)
              painter->setPen(Qt::white);
            #endif

            painter->drawRoundedRect(rect.adjusted(1,1,-1,-1), 5, 5);

            painter->restore();
          }
        }
      }
    }

    painter->restore();
  }
}
