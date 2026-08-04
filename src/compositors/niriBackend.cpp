#include "compositors/niriBackend.h"
#include "compositors/compositorBackend.h"
#include "compositors/niriTypes.hpp"

#include <qdebug.h>

namespace niri
{

NiriBackend::NiriBackend() : eventSock(this), cmdSock(this)
{
	QString sockAddr = std::getenv("NIRI_SOCKET");

	connect(&eventSock, &QLocalSocket::connected,
			[this, &sockAddr]() -> void
			{
				qDebug() << "Connecting to" << sockAddr;
				if (eventSock.write("\"EventStream\"\n") == -1)
				{
					qFatal() << "ERROR: failed to connect to Niri event stream";
				}
			});
	connect(&eventSock, &QLocalSocket::readyRead, this,
			&NiriBackend::ReadMessages);
	eventSock.connectToServer(sockAddr);

	connect(&cmdSock, &QLocalSocket::connected,
			[this]() -> void
			{
				// cmdSock.write("{\"Action\":{\"FocusWorkspace\":{\"reference\":{"
				// 			  "\"Index\":1}}}}\n");
			});
	cmdSock.connectToServer(sockAddr);
}

void NiriBackend::ReadMessages()
{
	QByteArray line;
	while (this->eventSock.canReadLine())
	{
		line = this->eventSock.readLine();
		ProcessMessage(line);
	}
}
void NiriBackend::ProcessMessage(const std::string_view message)
{
	niri::Response response;
	auto err = glz::read<glzOpts{}>(response, message);
	if (err.ec != glz::error_code::none)
	{
#ifndef __clangd__
		// qDebug() << glz::enum_to_string<glz::error_code>(err.ec);
#endif
	}
	else
	{
		auto out = glz::write<glzOpts{}>(response).value_or("ERROR");
		qDebug().noquote() << out;
	}
	niri::Event event;
	err = glz::read<glzOpts{}>(event, message);
	if (err.ec != glz::error_code::none)
	{
#ifndef __clangd__
		// qDebug() << glz::enum_to_string<glz::error_code>(err.ec);
#endif
	}
	else
	{
		auto eventSwitch = Overload{
			[this](WindowOpenedOrChangedEvent& event) -> void
			{
				auto& window{event.window};
				this->windows[window.id]
					= WindowInfo(window.title.value_or("").c_str(),
								  window.app_id.value_or("").c_str());
				qDebug() << "New Window";
			},
			[](WindowFocusChangedEvent& event) -> void
			{ qDebug() << event.id.value_or(0); },
			[](auto) -> void { }, // NOLINT
		};
		std::visit(eventSwitch, event.event);
		// auto out = glz::write<glzOpts{}>(event).value_or("ERROR");
		// qDebug().noquote() << out;
	} //namespace niri
}

} //namespace niri
