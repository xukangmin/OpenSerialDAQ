#ifndef UNITCONVERSIONMODEL_H
#define UNITCONVERSIONMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <vector>
#include <memory>

#include "UnitConversion.h"
#include "Database/DatabaseManager.h"

class DAQCORESHARED_EXPORT UnitConversionModel : public QAbstractTableModel
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
    UnitConversionModel(QObject* parent = nullptr);

    //bool isUnitConversionExists(QString portName);
    QModelIndex addUnitConversion(QHash<QString,QVariant> properties);

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
    std::unique_ptr<std::vector<std::shared_ptr<UnitConversion>>> mUnitConversions;
};

#endif // UNITCONVERSIONMODEL_H
