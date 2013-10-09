/*
 * ReplacementEngine.h
 *
 *  Created on: Jul 21, 2012
 *      Author: Adam
 */

#ifndef REPLACEMENTENGINE_H_
#define REPLACEMENTENGINE_H_

#include <QList>
#include <QString>

#include "ReplacementItem.h"

class ReplacementEngine
{
public:
	ReplacementEngine();
	virtual ~ReplacementEngine();

	QList<ReplacementItem*> list() const;
	void clear();
	QString serialize();
	void add(ReplacementItem *item);
	void removeAt(int i);
	QString doReplacements(QString str);
	void swap(int i, int j);
	void readFromFile(QString filename);

	void processFile( QString input, QString output );

private:
	QList<ReplacementItem*> mList;
};

#endif /* REPLACEMENTENGINE_H_ */
