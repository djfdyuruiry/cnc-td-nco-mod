#pragma once

#include "IRulesIniSection.h"
#include "RulesIniRule.h"
#include "RULES_CACHE_KEY.h"

class IRulesIni
{
protected:
	IRulesIni()
	{
	}

public:
	virtual ~IRulesIni()
	{
	}

	virtual IRulesIni& AndThenFrom(const char* rulesFilePath) = 0;

	virtual IRulesIni& WithSections(void (*rulesSetup)(IRulesIni&)) = 0;

	virtual Optional& ReadOptionalStringRule(RulesIniRule& rule) = 0;

	virtual const char* ReadStringRule(RulesIniRule& rule) = 0;

	virtual Optional& ReadOptionalIntRule(RulesIniRule& rule) = 0;

	virtual int ReadIntRule(RulesIniRule& rule) = 0;

	virtual bool HasSection(CacheKey key) = 0;

	virtual bool HasSectionForRule(RulesIniRule& rule) = 0;

	virtual IRulesIniSection& GetSectionForRule(RulesIniRule& rule) = 0;

	virtual void MarkAsInvalid() = 0;

	virtual bool IsValid() = 0;

	virtual IRulesIni& operator<<(SectionName section) = 0;

	virtual IRulesIni& operator<<(RulesIniType sectionDefaultType) = 0;

	virtual IRulesIni& operator<<(IRulesIniSection& section) = 0;

	virtual IRulesIni& operator<<(RulesIniRule& rule) = 0;

	virtual IRulesIniSection& operator[](CacheKey sectionKey) = 0;

	virtual IRulesIniSection& operator[](SectionName name) = 0;
};
