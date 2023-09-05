#pragma once

#include "Collectable.h"
#include "Reference.h"

class Blob;
class Blorp;

class Blob : public GC::Collectable
{
public:
	Blob();
	virtual ~Blob();

	virtual bool IterationBegin(void*& userData) override;
	virtual GC::Object* IterationNext(void* userData) override;
	virtual void IterationEnd(void* userData) override;

	GC::Reference<Blorp, false> ref;
};

class Blorp : public GC::Collectable
{
public:
	Blorp();
	virtual ~Blorp();

	virtual bool IterationBegin(void*& userData) override;
	virtual GC::Object* IterationNext(void* userData) override;
	virtual void IterationEnd(void* userData) override;

	GC::Reference<Blob, false> ref;
};