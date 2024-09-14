#include "app.h"

App::App(int &argc, char **argv, const QString &strOrg, const QString &strAppName) : QApplication(argc, argv), m_pSettings(0)
{
    setOrganizationName(strOrg);
    setApplicationName(strAppName);

    m_pSettings = new QSettings(strOrg, strAppName, this);

}

App *App::theApp()
{
    return (App*)qApp;
}

QSettings *App::settings()
{
    return m_pSettings;
}
