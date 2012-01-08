#include "ReadSettings.h"

ReadSettings::ReadSettings()
{
}

ReadSettings::~ReadSettings()
{
}

bool ReadSettings::startElement(const QString &namespaceURI,
						const QString &localName,
						const QString &qName,
						const QXmlAttributes &attributes)
{
	currentString.clear();

	if(localName=="item")
	{
	    list << new ReplacementItem;
	    list.last()->cs = attributes.value("casesensitive").toInt();
	    list.last()->recursive = attributes.value("recursive").toInt();
	    list.last()->regex = attributes.value("regex").toInt();
	}

	return true;
}

bool ReadSettings::endElement(const QString &namespaceURI,
						const QString &localName,
						const QString &qName)
{
	if(qName=="replace") {
	    list.last()->replace = currentString;
	} else if(qName=="with") {
	    list.last()->with = currentString;
	}
	return true;
}

bool ReadSettings::characters(const QString &str)
{
	currentString += str;
	return true;
}

bool ReadSettings::fatalError(const QXmlParseException &exception)
{
	return true;
}
