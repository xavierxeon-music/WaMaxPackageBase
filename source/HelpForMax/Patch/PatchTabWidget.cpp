#include "PatchTabWidget.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QMenu>
#include <QSettings>
#include <QTimer>

#include "MainWindow.h"
#include "MessageBar.h"
#include "PackageInfo.h"
#include "PackageTabWidget.h"
#include "PatchWidget.h"

Patch::TabWidget::TabWidget(QWidget* parent)
   : RecentTabWidget(parent, "Patch")
{
   connect(this, &QTabWidget::currentChanged, this, &TabWidget::slotTabChanged);
}

void Patch::TabWidget::populate(QMenu* patchMenu, QMenu* viewMenu, QToolBar* toolBar)
{
   //
   patchMenu->addAction(QIcon(":/PatchLoad.svg"), "Load", this, &TabWidget::slotPromptLoadPatch);

   QAction* saveRefAction = patchMenu->addAction(QIcon(":/PatchSave.svg"), "Save", this, &TabWidget::slotWriteRef);
   saveRefAction->setShortcut(QKeySequence::Save);

   patchMenu->addAction(QIcon(":/PatchSaveAll.svg"), "SaveAll", this, &TabWidget::slotWriteAllRefs);
   patchMenu->addSeparator();

   patchMenu->addMenu(getRecentMenu());
   patchMenu->addSeparator();

   QAction* closePatchAction = patchMenu->addAction(QIcon(":/PatchClose.svg"), "Close", this, &TabWidget::slotClosePatch);
   closePatchAction->setShortcut(QKeySequence::Close);

   //
   viewMenu->addSeparator();
   viewMenu->addAction(QIcon(":/PatchOpenInMax.svg"), "Open In Max", this, &TabWidget::slotOpenInMax);

   QAction* showXMLAction = viewMenu->addAction(QIcon(":/PatchOpenRef.svg"), "Open XML", this, &TabWidget::slotOpenXML);
   showXMLAction->setShortcut(QKeySequence::Open);
   viewMenu->addSeparator();

   //
   toolBar->addAction(saveRefAction);
   toolBar->addSeparator();
   toolBar->addAction(closePatchAction);
}

void Patch::TabWidget::forceDirtyCheck()
{
   for (QWidget* p = parentWidget(); p != nullptr; p = p->parentWidget())
   {
      MainWindow* mainWindow = qobject_cast<MainWindow*>(p);
      if (mainWindow)
      {
         QTimer::singleShot(100, mainWindow, &MainWindow::checkDirty);
         return;
      }
   }
}

void Patch::TabWidget::init()
{
   for (const QString& patchPath : getCurrrentFiles())
   {
      slotLoadPatch(patchPath);
   }
}

void Patch::TabWidget::slotPromptLoadPatch()
{
   const QString patchFileName = QFileDialog::getOpenFileName(this, "MAX PATCH", QString(), "*.maxpat");
   if (patchFileName.isEmpty())
      return;

   slotLoadPatch(patchFileName);
}

void Patch::TabWidget::slotLoadPatch(const QString& patchFileName)
{
   Package::Info* info = Package::TabWidget::findOrCreate(patchFileName);
   if (!info)
   {
      Message::Bar::warning() << "PATCH does not belong to a package" << patchFileName;
      this->deleteLater();
      return;
   }

   const QString patchName = info->extractPatchName(patchFileName);
   for (int index = 0; index < tabBar()->count(); index++)
   {
      if (patchName == tabText(index))
         return;
   }

   Widget* patchWidget = new Patch::Widget(this, info, patchFileName);
   addTab(patchWidget, patchName);

   addRecentFile(patchFileName);
   addCurrentFile(patchFileName);
}

void Patch::TabWidget::slotWriteRef()
{
   Widget* patchWidget = qobject_cast<Widget*>(currentWidget());
   if (!patchWidget)
      return;

   emit signalRefWritten(patchWidget->getPath());
   patchWidget->writeRef();
}

void Patch::TabWidget::slotWriteAllRefs()
{
   for (int index = 0; index < tabBar()->count(); index++)
   {
      Widget* patchWidget = qobject_cast<Widget*>(widget(index));
      if (!patchWidget)
         continue;

      patchWidget->writeRef();
      emit signalRefWritten(patchWidget->getPath());
   }
}

void Patch::TabWidget::slotClosePatch()
{
   Widget* patchWidget = qobject_cast<Widget*>(currentWidget());
   if (!patchWidget)
      return;

   removeCurrentFile(patchWidget->getPath());
   patchWidget->deleteLater();
}

void Patch::TabWidget::slotCloseAllPatches(const Package::Info* info)
{
   QList<Widget*> deleteList;
   for (int index = 0; index < tabBar()->count(); index++)
   {
      Widget* patchWidget = qobject_cast<Widget*>(widget(index));
      if (info && info != patchWidget->getPacakgeInfo())
         continue;

      deleteList.append(patchWidget);
   }

   for (Widget* patchWidget : deleteList)
   {
      removeCurrentFile(patchWidget->getPath());
      patchWidget->deleteLater();
   }
}

void Patch::TabWidget::slotOpenInMax()
{
   Widget* patchWidget = qobject_cast<Widget*>(currentWidget());
   if (patchWidget)
      patchWidget->openInMax();
}

void Patch::TabWidget::slotOpenXML()
{
   Widget* patchWidget = qobject_cast<Widget*>(currentWidget());
   if (patchWidget)
      patchWidget->openXML();
}

void Patch::TabWidget::slotTabChanged(int index)
{
   if (index < 0)
   {
      // no tab left
      emit signalTabSelected("", nullptr);
      return;
   }

   const Widget* patchWidget = qobject_cast<Widget*>(widget(index));

   const QString& path = patchWidget->getPath();
   if (path.isEmpty())
      return;

   emit signalTabSelected(path, patchWidget->getPacakgeInfo());
}

RecentTabWidget::Entry Patch::TabWidget::creatreEntry(const QFileInfo& fileInfo)
{
   const QString patchName = fileInfo.completeBaseName();
   auto openFunction = std::bind(&TabWidget::slotLoadPatch, this, fileInfo.absoluteFilePath());

   Entry entry{patchName, openFunction};
   return entry;
}
