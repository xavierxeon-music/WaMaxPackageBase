#include "SuggestTreeView.h"

#include <QTimer>

#include "PatchWidget.h"
#include "SuggestModelAbstract.h"

Suggest::TreeView::TreeView(QWidget* parent)
   : QTreeView(parent)
   , widget(nullptr)
   , model(nullptr)
{
   setRootIsDecorated(false);
   setUniformRowHeights(true);
}

void Suggest::TreeView::init(Patch::Widget* widget, Model::Abstract* model)
{
   this->widget = widget;
   this->model = model;

   setModel(model);

   //connect(model, &Model::Abstract::signalDataEdited, this, &TreeView::slotResizeColumns);
}

void Suggest::TreeView::setButton(QToolButton* transferButton)
{
   static const QString styleSheet = "QToolButton { border: 0px none #8f8f91;}";

   static const QString transfer = QString::fromUtf8("\u27a4");
   transferButton->setStyleSheet(styleSheet);
   transferButton->setText(transfer);

   connect(transferButton, &QAbstractButton::clicked, this, &TreeView::slotTransfer);
}

void Suggest::TreeView::slotResizeColumns()
{
   auto resizeIternal = [this]()
   {
      for (int col = 0; col < model->columnCount(); col++)
         this->resizeColumnToContents(col);

      QModelIndex index = this->model->index(0, 0);
      const int height = this->header()->height() + this->rowHeight(index);

      this->setMaximumHeight(height + 10);
   };

   QTimer::singleShot(10, this, resizeIternal);
}

void Suggest::TreeView::slotTransfer()
{
}
