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

    QString mStationName;
    double mStationVersion;
    QString mStationType;

    //bool isVariableGroupExists(QString portName);
    bool loadGroupsFromConfigFile(QString configFilePath = ":/stationconfig.json");
    QModelIndex addVariableGroup(QHash<QString,QVariant> properties, int validation_index = -1, QVector<QHash<QString,QVariant>> specific_properties = QVector<QHash<QString,QVariant>>());
    bool loadValidationGroups(QString equationTemplate, int validation_index);
    void addVariables(QHash<QString,QVariant> properties, int validation_index = -1, QVector<QHash<QString,QVariant>> specific_properties = QVector<QHash<QString,QVariant>>());
    bool isVariableGroupExists(QHash<QString,QVariant> property);
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    void removeAllRows(const QModelIndex& parent = QModelIndex());
    QHash<int, QByteArray> roleNames() const override;
    void resolveAllDependency();
    bool findVariableGroupByID(int varGroupID, std::shared_ptr<VariableGroup>& varGroup);
    bool findVariableGroupByName(QString name, std::shared_ptr<VariableGroup>& varGroup);
    void startDAQ(const QModelIndex& index, int simulation = 0);
    void endDAQ(const QModelIndex& index, int simulation = 0);
    void endAllDAQ(int simulation = 0);
signals:
    void updateProgress(QString progress, int index, int total);

private:
    bool isIndexValid(const QModelIndex& index) const;



private:
    DatabaseManager& mDb;
    std::unique_ptr<std::vector<std::shared_ptr<VariableGroup>>> mVariableGroups;
    VariableModel *mVariableModel;
};


#endif // VARIABLEGROUPMODEL_H
