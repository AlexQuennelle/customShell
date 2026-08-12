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

	auto operator=(const NiriBackend&) -> NiriBackend& = delete;
	auto operator=(NiriBackend&&) -> NiriBackend& = delete;

	signals:
	void WorkspaceAddedOrModified(Workspace* workspace);

	private:
	void ReadMessages();
	void ProcessMessage(const std::string_view message);

	// auto GetActiveWindow(const QString& outputName) -> WindowInfo& override;

	// std::unordered_map<QString, WindowInfo> activeWindows;
	std::unordered_map<uint64_t, WindowInfo> windows;
	std::unordered_map<uint64_t, Workspace> workspaces;
	QLocalSocket eventSock;
	QLocalSocket cmdSock;
	uint64_t focusedWorkspaceID{};
};

} //namespace niri
