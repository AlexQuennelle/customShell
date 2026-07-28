#pragma once

#include "compositors/compositorBackend.h"
#include "compositors/niriBackend.h"
#include "workspace/extWorkspaceManager.h"

#include <QDebug>
#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration>

class ShellBackend : public QObject
{
	Q_OBJECT
	QML_ELEMENT
	QML_SINGLETON
	Q_PROPERTY(bool IsTest READ IsTest WRITE SetTest)

	public:
	static auto Instance() -> ShellBackend&
	{
		static ShellBackend instance{};
		return instance;
	}
	static auto create(QQmlEngine* qmlEngine, QJSEngine* jsEngine)
		-> ShellBackend*
	{
		Q_UNUSED(qmlEngine)
		Q_UNUSED(jsEngine)
		return &Instance();
	}

	auto IsTest() const -> bool { return test; }
	void SetTest(bool value) { test = value; }

	void Update() { };

	Q_INVOKABLE void TestPrint() { qDebug("Hello"); }

	signals:
	void TestSignal();

	private:
	ShellBackend(QObject* parent = nullptr) : QObject(parent)
	{
		this->compositor = std::make_unique<NiriBackend>();
		workspaceManager = std::make_unique<WLWorkspaceManager>();
	};

	std::unique_ptr<ICompositorBackend> compositor{nullptr};
	std::unique_ptr<IWorkspaceManager> workspaceManager{nullptr};
	bool test{false};
};
