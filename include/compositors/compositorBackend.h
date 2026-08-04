#pragma once

#include <QObject>

class WindowInfo : public QObject
{
	Q_OBJECT; // NOLINT

	Q_PROPERTY(QString title READ GetTitle NOTIFY TitleChanged FINAL)
	Q_PROPERTY(QString appID READ GetAppID NOTIFY AppIDChanged FINAL)

	public:
	WindowInfo() = default;
	WindowInfo(QString title, QString appID);
	WindowInfo(const WindowInfo&) = delete;
	WindowInfo(WindowInfo&&) = delete;

	auto GetTitle() const -> const QString&;
	auto GetAppID() const -> const QString&;

	void SetTitle(const QString& newTitle);
	void SetAppID(const QString& newAppID);

	auto operator=(const WindowInfo& other) -> WindowInfo& = delete;
	auto operator=(WindowInfo&& other) noexcept -> WindowInfo&;

	signals:
	void TitleChanged(const QString& title);
	void AppIDChanged(const QString& appID);

	private:
	QString title{};
	QString appID{};
};

class IWorkspaceManager : public QObject
{
	Q_OBJECT; // NOLINT

	public:
	virtual ~IWorkspaceManager() = default;

	virtual auto GetWorkspaces(QStringView outputName) -> QList<QObject*> = 0;
	virtual void CreateWorkspace(QStringView name) = 0;
	virtual void RemoveWorkspace(QStringView id) = 0;
	virtual void SetWorkspaceOutput(QStringView id, QStringView outputName) = 0;
	virtual void SetWorkspaceName(QStringView id, QStringView name) = 0;
	virtual void SetWorkspaceIndex(QStringView id, uint64_t index) = 0;
	virtual void ActivateWorkspace(QStringView id) = 0;

	signals:
	void WorkspacesChanged();
};
Q_DECLARE_INTERFACE(IWorkspaceManager, "WorkspaceManagerInterfaceClass")
class ICompositorBackend : public QObject
{
	Q_OBJECT; // NOLINT

	public:
	virtual ~ICompositorBackend() = default;

	virtual auto GetActiveWindow(QStringView outputName) -> WindowInfo& = 0;

	signals:
	// void ActiveWindowChanged(WindowData* window);
};
Q_DECLARE_INTERFACE(ICompositorBackend, "CompositorInterfaceClass")

class Workspace : public QObject
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
