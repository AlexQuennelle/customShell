#pragma once

#include <QObject>
#include <QtQuick>
#include <qqml.h>

class WorkspaceButton : public QQuickItem
{
	Q_OBJECT;	 // NOLINT
	QML_ELEMENT; // NOLINT

	Q_PROPERTY(bool hovered READ IsHovered NOTIFY hoverChanged); // NOLINT

	public:
	WorkspaceButton(QQuickItem* parent = nullptr) : QQuickItem(parent)
	{
		setFlag(QQuickItem::ItemHasContents, true);
		this->setAcceptHoverEvents(true);
		this->setAcceptedMouseButtons(Qt::LeftButton | Qt::RightButton);
	}

	auto IsHovered() const -> bool { return this->hovered; }

	signals:
	void clicked(Qt::MouseButton button);
	void mousePressed(Qt::MouseButton button);
	void mouseReleased();
	void hoverChanged();

	protected:
	void mousePressEvent(QMouseEvent* event) override
	{
		emit this->mousePressed(event->button());
		this->pressedButtons |= event->button();
		event->accept();
	};
	void mouseReleaseEvent(QMouseEvent* event) override
	{
		emit this->mouseReleased();

		if ((event->button() & this->pressedButtons) != 0)
		{
			emit this->clicked(event->button());
		}

		this->pressedButtons &= ~event->button();
		event->accept();
	};
	void hoverEnterEvent(QHoverEvent* event) override
	{
		this->hovered = true;
		emit this->hoverChanged();
		event->accept();
	};
	void hoverLeaveEvent(QHoverEvent* event) override
	{
		this->hovered = false;
		emit this->hoverChanged();
		event->accept();
	};

	private:
	Qt::MouseButtons pressedButtons{0};
	bool hovered{false};
};
