/**************************************************************************
* Otter Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2013 - 2014 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
* Copyright (C) 2014 Piotr Wójcik <chocimier@tlen.pl>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
**************************************************************************/

#ifndef OTTER_BOOKMARKSMANAGER_H
#define OTTER_BOOKMARKSMANAGER_H

#include <QtCore/QDateTime>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>
#include <QtGui/QStandardItemModel>

namespace Otter
{

enum BookmarkType
{
	FolderBookmark = 0,
	UrlBookmark = 1,
	SeparatorBookmark = 2
};

struct BookmarkInformation
{
	QString url;
	QString title;
	QString description;
	QString keyword;
	QDateTime added;
	QDateTime modified;
	QDateTime visited;
	QList<BookmarkInformation*> children;
	BookmarkType type;
	int identifier;
	int parent;
	int visits;

	BookmarkInformation() : type(FolderBookmark), identifier(-1), parent(-1), visits(0) {}
};

class BookmarksManager : public QObject
{
	Q_OBJECT

public:
	~BookmarksManager();

	static void createInstance(QObject *parent = NULL);
	static void updateVisit(const QUrl &url);
	static BookmarksManager* getInstance();
	static BookmarkInformation* getBookmark(const int identifier);
	static BookmarkInformation* getBookmark(const QString &keyword);
	static QStringList getKeywords();
	static QStringList getUrls();
	static QList<BookmarkInformation*> getBookmarks();
	static QList<BookmarkInformation*> getFolder(int folder = 0);
	static bool addBookmark(BookmarkInformation *bookmark, int folder = 0, int index = -1);
	static bool updateBookmark(BookmarkInformation *bookmark);
	static bool deleteBookmark(BookmarkInformation *bookmark, bool notify = true);
	static bool deleteBookmark(const QUrl &url);
	static bool hasBookmark(const QString &url);
	static bool hasBookmark(const QUrl &url);
	static bool save(const QString &path = QString());

protected:
	explicit BookmarksManager(QObject *parent = NULL);

	void timerEvent(QTimerEvent *event);
	void scheduleSave();
	static void writeBookmark(QXmlStreamWriter *writer, BookmarkInformation *bookmark);
	static void updateIndex();
	static void updateUrls();
	static void updateKeywords();
	BookmarkInformation* readBookmark(QXmlStreamReader *reader, int parent = -1);

protected slots:
	void load();

private:
	int m_saveTimer;

	static BookmarksManager *m_instance;
	static QHash<int, BookmarkInformation*> m_pointers;
	static QList<BookmarkInformation*> m_bookmarks;
	static QList<BookmarkInformation*> m_allBookmarks;
	static QSet<QString> m_urls;
	static QHash<QString, BookmarkInformation*> m_keywords;
	static int m_identifier;

signals:
	void folderModified(int folder);
};

}

#endif
