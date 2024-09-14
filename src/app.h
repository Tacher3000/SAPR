#ifndef APP_H
#define APP_H

#include <QApplication>
#include <QSettings>

class App : public QApplication
{
    Q_OBJECT
public:
    App(int &argc, char **argv, const QString &strOrg, const QString &strAppName);
    static App* theApp();
    QSettings *settings();
private:
    QSettings *m_pSettings;
};

#endif // APP_H
