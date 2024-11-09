#ifndef MainWindowH
#define MainWindowH

#include <QMainWindow>

#include "Package/PackageTabWidget.h"
#include "Patch/PatchTabWidget.h"
#include "StructureWidget.h"
#include "TestClient.h"

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   MainWindow();

private slots:
   void slotCheckDirty();

private:
   void populateMenuAndToolBar();
   void closeEvent(QCloseEvent* ce) override;
   void toogleDock(QWidget* widget, const QString& name, bool enabled);

private:
   Package::TabWidget* packageWidget;
   Patch::TabWidget* patchWidget;
   Structure::Widget* structureWidget;
#ifdef TEST_CLIENT_AVAILABLE
   TestClient* testClient;
#endif // TEST_CLIENT_AVAILABLE
};

#endif // NOT MainWindowH
