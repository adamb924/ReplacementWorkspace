/*
 * ReplacementEngine.cpp
 *
 *  Created on: Jul 21, 2012
 *      Author: Adam
 */

#include <QFile>
#include <QtXml>
#include <QRegExp>

#include "ReplacementEngine.h"
#include "ReadSettings.h"

ReplacementEngine::ReplacementEngine()
{
	// TODO Auto-generated constructor stub

}

ReplacementEngine::~ReplacementEngine()
{
	// TODO Auto-generated destructor stub
}

QList<ReplacementItem*> ReplacementEngine::list() const
{
	return mList;
}

void ReplacementEngine::clear()
{
	qDeleteAll(mList.begin(), mList.end());
	mList.clear();
}

QString ReplacementEngine::serialize()
{
	QString theXML;
	QXmlStreamWriter xmlstream(&theXML);
	xmlstream.setCodec("UTF-8");
	xmlstream.setAutoFormatting(true);
	xmlstream.writeStartDocument();
	xmlstream.writeStartElement("replacements");
	for (int i = 0; i < mList.length(); i++)
	{
		xmlstream.writeStartElement("item");
		xmlstream.writeAttribute("casesensitive", QString::number(mList[i]->cs));
		xmlstream.writeAttribute("recursive",
				QString::number(mList[i]->recursive));
		xmlstream.writeAttribute("regex", QString::number(mList[i]->regex));
		xmlstream.writeTextElement("replace", mList[i]->replace);
		xmlstream.writeTextElement("with", mList[i]->with);
		xmlstream.writeEndElement(); // item
	}
	xmlstream.writeEndDocument(); // replacements

	return theXML;
}

void ReplacementEngine::add(ReplacementItem *item)
{
	mList << item;
}

void ReplacementEngine::removeAt(int i)
{
	if( i < mList.count() )
		mList.removeAt(i);
}


QString ReplacementEngine::doReplacements(QString str)
{
	QString old;
	for (int i = 0; i < mList.length(); i++)
	{
		if (mList[i]->replace.length() == 0)
			continue;
		QString tmp = mList[i]->with;
		if (mList[i]->recursive)
		{
			do
			{
				old = str;
				if (mList[i]->regex)
				{
					tmp.replace("\\n", "\n");
					tmp.replace("\\r", "\r");
					tmp.replace("\\t", "\t");
					if (mList[i]->cs)
					{
						str.replace(
								QRegExp(mList[i]->replace, Qt::CaseSensitive,
										QRegExp::RegExp2), tmp);
					}
					else
					{
						str.replace(
								QRegExp(mList[i]->replace, Qt::CaseInsensitive,
										QRegExp::RegExp2), tmp);
					}
				}
				else
				{
					tmp.replace("\n", "\\n");
					tmp.replace("\r", "\\r");
					tmp.replace("\t", "\\t");
					if (mList[i]->cs)
					{
						str.replace(mList[i]->replace, mList[i]->with,
								Qt::CaseSensitive);
					}
					else
					{
						str.replace(mList[i]->replace, mList[i]->with,
								Qt::CaseInsensitive);
					}
				}
			} while (old != str);
		}
		else
		{
			if (mList[i]->regex)
			{
				tmp.replace("\\n", "\n");
				tmp.replace("\\r", "\r");
				tmp.replace("\\t", "\t");
				if (mList[i]->cs)
				{
					str.replace(
							QRegExp(mList[i]->replace, Qt::CaseSensitive,
									QRegExp::RegExp2), tmp);
				}
				else
				{
					str.replace(
							QRegExp(mList[i]->replace, Qt::CaseInsensitive,
									QRegExp::RegExp2), tmp);
				}
			}
			else
			{
				tmp.replace("\n", "\\n");
				tmp.replace("\r", "\\r");
				tmp.replace("\t", "\\t");
				if (mList[i]->cs)
				{
					str.replace(mList[i]->replace, mList[i]->with,
							Qt::CaseSensitive);
				}
				else
				{
					str.replace(mList[i]->replace, mList[i]->with,
							Qt::CaseInsensitive);
				}
			}
		}
	}

	return str;
}

void ReplacementEngine::swap(int i, int j)
{
	mList.swap(i, j);
}

void ReplacementEngine::processFile( QString input, QString output )
{
	QString content;
	QFile file(input);
	if (file.open(QFile::ReadOnly))
	{
		QTextStream stream(&file);
		stream.setCodec("UTF-8");
		content = stream.readAll();

                if( content == doReplacements(content) )
                {
                    qDebug() << "The same";
                }

		content = doReplacements(content);

		QFile outfile(output);
		if (outfile.open(QFile::WriteOnly))
		{
			QTextStream outstream(&outfile);
			outstream.setCodec("UTF-8");
			outstream << content;
		}
	}
}

void ReplacementEngine::readFromFile(QString filename)
{
	QFile file(filename);
	QXmlInputSource inputSource(&file);

	QXmlSimpleReader reader;
	ReadSettings handler;
	reader.setContentHandler(&handler);
	reader.setErrorHandler(&handler);
	reader.parse(inputSource);

	clear();
	for (int i = 0; i < handler.list.size(); i++)
		add(new ReplacementItem(handler.list[i]->replace,
				handler.list[i]->with, handler.list[i]->regex,
				handler.list[i]->recursive, handler.list[i]->cs));
}
