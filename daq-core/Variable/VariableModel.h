#ifndef VARIABLEMODEL_H
#define VARIABLEMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <vector>
#include <memory>
#include <Device/DeviceModel.h>
#include "Variable.h"
#include "Database/DatabaseManager.h"

class DAQCORESHARED_EXPORT VariableModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    enum Roles {
           IdRole = Qt::UserRole + 1,
           NameRole,
           TypeRole,
           ComPortRole,
           BaudRateRole,
           ParityRole,
           DataBitsRole,
           StopBitsRole
    };
    VariableModel(DeviceModel* dev_model, QObject* parent = nullptr);

    DeviceModel* m_dev_model;
    //bool isVariableExists(QString portName);
    QModelIndex addVariable(QHash<QString,QVariant> properties, QHash<QString,QVariant> group_properties = QHash<QString,QVariant>());
    void updateVariable(Variable& var);
    void addDataToVariable(QHash<QString,QVariant> data, std::shared_ptr<Variable> &var);
    bool findVariableByNameAndDeviceID(QString name, int device_id, std::shared_ptr<Variable>& var_ret);
    bool findVariableByNameAndGroupID(QString name, int group_id, std::shared_ptr<Variable>& var_ret);
    bool findVariableByID(int var_id, std::shared_ptr<Variable>& var_ret);
    bool calculateVariable(const std::shared_ptr<Variable>& var);
    bool isVariableExistsInVector(const std::shared_ptr<Variable>& t, std::vector<std::shared_ptr<Variable>>& v);
    bool isVariableExists(QHash<QString,QVariant> property);
    bool resolveDependency(int group_id);
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    QHash<int, QByteArray> roleNames() const override;
    bool findVariablesByGroupID(int group_id, std::vector<std::shared_ptr<Variable>>& var_ret);

private:
    bool isIndexValid(const QModelIndex& index) const;



private:
    DatabaseManager& mDb;
    std::unique_ptr<std::vector<std::shared_ptr<Variable>>> mVariables;
};

class DAQCORESHARED_EXPORT VariableProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT

public:
    VariableProxyModel(QObject *parent = nullptr);

    void setGroupID(int id);

    int mGroupID;
    QVariant data(const QModelIndex &index, int role) const noexcept override;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const override;
    //bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

};

#endif // VARIABLEMODEL_H
