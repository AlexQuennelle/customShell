#pragma once

#include <QObject>
#include <qqml.h>
#include <qqmlintegration.h>

class WindowInfo : public QObject
{
	Q_OBJECT;	 // NOLINT
	QML_ELEMENT; // NOLINT
	QML_UNCREATABLE("");

	Q_PROPERTY(QString title READ GetTitle NOTIFY TitleChanged FINAL)
	Q_PROPERTY(QString appID READ GetAppID NOTIFY AppIDChanged FINAL); // NOLINT

	public:
	WindowInfo() = default;
	WindowInfo(QString title, QString appID,
			   std::optional<uint64_t> workspace = {});
	WindowInfo(const WindowInfo&) = delete;
	WindowInfo(WindowInfo&&) = delete;
	~WindowInfo() override = default;

	auto GetTitle() const -> const QString&;
	auto GetAppID() const -> const QString&;
	auto GetWorkspace() const -> std::optional<uint64_t>;

	void SetTitle(const QString& newTitle);
	void SetAppID(const QString& newAppID);
	void SetWorkspace(const std::optional<uint64_t> id);

	auto operator=(const WindowInfo& other) -> WindowInfo&;
	auto operator=(WindowInfo&& other) noexcept -> WindowInfo&;

	signals:
	void TitleChanged(const QString& title);
	void AppIDChanged(const QString& appID);

	private:
	QString title{};
	QString appID{};
	std::optional<uint64_t> workspace;
};

class Workspace : public QObject
{
	Q_OBJECT; // NOLINT

	QML_ELEMENT
	QML_UNCREATABLE("")

	Q_PROPERTY(QString id READ GetID)
	Q_PROPERTY(QString name READ GetName NOTIFY nameChanged)
	Q_PROPERTY(uint8_t index READ GetIndex NOTIFY indexChanged)
	Q_PROPERTY(bool urgent READ GetUrgent NOTIFY urgentChanged)
	Q_PROPERTY(bool active READ GetActive NOTIFY activeChanged)
	Q_PROPERTY(bool focused READ GetFocused NOTIFY focusedChanged)
	Q_PROPERTY(bool empty READ GetEmpty NOTIFY emptyChanged)

	public:
	Workspace(QString id, QString name, QString output,
			  std::optional<uint64_t> windowID, uint8_t index, bool urgent,
			  bool active, bool focused);

	auto GetID() const -> const QString&;
	auto GetName() const -> const QString&;
	auto GetOutput() const -> const QString&;
	auto GetActiveWindowID() const -> std::optional<uint64_t>;
	auto GetIndex() const -> uint8_t;
	auto GetUrgent() const -> bool;
	auto GetActive() const -> bool;
	auto GetFocused() const -> bool;
	auto GetEmpty() const -> bool;
	auto GetDead() const -> bool;

	void SetName(const QString& name);
	void SetOutput(const QString& output);
	void SetActiveWindowId(const std::optional<uint64_t> windowID);
	void SetIndex(uint8_t index);
	void SetUrgent(bool urgent);
	void SetActive(bool active);
	void AddWindow(uint64_t id);
	void RemoveWindow(uint64_t id);
	void SetFocused(bool focused);

	void SetDead();

	signals:
	void nameChanged(QString newName);
	void indexChanged(uint8_t newIndex);
	void urgentChanged(bool newUrgent);
	void activeChanged(bool newActive);
	void focusedChanged(bool newFocused);
	void emptyChanged(bool newEmpty);

	private:
	std::unordered_set<uint64_t> windows;
	QString id;
	QString name;
	QString output;
	std::optional<uint64_t> activeWindowID;
	uint8_t index{};
	bool urgent{};
	bool active{};
	bool focused{};
	bool dead{false};
};

class IWorkspaceManager : public QObject // NOLINT
{
	Q_OBJECT; // NOLINT

	public:
	~IWorkspaceManager() override = default;

	virtual auto GetWorkspaces(const QString& outputName)
		-> QList<Workspace*> = 0;
	virtual void CreateWorkspace(const QString& name) = 0;
	virtual void RemoveWorkspace(const QString& id) = 0;
	virtual void SetWorkspaceOutput(const QString& id,
									const QString& outputName) = 0;
	virtual void SetWorkspaceName(const QString& id, const QString& name) = 0;
	virtual void SetWorkspaceIndex(const QString& id, uint64_t index) = 0;
	virtual void ActivateWorkspace(const QString& id) = 0;

	signals:
	void WorkspacesChanged(const QString& output,
						   QList<Workspace*>& workspaces);
};
Q_DECLARE_INTERFACE(IWorkspaceManager, "WorkspaceManagerInterfaceClass")
class ICompositorBackend : public QObject // NOLINT
{
	Q_OBJECT; // NOLINT

	public:
	~ICompositorBackend() override = default;

	// virtual auto GetActiveWindow(const QString& outputName) -> WindowInfo& =
	// 0;

	signals:
	void ActiveWindowChanged(const QString& output,
							 std::optional<WindowInfo&> window);
};
Q_DECLARE_INTERFACE(ICompositorBackend, "CompositorInterfaceClass")
