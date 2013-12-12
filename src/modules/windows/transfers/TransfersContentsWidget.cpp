#include "TransfersContentsWidget.h"
#include "ProgressBarDelegate.h"
#include "../../../core/ActionsManager.h"
#include "../../../core/Utils.h"

#include "ui_TransfersContentsWidget.h"

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QtMath>
#include <QtCore/QQueue>
#include <QtGui/QClipboard>
#include <QtGui/QDesktopServices>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>

namespace Otter
{

TransfersContentsWidget::TransfersContentsWidget(Window *window) : ContentsWidget(window),
	m_model(new QStandardItemModel(this)),
	m_ui(new Ui::TransfersContentsWidget)
{
	m_ui->setupUi(this);

	QStringList labels;
	labels << tr("Status") << tr("Filename") << tr("Size") << tr("Progress") << tr("Time") << tr("Speed") << tr("Started") << tr("Finished");

	m_model->setHorizontalHeaderLabels(labels);

	m_ui->transfersView->setModel(m_model);
	m_ui->transfersView->horizontalHeader()->setTextElideMode(Qt::ElideRight);
	m_ui->transfersView->horizontalHeader()->resizeSection(0, 30);
	m_ui->transfersView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	m_ui->transfersView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	m_ui->transfersView->setItemDelegateForColumn(3, new ProgressBarDelegate(this));

	const QList<TransferInformation*> transfers = TransfersManager::getTransfers();

	for (int i = 0; i < transfers.count(); ++i)
	{
		addTransfer(transfers.at(i));
	}

	connect(m_ui->transfersView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(updateActions()));
	connect(m_ui->transfersView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openTransfer(QModelIndex)));
	connect(m_ui->transfersView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
	connect(m_ui->stopResumeButton, SIGNAL(clicked()), this, SLOT(stopResumeTransfer()));
	connect(m_ui->downloadLineEdit, SIGNAL(returnPressed()), this, SLOT(startQuickTransfer()));
	connect(TransfersManager::getInstance(), SIGNAL(transferStarted(TransferInformation*)), this, SLOT(addTransfer(TransferInformation*)));
	connect(TransfersManager::getInstance(), SIGNAL(transferRemoved(TransferInformation*)), this, SLOT(removeTransfer(TransferInformation*)));
	connect(TransfersManager::getInstance(), SIGNAL(transferUpdated(TransferInformation*)), this, SLOT(updateTransfer(TransferInformation*)));
}

TransfersContentsWidget::~TransfersContentsWidget()
{
	delete m_ui;
}

void TransfersContentsWidget::changeEvent(QEvent *event)
{
	QWidget::changeEvent(event);

	switch (event->type())
	{
		case QEvent::LanguageChange:
			m_ui->retranslateUi(this);

			break;
		default:
			break;
	}
}

void TransfersContentsWidget::addTransfer(TransferInformation *transfer)
{
	QList<QStandardItem*> items;
	QStandardItem *item = new QStandardItem();
	item->setData(qVariantFromValue((void*) transfer), Qt::UserRole);

	items.append(item);

	item = new QStandardItem(QFileInfo(transfer->target).fileName());

	items.append(item);

	for (int i = 2; i < m_model->columnCount(); ++i)
	{
		items.append(new QStandardItem());
	}

	m_model->appendRow(items);

	if (transfer->state == RunningTransfer)
	{
		m_speeds[transfer] = QQueue<qint64>();
	}

	updateTransfer(transfer);
}

void TransfersContentsWidget::removeTransfer(TransferInformation *transfer)
{
	const int row = findTransfer(transfer);

	if (row >= 0)
	{
		m_model->removeRow(row);
	}

	m_speeds.remove(transfer);
}

void TransfersContentsWidget::removeTransfer()
{
	TransferInformation *transfer = getTransfer(m_ui->transfersView->currentIndex());

	if (transfer)
	{
		if (transfer->state == RunningTransfer && QMessageBox::warning(this, tr("Warning"), tr("This transfer is still running.\nDo you really want to remove it?"), (QMessageBox::Yes | QMessageBox::Cancel)) == QMessageBox::Cancel)
		{
			return;
		}

		TransfersManager::removeTransfer(transfer);
	}
}

void TransfersContentsWidget::updateTransfer(TransferInformation *transfer)
{
	const int row = findTransfer(transfer);

	if (row < 0)
	{
		return;
	}

	QString remainingTime;

	if (m_speeds.contains(transfer))
	{
		if (transfer->state == RunningTransfer)
		{
			m_speeds[transfer].enqueue(transfer->speed);

			if (m_speeds[transfer].count() > 10)
			{
				m_speeds[transfer].dequeue();
			}

			if (transfer->bytesTotal > 0)
			{
				qint64 speedSum = 0;
				const QList<qint64> speeds = m_speeds[transfer];

				for (int i = 0; i < speeds.count(); ++i)
				{
					speedSum += speeds.at(i);
				}

				speedSum /= (speeds.count());

				remainingTime = Utils::formatTime(qreal(transfer->bytesTotal - transfer->bytesReceived) / speedSum);
			}
		}
		else
		{
			m_speeds.remove(transfer);
		}
	}

	QIcon icon;

	switch (transfer->state)
	{
		case RunningTransfer:
			icon = Utils::getIcon("task-ongoing");

			break;
		case FinishedTransfer:
			icon = Utils::getIcon("task-complete");

			break;
		case ErrorTransfer:
			icon = Utils::getIcon("task-reject");

			break;
		default:
			break;
	}

	m_model->item(row, 0)->setIcon(icon);
	m_model->item(row, 2)->setText(Utils::formatUnit(transfer->bytesTotal, false, 1));
	m_model->item(row, 3)->setText((transfer->bytesTotal > 0) ? QString::number(qFloor(((qreal) transfer->bytesReceived / transfer->bytesTotal) * 100), 'f', 0) : QString());
	m_model->item(row, 4)->setText(remainingTime);
	m_model->item(row, 5)->setText((transfer->state == RunningTransfer) ? Utils::formatUnit(transfer->speed, true, 1) : QString());
	m_model->item(row, 6)->setText(transfer->started.toString("yyyy-MM-dd HH:mm:ss"));
	m_model->item(row, 7)->setText(transfer->finished.toString("yyyy-MM-dd HH:mm:ss"));

	const QString tooltip = tr("<pre style='font-family:auto;'>Source: %1\nTarget: %2\nSize: %3\nDownloaded: %4\nProgress: %5</pre>").arg(transfer->source.toHtmlEscaped()).arg(transfer->target.toHtmlEscaped()).arg((transfer->bytesTotal > 0) ? tr("%1 (%n B)", "", transfer->bytesTotal).arg(Utils::formatUnit(transfer->bytesTotal)) : QString('?')).arg(tr("%1 (%n B)", "", transfer->bytesReceived).arg(Utils::formatUnit(transfer->bytesReceived))).arg(QString("%1%").arg(((transfer->bytesTotal > 0) ? (((qreal) transfer->bytesReceived / transfer->bytesTotal) * 100) : 0.0), 0, 'f', 1));

	for (int i = 0; i < m_model->columnCount(); ++i)
	{
		m_model->item(row, i)->setToolTip(tooltip);
	}

	if (m_ui->transfersView->selectionModel()->hasSelection())
	{
		updateActions();
	}
}

void TransfersContentsWidget::openTransfer(const QModelIndex &index)
{
	TransferInformation *transfer = getTransfer(index.isValid() ? index : m_ui->transfersView->currentIndex());

	if (transfer)
	{
		QDesktopServices::openUrl(QUrl(transfer->target, QUrl::TolerantMode));
	}
}

void TransfersContentsWidget::openTransferFolder(const QModelIndex &index)
{
	TransferInformation *transfer = getTransfer(index.isValid() ? index : m_ui->transfersView->currentIndex());

	if (transfer)
	{
		QDesktopServices::openUrl(QUrl(QFileInfo(transfer->target).dir().canonicalPath(), QUrl::TolerantMode));
	}
}

void TransfersContentsWidget::copyTransferInformation()
{
	QStandardItem *item = m_model->itemFromIndex(m_ui->transfersView->currentIndex());

	if (item)
	{
		QApplication::clipboard()->setText(item->toolTip().remove(QRegExp("<[^>]*>")));
	}
}

void TransfersContentsWidget::stopResumeTransfer()
{
	TransferInformation *transfer = getTransfer(m_ui->transfersView->selectionModel()->hasSelection() ? m_ui->transfersView->selectionModel()->currentIndex() : QModelIndex());

	if (transfer)
	{
		if (transfer->state == RunningTransfer)
		{
			TransfersManager::stopTransfer(transfer);
		}
		else if (transfer->state == ErrorTransfer)
		{
			TransfersManager::resumeTransfer(transfer);
		}

		updateActions();
	}
}

void TransfersContentsWidget::startQuickTransfer()
{
	TransfersManager::startTransfer(m_ui->downloadLineEdit->text(), QString(), false, true);

	m_ui->downloadLineEdit->clear();
}

void TransfersContentsWidget::clearFinishedTransfers()
{
	const QList<TransferInformation*> transfers = TransfersManager::getTransfers();

	for (int i = 0; i < transfers.count(); ++i)
	{
		if (transfers.at(i)->state == FinishedTransfer)
		{
			TransfersManager::removeTransfer(transfers.at(i));
		}
	}
}

void TransfersContentsWidget::showContextMenu(const QPoint &point)
{
	TransferInformation *transfer = getTransfer(m_ui->transfersView->indexAt(point));
	QMenu menu(this);

	if (transfer)
	{
		menu.addAction(tr("Open"), this, SLOT(openTransfer()));
		menu.addAction(tr("Open Folder"), this, SLOT(openTransferFolder()));
		menu.addSeparator();
		menu.addAction(((transfer->state == ErrorTransfer) ? tr("Resume") : tr("Stop")), this, SLOT(stopResumeTransfer()))->setEnabled(transfer->state == RunningTransfer || transfer->state == ErrorTransfer);
		menu.addSeparator();
		menu.addAction(tr("Remove"), this, SLOT(removeTransfer()));
	}

	const QList<TransferInformation*> transfers = TransfersManager::getTransfers();
	int finishedTransfers = 0;

	for (int i = 0; i < transfers.count(); ++i)
	{
		if (transfers.at(i)->state == FinishedTransfer)
		{
			++finishedTransfers;
		}
	}

	menu.addAction(tr("Clear Finished Transfers"), this, SLOT(clearFinishedTransfers()))->setEnabled(finishedTransfers > 0);

	if (transfer)
	{
		menu.addSeparator();
		menu.addAction(tr("Copy Transfer Information"), this, SLOT(copyTransferInformation()));
	}

	menu.exec(m_ui->transfersView->mapToGlobal(point));
}

void TransfersContentsWidget::updateActions()
{
	TransferInformation *transfer = getTransfer(m_ui->transfersView->selectionModel()->hasSelection() ? m_ui->transfersView->selectionModel()->currentIndex() : QModelIndex());

	m_ui->stopResumeButton->setEnabled(transfer && (transfer->state == RunningTransfer || transfer->state == ErrorTransfer));
	m_ui->stopResumeButton->setText((transfer && transfer->state == ErrorTransfer) ? tr("Resume") : tr("Stop"));

	getAction(CopyAction)->setEnabled(transfer);
	getAction(DeleteAction)->setEnabled(transfer);

	if (transfer)
	{
		m_ui->sourceValueLabel->setText(transfer->source.toHtmlEscaped());
		m_ui->targetValueLabel->setText(transfer->target.toHtmlEscaped());
		m_ui->sizeValueLabel->setText((transfer->bytesTotal > 0) ? tr("%1 (%n B)", "", transfer->bytesTotal).arg(Utils::formatUnit(transfer->bytesTotal)) : QString('?'));
		m_ui->downloadedValueLabel->setText(tr("%1 (%n B)", "", transfer->bytesReceived).arg(Utils::formatUnit(transfer->bytesReceived)));
		m_ui->progressValueLabel->setText(QString("%1%").arg(((transfer->bytesTotal > 0) ? (((qreal) transfer->bytesReceived / transfer->bytesTotal) * 100) : 0.0), 0, 'f', 1));
	}
	else
	{
		m_ui->sourceValueLabel->clear();
		m_ui->targetValueLabel->clear();
		m_ui->sizeValueLabel->clear();
		m_ui->downloadedValueLabel->clear();
		m_ui->progressValueLabel->clear();
	}

	emit actionsChanged();
}

void TransfersContentsWidget::print(QPrinter *printer)
{
	m_ui->transfersView->render(printer);
}

void TransfersContentsWidget::triggerAction(WindowAction action, bool checked)
{
	Q_UNUSED(checked)

	switch (action)
	{
		case CopyAction:
			copyTransferInformation();

			break;
		case DeleteAction:
			removeTransfer();

			break;
		default:
			break;
	}
}

void TransfersContentsWidget::setHistory(const HistoryInformation &history)
{
	Q_UNUSED(history)
}

void TransfersContentsWidget::setZoom(int zoom)
{
	Q_UNUSED(zoom)
}

void TransfersContentsWidget::setUrl(const QUrl &url)
{
	Q_UNUSED(url)
}

TransferInformation* TransfersContentsWidget::getTransfer(const QModelIndex &index)
{
	if (index.isValid() && m_model->item(index.row(), 0))
	{
		return static_cast<TransferInformation*>(m_model->item(index.row(), 0)->data(Qt::UserRole).value<void*>());
	}

	return NULL;
}

ContentsWidget* TransfersContentsWidget::clone(Window *window)
{
	Q_UNUSED(window)

	return NULL;
}

QAction* TransfersContentsWidget::getAction(WindowAction action)
{
	if (m_actions.contains(action))
	{
		return m_actions[action];
	}

	QAction *actionObject = new QAction(this);
	actionObject->setData(action);

	connect(actionObject, SIGNAL(triggered()), this, SLOT(triggerAction()));

	switch (action)
	{
		case CopyAction:
			ActionsManager::setupLocalAction(actionObject, "Copy");

			break;
		case DeleteAction:
			ActionsManager::setupLocalAction(actionObject, "Delete");

			break;
		default:
			actionObject->deleteLater();
			actionObject = NULL;

			break;
	}

	if (actionObject)
	{
		m_actions[action] = actionObject;
	}

	return actionObject;
}

QUndoStack* TransfersContentsWidget::getUndoStack()
{
	return NULL;
}

QString TransfersContentsWidget::getTitle() const
{
	return tr("Transfers Manager");
}

QString TransfersContentsWidget::getType() const
{
	return "transfers";
}

QUrl TransfersContentsWidget::getUrl() const
{
	return QUrl("about:transfers");
}

QIcon TransfersContentsWidget::getIcon() const
{
	return QIcon(":/icons/transfers.png");
}

QPixmap TransfersContentsWidget::getThumbnail() const
{
	return QPixmap();
}

HistoryInformation TransfersContentsWidget::getHistory() const
{
	HistoryEntry entry;
	entry.url = getUrl().toString();
	entry.title = getTitle();
	entry.position = QPoint(0, 0);
	entry.zoom = 100;

	HistoryInformation information;
	information.index = 0;
	information.entries.append(entry);

	return information;
}

int TransfersContentsWidget::getZoom() const
{
	return 100;
}

int TransfersContentsWidget::findTransfer(TransferInformation *transfer) const
{
	if (!transfer)
	{
		return -1;
	}

	for (int i = 0; i < m_model->rowCount(); ++i)
	{
		if (transfer == static_cast<TransferInformation*>(m_model->item(i, 0)->data(Qt::UserRole).value<void*>()))
		{
			return i;
		}
	}

	return -1;
}

void TransfersContentsWidget::triggerAction()
{
	QAction *action = qobject_cast<QAction*>(sender());

	if (action)
	{
		triggerAction(static_cast<WindowAction>(action->data().toInt()));
	}
}

bool TransfersContentsWidget::canZoom() const
{
	return false;
}

bool TransfersContentsWidget::isClonable() const
{
	return false;
}

bool TransfersContentsWidget::isLoading() const
{
	return false;
}

bool TransfersContentsWidget::isPrivate() const
{
	return false;
}

}