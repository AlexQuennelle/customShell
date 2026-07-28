#include "compositors/niriBackend.h"
#include "compositors/niriTypes.h"

#include <meta>
#include <qdebug.h>

NiriBackend::NiriBackend() : eventSock(this), cmdSock(this)
{
	QString sockAddr = std::getenv("NIRI_SOCKET");

	std::string testStr = R"({"WorkspaceActivated":{"foc0sed":true}})";
	// std::string testStr = R"({"Ok":{"Version":"bloop"}})";
	// std::string testStr = R"({"Ok":"Handled"})";

	// niri::Message expectedRes
	// = niri::Event(niri::WorkspaceActivated{.id = 1});
	niri::Event expectedRes{.event = niri::WorkspaceActivatedEvent()};
	// = niri::Response{.data = niri::OkResponse(std::string("Handled"))};
	auto garbage = glz::write<glzOpts{}>(expectedRes);
	qDebug() << testStr << garbage.value_or("Error Writing");
	niri::Response testRes;
	auto err = glz::read<glzOpts{}>(testRes, testStr);
	if (err.ec != glz::error_code::none)
	{
		qDebug() << "Is not Response";
		qDebug() << "Error:" << err.custom_error_message;
#ifndef __clangd__
		qDebug() << glz::enum_to_string<glz::error_code>(err.ec);
#endif
		niri::Event testEv;
		err = glz::read<glzOpts{}>(testEv, testStr);
		if (err.ec != glz::error_code::none)
		{
			qDebug() << "Is not Event";
			qDebug() << "Error:" << err.custom_error_message;
#ifndef __clangd__
			qDebug() << glz::enum_to_string<glz::error_code>(err.ec);
#endif
		}
		else
		{
			qDebug() << "Is Event:" << testEv.event.index();
			assert(testEv.event.index() == 2);
		}
	}
	else
	{
		qDebug() << "Is Response";
	}

	assert(false && "Break");

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
			&NiriBackend::ProcessEvent);
	eventSock.connectToServer(sockAddr);

	connect(&cmdSock, &QLocalSocket::connected,
			[this]() -> void
			{
				// cmdSock.write("{\"Action\":{\"FocusWorkspace\":{\"reference\":{"
				// 			  "\"Index\":1}}}}\n");
			});
	cmdSock.connectToServer(sockAddr);
}

void NiriBackend::ProcessEvent()
{
	// qDebug() << "New Event";
	QByteArray line;
	while (this->eventSock.canReadLine())
	{
		line = this->eventSock.readLine();
		// qDebug() << line;
	}
}
