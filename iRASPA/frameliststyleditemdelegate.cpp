#include "frameliststyleditemdelegate.h"
#include <qtreeview.h>
#include "iraspastructure.h"
#include "framelistview.h"
#include "framelistviewmodel.h"

FrameListViewStyledItemDelegate::FrameListViewStyledItemDelegate(QWidget* parent) : QStyledItemDelegate(parent)
{

}


void FrameListViewStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  if(!index.isValid())
    return;

  QStyledItemDelegate::paint(painter, option, index);

  if(iRASPAStructure *iraspa_structure = static_cast<iRASPAStructure*>(index.internalPointer()))
  {
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QStyleOptionViewItem opt = option;
    QStyledItemDelegate::initStyleOption(&opt, index);
    QRect rect = opt.rect;

    // handle selection
    if(option.state & QStyle::State_Selected)
    {
      if(FrameListView *item = qobject_cast<FrameListView*>(this->parent()))
      {
        if(FrameListViewModel *model = qobject_cast<FrameListViewModel*>(item->model()))
        {
          if(model->isMainSelectedItem(iraspa_structure->shared_from_this()))
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
