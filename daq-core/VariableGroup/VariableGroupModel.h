#ifndef VARIABLEGROUPMODEL_H
#define VARIABLEGROUPMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <vector>
#include <memory>
#include <Device/DeviceModel.h>
#include "VariableGroup.h"
#include "Database/DatabaseManager.h"
#include "Variable/VariableModel.h"

class DAQCORESHARED_EXPORT VariableGroupModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum Roles {
           IdRole = Qt::UserRole + 1,
           NameRole,
           DataRole
    };
    VariableGroupModel(VariableModel *variableModel, QObject* parent = nullptr);


    //bool isVariableGroupExists(QString portName);
    void loadGroupsFromConfigFile(QString configFilePath = ":/stationconfig.json");
    QModelIndex addVariableGroup(QHash<QString,QVariant> properties, QHash<QString,QVariant> specific_properties = QHash<QString,QVariant>());
    void addVariables(QHash<QString,QVariant> properties, QHash<QString,QVariant> specific_properties);
    bool isVariableGroupExists(QHash<QString,QVariant> property);
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    QHash<int, QByteArray> roleNames() const override;

private:
    bool isIndexValid(const QModelIndex& index) const;



private:
    DatabaseManager& mDb;
    std::unique_ptr<std::vector<std::shared_ptr<VariableGroup>>> mVariableGroups;
    VariableModel *mVariableModel;
};


#endif // VARIABLEGROUPMODEL_H
