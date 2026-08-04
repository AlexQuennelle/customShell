#pragma once

#include "compositors/compositorBackend.h"

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

	private:
	void ReadMessages();
	void ProcessMessage(const std::string_view message);

	auto GetActiveWindow(QStringView outputName) -> WindowInfo& override
	{
		return activeWindows.begin()->second;
	}

	std::map<QString, WindowInfo> activeWindows;
	std::map<uint64_t, WindowInfo> windows;
	QLocalSocket eventSock;
	QLocalSocket cmdSock;
};

} //namespace niri
