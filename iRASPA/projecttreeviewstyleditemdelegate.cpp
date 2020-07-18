#include "projecttreeviewstyleditemdelegate.h"
#include <qtreeview.h>
#include "iraspastructure.h"
#include "projecttreeview.h"
#include "projecttreeviewmodel.h"

ProjectTreeViewStyledItemDelegate::ProjectTreeViewStyledItemDelegate(QWidget* parent) : QStyledItemDelegate(parent)
{

}


void ProjectTreeViewStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  if(!index.isValid())
    return;

  QStyledItemDelegate::paint(painter, option, index);


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
    if(ProjectTreeView *item = qobject_cast<ProjectTreeView*>(this->parent()))
    {
      if(ProjectTreeViewModel *model = qobject_cast<ProjectTreeViewModel*>(item->model()))
      {
        if(ProjectTreeNode *item = static_cast<ProjectTreeNode*>(index.internalPointer()))
        {
          if(model->isMainSelectedItem(item->shared_from_this()))
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
  }


  painter->restore();
}
