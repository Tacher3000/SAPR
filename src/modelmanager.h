#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include <QTableView>
#include <QFile>
#include <QDataStream>

#include "sizetablemodel.h"
#include "nodemodel.h"

class ModelManager : public QObject {
    Q_OBJECT

public:
    explicit ModelManager(QObject *parent = nullptr);

    void initializeModels(QTableView* sizeTableView, QTableView* nodeTableView);
    void updateNodeModel();
    void saveModels(const QString &filePath);
    void loadModels(const QString &filePath);
    void clearModels();

    SizeTableModel* sizeModel() const { return m_sizeModel; }
    NodeModel* nodeModel() const { return m_nodeModel; }

signals:
    void nodeModelUpdated();

private:
    SizeTableModel *m_sizeModel;
    NodeModel *m_nodeModel;
};

#endif // MODELMANAGER_H
