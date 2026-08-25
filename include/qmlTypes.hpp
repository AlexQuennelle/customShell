#pragma once

#include <QObject>
#include <QtQuick>
#include <qqml.h>

class WorkspaceButton : public QQuickItem
{
	Q_OBJECT;	 // NOLINT
	QML_ELEMENT; // NOLINT

	Q_PROPERTY(bool hovered READ IsHovered NOTIFY HoverChanged); // NOLINT

	public:
	WorkspaceButton(QQuickItem* parent = nullptr) : QQuickItem(parent)
	{
		this->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
		this->setAcceptHoverEvents(true);
	}

	auto IsHovered() const -> bool { return this->hovered; }

	signals:
	void HoverChanged();

	protected:
	void mousePressEvent(QMouseEvent* event) override { };
	void mouseReleaseEvent(QMouseEvent* event) override { };
	void hoverEnterEvent(QHoverEvent* event) override
	{
		this->hovered = true;
		emit this->HoverChanged();
		event->accept();
	};
	void hoverLeaveEvent(QHoverEvent* event) override
	{
		this->hovered = false;
		emit this->HoverChanged();
		event->accept();
	};

	private:
	bool hovered{false};
};
