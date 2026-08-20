#pragma once

// #include "compositorBackend.h"
#include "compositors/niriBackend.h"

namespace niri
{

class WorkspaceManager : public IWorkspaceManager
{
	Q_OBJECT; // NOLINT

	public:
	WorkspaceManager(niri::NiriBackend& backend);
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
	std::optional<niri::NiriBackend&> backend;
	//		<Output	, Group		 <Workspace>>
	// std::unordered_map<QString, std::vector<Workspace>> groups;
	//		<ID		, Workspace>
	// std::unordered_map<QString, Workspace> workspaces;
};

} //namespace niri
