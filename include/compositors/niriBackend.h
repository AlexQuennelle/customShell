#pragma once

#include "compositorBackend.h"

#include <QLocalSocket>

namespace niri
{

class NiriBackend : public ICompositorBackend
{
	Q_OBJECT; // NOLINT

	public:
	NiriBackend();
	NiriBackend(const NiriBackend&) = delete;
	NiriBackend(NiriBackend&&) = delete;
	~NiriBackend() override = default;

	auto Workspaces(const QString& outputName) -> QList<Workspace*>;

	auto operator=(const NiriBackend&) -> NiriBackend& = delete;
	auto operator=(NiriBackend&&) -> NiriBackend& = delete;

	signals:
	void WorkspacesChanged(const QString& output,
						   QList<Workspace*>& workspaces);

	private:
	void ReadMessages();
	void ProcessMessage(const std::string_view message);

	std::unordered_map<uint64_t, WindowInfo> windows;
	std::unordered_map<uint64_t, Workspace> workspaces;
	QHash<QString, QList<Workspace*>> workspaceGroups;
	QHash<QString, uint64_t> activeWorkspaces;
	QLocalSocket eventSock;
	QLocalSocket cmdSock;
	uint64_t focusedWorkspaceID{};
};

} //namespace niri
