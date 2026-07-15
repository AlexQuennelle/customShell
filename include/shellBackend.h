#pragma once

#include <qobject.h>
#include <qqmlintegration.h>

class ShellBackend : public QObject
{
	Q_OBJECT

	Q_PROPERTY(bool IsTest READ IsTest WRITE SetTest)
	// QML_ELEMENT
	// QML_SINGLETON;

	public:
	static auto Instance() -> ShellBackend&
	{
		static ShellBackend instance{};
		return instance;
	}

	auto IsTest() const -> bool { return test; }
	void SetTest(bool value) { test = value; }

	signals:

	private:
	ShellBackend(QObject* parent = nullptr) : QObject(parent) { };

	bool test{false};
};
