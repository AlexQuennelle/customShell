#pragma once

#include "compositorBackend.h"

#include <QAbstractItemModel>

class QListModel : public QAbstractListModel
{
	Q_OBJECT; // NOLINT

	public:
	enum Roles // NOLINT
	{
		ObjectRole = Qt::UserRole + 1,
	};
	QListModel(QObject* parent = nullptr) : QAbstractListModel(parent) { }
	~QListModel() override = default;

	auto roleNames() const -> QHash<int, QByteArray> override
	{
		QHash<int, QByteArray> roles;
		roles.insert(Roles::ObjectRole, "object");
		return roles;
	}
	auto data(const QModelIndex& index, int role = Roles::ObjectRole) const
		-> QVariant override
	{
		if (!index.isValid())
			return {};

		const Workspace* val = this->listData[index.row()]; // NOLINT
		switch (role)
		{
		case Roles::ObjectRole:
			return QVariant::fromValue(val);
		default:
			return {};
		}
	}
	auto rowCount(const QModelIndex& /*parent*/ = QModelIndex()) const
		-> int override
	{
		return this->listData.count(); // NOLINT
	}
	void Reset(QList<Workspace*>& newList)
	{
		this->beginResetModel();
		this->listData = newList;
		this->endResetModel();
	}

	Q_INVOKABLE void move(int target, int dest)
	{
		if (target == dest)
			return;

		if (target >= 0 && target < this->rowCount())
		{
			if (dest >= 0 && dest < this->rowCount())
			{
				if (target == dest - 1)
					dest = target++;
				this->beginResetModel();
				this->listData.move(target, dest);
				this->endResetModel();
				// this->beginMoveRows(QModelIndex(), target, target,
				// 					QModelIndex(), dest);
				// this->endMoveRows();
			}
		}
	}

	private:
	QList<Workspace*> listData;
};
