#pragma once

#include "compositors/compositorBackend.h"

#include <QLocalSocket>

class NiriBackend : public ICompositorBackend
{
	Q_OBJECT

	public:
	NiriBackend();
	~NiriBackend() override = default;

	private:
	void ProcessEvent();

	QLocalSocket eventSock;
	QLocalSocket cmdSock;
};
