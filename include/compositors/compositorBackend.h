#pragma once

#include <QObject>

class IWorkspaceManager : public QObject
{ };
class ICompositorBackend : public QObject
{ };

class Workspace : QObject
{
	Q_OBJECT; // NOLINT

	Q_PROPERTY(QString id READ GetID)
	Q_PROPERTY(QString name READ GetName NOTIFY NameChanged)
	Q_PROPERTY(uint8_t index READ GetIndex NOTIFY IndexChanged)
	Q_PROPERTY(bool urgent READ GetUrgent NOTIFY UrgentChanged)
	Q_PROPERTY(bool active READ GetActive NOTIFY ActiveChanged)
	Q_PROPERTY(bool focused READ GetFocused NOTIFY FocusedChanged)

	public:
	auto GetID() -> QString { return this->id; }
	auto GetName() -> QString { return this->name; }
	auto GetIndex() -> uint8_t { return this->index; }
	auto GetUrgent() -> bool { return this->urgent; }
	auto GetActive() -> bool { return this->active; }
	auto GetFocused() -> bool { return this->focused; }

	signals:
	void NameChanged(QString newName);
	void IndexChanged(uint8_t newIndex);
	void UrgentChanged(bool newUrgent);
	void ActiveChanged(bool newActive);
	void FocusedChanged(bool newFocused);

	private:
	QString id;
	QString name;
	uint8_t index;
	bool urgent;
	bool active;
	bool focused;
};
