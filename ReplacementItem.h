#ifndef REPLACEMENTITEM_H
#define REPLACEMENTITEM_H

class ReplacementItem
{
public:
	QString replace, with;
	bool regex, recursive, cs;

	ReplacementItem(QString r, QString w, bool reg, bool rec,
			bool casesensitive)
	{
		replace = r;
		with = w;
		regex = reg;
		recursive = rec;
		cs = casesensitive;
	}

	ReplacementItem()
	{
	}
};

#endif // REPLACEMENTITEM_H
