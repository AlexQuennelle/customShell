#pragma once

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

	auto GetWorkspaces(const QString& outputName) -> QList<Workspace*> override;
	void CreateWorkspace(const QString& name) override;
	void RemoveWorkspace(const QString& id) override;
	void SetWorkspaceOutput(const QString& id,
							const QString& outputName) override;
	void SetWorkspaceName(const QString& id, const QString& name) override;
	void SetWorkspaceIndex(const QString& id, uint64_t index) override;
	void ActivateWorkspace(const QString& id) override;

	private:
	std::optional<niri::NiriBackend&> backend;
};

} //namespace niri
