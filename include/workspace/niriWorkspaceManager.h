#pragma once

#include "compositors/compositorBackend.h"

namespace niri
{

class WorkspaceManager : public IWorkspaceManager
{
	Q_OBJECT; // NOLINT

	public:
	WorkspaceManager();
	WorkspaceManager(const WorkspaceManager&) = delete;
	WorkspaceManager(WorkspaceManager&&) = delete;
	~WorkspaceManager() override = default;

	auto operator=(const WorkspaceManager&) -> WorkspaceManager& = delete;
	auto operator=(WorkspaceManager&&) -> WorkspaceManager& = delete;

	auto GetWorkspaces(QStringView outputName) -> QList<QObject*> override;
	void CreateWorkspace(QStringView name) override;
	void RemoveWorkspace(QStringView id) override;
	void SetWorkspaceOutput(QStringView id, QStringView outputName) override;
	void SetWorkspaceName(QStringView id, QStringView name) override;
	void SetWorkspaceIndex(QStringView id, uint64_t index) override;
	void ActivateWorkspace(QStringView id) override;

	private:
	std::map<QString, std::vector<Workspace>> groups;
};

} //namespace niri
