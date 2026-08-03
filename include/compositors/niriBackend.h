#pragma once

#include "compositors/compositorBackend.h"

#include <QLocalSocket>

class NiriBackend : public ICompositorBackend
{
	Q_OBJECT; // NOLINT

	public:
	NiriBackend();
	~NiriBackend() override = default;

	private:
	void ReadMessages();
	void ProcessMessage(const std::string_view message);

	QLocalSocket eventSock;
	QLocalSocket cmdSock;
};
